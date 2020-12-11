// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

// 0    ... 4095 is 1
// 4096 ... 8191 is 2
// so >> 12 means the no of per 4096 bytes physical page
#define PA2INDEX(pa) (((uint64)pa) >> 12)

//struct spinlock free_lk;

// physical pages counter(the biggest address is lower than
// PHYSTOP because PHYSTOP - 0x80000000 is the size of DRAM)
struct {
	struct spinlock lk; // use spinlock to protect counter 
	int counter[PGROUNDDOWN(PHYSTOP) / PGSIZE + 1];
} p_count;

void init_counter()
{
  initlock(&p_count.lk, "p_count");
  acquire(&p_count.lk);
  int sz = PGROUNDDOWN(PHYSTOP) / PGSIZE + 1;
  for(int i = 0;i < sz;++i)
    p_count.counter[i] = 0;
  
  release(&p_count.lk);
}

void incr_counter(uint64 pa)
{
  acquire(&p_count.lk);
  ++p_count.counter[PA2INDEX(pa)];
  release(&p_count.lk);
}

void decr_counter(uint64 pa)
{ 
  acquire(&p_count.lk);
  --p_count.counter[PA2INDEX(pa)];
  release(&p_count.lk);
}

int get_counter(uint64 pa)
{
  acquire(&p_count.lk);
  int ret = p_count.counter[PA2INDEX(pa)];
  release(&p_count.lk);
  return ret;
}

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

void
kinit()
{
  init_counter();
//  initlock(&free_lk,"free_lk");
  initlock(&kmem.lock, "kmem");
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
  {
    incr_counter((uint64)p);
	kfree(p);
  }
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  decr_counter((uint64)pa);
  
  acquire(&p_count.lk);
  int cnt = p_count.counter[PA2INDEX(pa)];
  release(&p_count.lk);
  if(cnt > 0)
    return;

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  if(r){
    memset((char*)r, 5, PGSIZE); // fill with junk
    incr_counter((uint64)r);
  }
  return (void*)r;
}
