#include <iostream>
#include <string>
#include <fstream>
using namespace std;

string file_hdr = "/home/suen/Work/xv6_lab/mit_6.S081_lab";

int main()
{
	string t;
	ifstream ifs;
	ifs.open("find_files");

	ofstream ofs;
	ofs.open("../cscope.files");

	while(ifs >> t)
	{
		t.erase(0,2);
		ofs << (file_hdr + t) << "\n";
	}

	ifs.close();
	ofs.close();

	return 0;
}
