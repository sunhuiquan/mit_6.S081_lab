# mit_6.S081_lab

### 具体的代码只需切换到对应的分支

### Lab 1：2020/11/20

配工具配半天，qemu版本太低自己built了环境，无感，实验不难，
第一次真正接触管道。。。以前的想法有误。。。顺利完成。

### Lab 2：2020/11/23

这个轻松，没啥障碍顺利完成。

### Lab 3：2020/11/29

费劲千辛万苦搞完了，debug了半天，虽然通过了lab，但是ans.txt
和一些关于exec和sbrk的细节还没处理好，不过无所谓，剩下的没啥
技术含量了就省略了。

### Lab 4：2020/12/4

实验倒是相对Lab3简单的多，顺利完成，但是看xv6 book的一些细节
搞得爆炸，最后发现一定要粗略看一遍xv6 book后就去看课程，然后
明确细节，再细读一遍xv6 book，此时应该理解的不错了。(不看课程
真的容易自闭，卡死在细节上。)

一开始没有借鉴trapframe的做法，神经病地一样一个一个寄存器的
保存。。。后来发现一个memmove就完成的事情。。。

结果证明直接使用上面说的方法，不加思考会把人逼疯。。发现一个十分细
微的bug，查了一个多小时。不要改动任何不必要的地方，不然有些细节bug
根本发现不了，不要试图改变非用户寄存器，即使下一行就恢复它，详情看
sysproc.c写的注释。。。一点不顺利地完成了lab4.

### Lab 5：2020/12/7

顺利完成，比前面简单多了。

### Lab 6：2020/12/11

出了非常奇怪的错误，查了半天，最后终于过了，心累。

### Lab 7：2020/12/19

这个lab的代码很简单，但是xv6 book这一章真是读吐了，一堆东西，还是
这个lab的part3的barrier我一开始以为是xv6 book ch6中的memory barrier，后来才反应过来这完全两个东西。

### Lab 8：2020/12/20

整体上和lab7一样难度一般，但这里出了个非常奇葩的bug，不是代码的问题
而是电脑的问题，重启虚拟机后就好了。。。看来一生中总会遇到那么一些玄
学bug。。。好在最后终于通过了。

### to do： 
Lab 9：
Lab 10：
Lab 11：
