#pragma once
#include "Huffman_Tree.h"
#include "compress.h"

//用来存储哈夫曼编码映射到字符的关系
map<string, int> key;
//用来存储生成的哈夫曼文件的所有字节
char* file;
//在位运算的时候用来表示file的第几个char变量
int buffer_ptr;
//用来表示file的第buffer_ptr的第几个位
int count_bit;
//用来存储转换为用来形式的编码的所有字节
char* reduction;
//读取生成的哈夫曼文件
void read(char* source_file_name) {
	ifstream in;
	in.open(source_file_name, ios::binary);
	int n;
	in.read((char*)& n, sizeof(int));
	for (int i = 0; i < n; i++) {
		char buffer;
		in.read(&buffer, sizeof(char));
		int x = (unsigned int)buffer;
		int temp;
		in.read((char*)& temp, sizeof(int));
		res[(unsigned char)buffer] = temp;
	}
	in.read((char*)& buffer_ptr, sizeof(int));
	in.read((char*)& count_bit, sizeof(int));
	file = new char[buffer_ptr + 1];
	for (int i = 0; i <= buffer_ptr; i++) {
		char buffer;
		in.read(&buffer, sizeof(char));
		file[i] = buffer;
		int x = (unsigned char)buffer;
	}
	in.read((char*)& _file_length, sizeof(int));
}
void get_string_map() {
	for (int i = 0; i < 256; i++) {
		if (!code[i].empty()) {
			key.insert(pair<string, int>(code[i], i));
		}
	}
}
void convert() {
	reduction = new char[_file_length];
	string temp;
	int ptr = 0;//计数字符位
	int i = 0;//计数bit
	int m = 0;//将结果输入结果字符串的索引
	while (1) {
		if (m == _file_length)break;
		int x = (unsigned char)file[ptr];
		x = x & (1 << (7 - i));
		i++;
		if (i == 8) {
			ptr++;
			i = 0;
		}
		if (x) {
			temp.push_back('1');
		}
		else temp.push_back('0');
		if (key.count(temp)) {
			reduction[m++] = key[temp];
			temp.clear();
		}
	}
}
void reduction_file(char* filename) {
	string temp = filename;
	for (int i = 0; i < Len; i++) {
		temp.pop_back();
	}
	ofstream out;
	out.open(temp, ios::out | ios::binary);
	for (int i = 0; i < _file_length; i++) {
		char tmp = reduction[i];
		out.write(&tmp, sizeof(char));
	}
	out.close();
}
void decompress(char* file_name) {
	read(file_name);
	priority_queue<node> queue = get_queue(res);
	node* head = Huffman_Tree(queue);
	get_code(head);
	get_string_map();
	convert();
	reduction_file(file_name);
}