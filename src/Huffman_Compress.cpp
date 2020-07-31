#include "compress.h"
#include "decompress.h"

int main(int argc, char** argv)
{
	if (argc < 2) {
		cout << "Without parameter, the program can't run." << endl;
		return 0;
	}
	string file_name = argv[1];
	if (file_name.size() > Len) {
		string temp = file_name.substr(file_name.length() - Len, file_name.length());
		if (temp == suffix) {
				decompress(argv[1]);
		}
		else {
			compress(argv[1]);
		}
	}
	else {
		compress(argv[1]);
	}
}

