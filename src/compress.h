#pragma once
#include "Huffman_Tree.h"


using namespace std;
//频率数组
int res[256];
//存储出现的字节的哈夫曼编码
string code[256];
//存储所有的字节
//vector<char> all_alpha;
//输入文件的长度
int _file_length;

//用于get_code 函数暂存字符串
string temp;

//获取每个字节及其出现的频数
void get_frequency(char* source_file_name) {
	ifstream in;
	in.open(source_file_name, ios::binary);
	char buffer;
	in.read(&buffer, sizeof(char));
	while (!in.eof()) {
		res[(unsigned char)(buffer)]++;
		//all_alpha.push_back(buffer);
		in.read(&buffer, sizeof(char));
	}
	in.close();
}
//获取输入文件的字节长度
void file_length() {
	for (int i = 0; i < 256; i++) {
		_file_length += res[i];
	}
}

//遍历树并得到编码
void get_code(node* head) {
	if (head->left == nullptr && head->right == nullptr) {
		int value = head->value;
		code[value] = temp;
		return;
	}
	temp.push_back('0');
	get_code(head->left);
	temp.pop_back();
	temp.push_back('1');
	get_code(head->right);
	temp.pop_back();
}

//将频率数组与哈夫曼编码按位和二进制文件的方式写进文件
void write_out(char* source_file_name) {
	ifstream in;
	in.open(source_file_name, ios::binary);
	char buffer;
	string temp;
	char* output = new char[_file_length];
	memset(output, 0, sizeof(char) * _file_length);
	//重新读文件，因为如果将所有的字节直接读进数组或vector内存可能会炸
	in.read(&buffer, sizeof(char));
	int count = 0;
	long long int buffer_ptr = 0;
	string total;
	while (!in.eof()) {
		//在这里面将原字节转换为哈夫曼编码并压进新的字节
		string temp = code[(unsigned char)buffer];
		total += temp;
		for (int i = 0; i < temp.size(); i++) {
			int x = temp[i] - '0';
			x = x << (7 - count);
			output[buffer_ptr] = output[buffer_ptr] | x;
			count++;
			if (count == 8) {
				count = 0;
				buffer_ptr++;
			}
		}
		in.read(&buffer, sizeof(char));
	}
	in.close();
	//开始输出
	ofstream out;
	string out_name = source_file_name;
	out_name += suffix;//加上后缀名
	out.open(out_name, ios::out | ios::binary);
	//统计出现多少种字节
	int total_num = 0;
	for (int i = 0; i < 256; i++) {
		if (res[i])total_num++;
	}
	//将所有字节数存入文件，在读取的时候就可以用for循环定位
	out.write((char*)& total_num, sizeof(int));
	for (int i = 0; i < 256; i++) {
		//将该文件出现过的字节极其对应的哈夫曼编码存进文件，在解码的时候再构建树获取
		if (res[i]) {
			buffer = i;
			int num = res[i];
			out.write(&buffer, sizeof(char));
			out.write((char*)& num, sizeof(int));
		}
	}

	out.write((char*)& buffer_ptr, sizeof(int));
	out.write((char*)& count, sizeof(int));
	for (int i = 0; i <= buffer_ptr; i++) {
		char temp = output[i];
		int x = (unsigned char)output[i];
		out.write(&temp, sizeof(char));
	}
	out.write((char*)& _file_length, sizeof(int));
	out.close();
	delete[]output;
}

void compress(char* file) {
	get_frequency(file);
	file_length();
	priority_queue<node> queue = get_queue(res);
	node* head = Huffman_Tree(queue);
	get_code(head);
	write_out(file);
}

