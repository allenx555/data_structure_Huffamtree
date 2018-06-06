#include "functions.h"

struct HuffmanTree {
	int weight = 0;
	int parent = -1;
	char data[1];
	int lchirld = -1;
	int rchirld = -1;
}Leaf[253];//ascii��127������໹��126������ϼ��ڵ�

struct HuffmanCode {//���Դ������Ľṹ����
	int code[127] = { -1 };
}Codes[127];

int data[128] = { 0 };//���ڴ���ÿ���ַ���weight

void Textcount() {//�ļ��Ķ�ȡ
	FILE *fp;
	int i;

	fp = fopen("data.txt", "r");
	if (fp == NULL) {
		printf("�ļ������ڻ��޷���\n");
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


void Creatree() {//����Huffman��
	int i,j=0;
	int first, second;

	for (i = 0; i < 127;i++) {//���ĳ�ʼ��
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

void find_two_smallest(int *first, int *second) {//�Ƚ�weight��С
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

void Codetree() {//Huffman���ı���
	int i,pre,length,j,k=0,post,t,max;

	for (i = 127;;i++) {//���ҳ�ʼ�ڵ�
		max = i;
		if (Leaf[i].parent == 0)
			break;
	}
	for (i = 0;i < 127;i++) {//����
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
		for (j = 0, k = length - 1;j<k;j++, k--){//���鵹��
			t = Codes[i].code[j];
			Codes[i].code[j] = Codes[i].code[k];
			Codes[i].code[k] = t;
		}
		printf("\n%s", Leaf[i].data);//��ӡ����
		for (j = 0;Codes[i].code[j] != -1;j++) {
			printf(" %d", Codes[i].code[j]);//��������ṹ�����ڴ���ı���
		}
	}
}

void Textcode() {//�ļ��ı���
	FILE *fp_new,*fp_old;
	int i,j=0;

	fp_old = fopen("data.txt", "r");
	fp_new = fopen("data_new.txt", "w+");//�������ı��Դ�ű��������

	char ch;
	ch = fgetc(fp_old);
	printf("\nԭ�ģ�\n");
	do {//��ȡԭ����
		printf("%c",ch);
		ch = fgetc(fp_old);
	} while (ch != EOF);
	fp_old = fopen("data.txt", "r");
	ch = fgetc(fp_old);
	printf("\n�����\n");
	do {//��������
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

void Textranslate() {//������������
	FILE *fp;
	int i,num,next,max,point;

	for (i = 127;;i++) {//���ҳ�ʼ�ڵ�
		max = i;
		if (Leaf[i].parent == 0)
			break;
	}
	fp = fopen("data_new.txt", "r");
	char ch = fgetc(fp);
	point = max;
	printf("\n����Ϊ��\n");
	do {//����
		num = int(ch - 48);
		point = max;//����Ϊͷ���
		for (;;) {//��ѭ��Ϊ�����ַ�����ͷ��㿪ʼ����С��127��������ѭ�������ԭ�ַ�
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