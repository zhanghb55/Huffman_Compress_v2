#pragma once 
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <cstring>
#include <fstream>
using namespace std;

string suffix = ".zhbzip";
#define Len suffix.size()

//哈夫曼编码的节点
class node {
public:
	int value;
	int power;
	node* left;
	node* right;
	node() = default;
	node(int temp, int x, node* a, node* b) {
		value = temp;
		power = x;
		left = a;
		right = b;
	}
	//修改优先级，让比较权小的结点先合并
	friend bool operator < (const node& a, const node& b) {
		return a.power > b.power;
	}
};

//由频率数组得到叶节点的优先队列
priority_queue<node> get_queue(int*res) {
	priority_queue<node> temp;
	for (int i = 0; i < 256; i++) {
		if (res[i]) {
			node newone(i, res[i], nullptr, nullptr);
			temp.push(newone);
		}
	}
	return temp;
}
//由优先队列建树
node* Huffman_Tree(priority_queue<node> queue) {
	while (queue.size() > 1) {
		node* temp1 = new node(queue.top());
		queue.pop();
		node* temp2 = new node(queue.top());
		queue.pop();
		node temp(-1, temp1->power + temp2->power, temp2, temp1);
		queue.push(temp);
	}
	node* head = new node(queue.top());
	return head;
}
//删除树
void del(node* head) {
	if (head != NULL) {
		del(head->left);
		del(head->right);
		delete head;
	}
}