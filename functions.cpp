#include "functions.h"

struct HuffmanTree {
	int weight = 0;
	int parent = -1;
	char data[1];
	int lchirld = -1;
	int rchirld = -1;
}Leaf[253];//ascii共127个，最多还需126个存放上级节点

struct HuffmanCode {//用以储存编码的结构数组
	int code[127] = { -1 };
}Codes[127];

int data[128] = { 0 };//用于储存每个字符的weight

void Textcount() {//文件的读取
	FILE *fp;
	int i;

	fp = fopen("data.txt", "r");
	if (fp == NULL) {
		printf("文件不存在或无法打开\n");
		exit(EXIT_FAILURE);
	}
	char ch = fgetc(fp);
	do {
		data[int(ch)]++;
		ch = fgetc(fp);
	} while (ch != EOF);
	for (i = 0;i < 127;i++) {
		if (data[i] != 0)
			data[127]++;
	}
}


void Creatree() {//创建Huffman树
	int i,j=0;
	int first, second;

	for (i = 0; i < 127;i++) {//树的初始化
		if (data[i] == 0) {
			j++;
			continue;
		}
		Leaf[j].weight = data[i];
		Leaf[j].data[0] = char(i);
		Leaf[j].parent = 0;
		Leaf[j].rchirld = 0;
		Leaf[j].lchirld = 0;
		j++;
	}

	for (i = 0;i < (data[127]-1);i++) {
		find_two_smallest(&first,&second);
		Leaf[j].lchirld = first;
		Leaf[ j].rchirld = second;
		Leaf[ j].weight = Leaf[first].weight + Leaf[second].weight;
		Leaf[j].parent = 0;
		Leaf[j].data[0] = '-1';
		Leaf[first].parent = j;
		Leaf[second].parent = j;
 		j++;
	}
}

void find_two_smallest(int *first, int *second) {//比较weight大小
	int i,max=0;
	for (i = 0;i < 254;i++) {
		if (max < Leaf[i].weight)
			max = Leaf[i].weight + 1;
	}
	int smallest = max,second_smallest = max;
	*first = 0;
	*second = 0;
	for (i = 0; i< 254;i++) {
		if (smallest>=Leaf[i].weight&&Leaf[i].weight!=0&&Leaf[i].parent==0){
			smallest = Leaf[i].weight;
			*first = i;
		}
	}
	for (i = 0;i< 254;i++) {
		if (second_smallest<Leaf[i].weight || i == *first|| Leaf[i].weight== 0|| Leaf[i].parent != 0)
			continue;
		else {
			second_smallest = Leaf[i].weight;
			*second = i;
		}
	}
}

void Codetree() {//Huffman树的编码
	int i,pre,length,j,k=0,post,t,max;

	for (i = 127;;i++) {//查找初始节点
		max = i;
		if (Leaf[i].parent == 0)
			break;
	}
	for (i = 0;i < 127;i++) {//编码
		pre = i;
		if (Leaf[i].parent == -1)
			continue;
		post = i;
		length = 0;
		j = 0;
		while (pre != max) {
			post = pre;
			pre = Leaf[pre].parent;
			length++;
			if (Leaf[pre].lchirld == post)
				Codes[i].code[j] = 0;
			else
				Codes[i].code[j] = 1;
			j++;
			Codes[i].code[j] = -1;
		}
		for (j = 0, k = length - 1;j<k;j++, k--){//数组倒序
			t = Codes[i].code[j];
			Codes[i].code[j] = Codes[i].code[k];
			Codes[i].code[k] = t;
		}
		printf("\n%s", Leaf[i].data);//打印编码
		for (j = 0;Codes[i].code[j] != -1;j++) {
			printf(" %d", Codes[i].code[j]);//按序输出结构数组内储存的编码
		}
	}
}

void Textcode() {//文件的编码
	FILE *fp_new,*fp_old;
	int i,j=0;

	fp_old = fopen("data.txt", "r");
	fp_new = fopen("data_new.txt", "w+");//创建新文本以存放编码后文章

	char ch;
	ch = fgetc(fp_old);
	printf("\n原文：\n");
	do {//读取原文章
		printf("%c",ch);
		ch = fgetc(fp_old);
	} while (ch != EOF);
	fp_old = fopen("data.txt", "r");
	ch = fgetc(fp_old);
	printf("\n编译后：\n");
	do {//编码文章
		for (i = 0;i < 127;i++) {
			if (ch == Leaf[i].data[0]) {
				for (j = 0;Codes[i].code[j] != -1;j++) {
					fprintf(fp_new, "%d", Codes[i].code[j]);
					printf("%d", Codes[i].code[j]);
				}
				break;
			}
		}
		ch = fgetc(fp_old);
	} while (ch != EOF);

	fclose(fp_new);
	fclose(fp_old);
}

void Textranslate() {//翻译编码后文章
	FILE *fp;
	int i,num,next,max,point;

	for (i = 127;;i++) {//查找初始节点
		max = i;
		if (Leaf[i].parent == 0)
			break;
	}
	fp = fopen("data_new.txt", "r");
	char ch = fgetc(fp);
	point = max;
	printf("\n译文为：\n");
	do {//翻译
		num = int(ch - 48);
		point = max;//重置为头结点
		for (;;) {//内循环为单个字符，从头结点开始，当小于127是跳出内循环，输出原字符
			if (num == 0)
				point = Leaf[point].lchirld;
			else
				point = Leaf[point].rchirld;
			if (point < 127) {
				printf("%s", Leaf[point].data);
				break;
			}
			ch = fgetc(fp);
			num = int(ch - 48);
		}
		ch = fgetc(fp);
	} while (ch != EOF);
}