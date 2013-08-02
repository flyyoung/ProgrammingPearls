/*
��������1��
λͼ

*/

#include <iostream>
#include <algorithm>
#include <ctime>
#include <bitset>
#include <cstdio>
using namespace std;

#define DATA_NUM 10000         //���ɵ�������ĸ���
#define SOURCE "data.txt"      //������������ļ�����
#define RESULT "result.txt"    //�������������ļ�����

//���ܣ�����������ļ�
//������num ����������ĸ���
void make_data(int num)
{
    int *temp = new int[DATA_NUM];
    if(temp == NULL)
    {
        cout << "new error in make_data()" << endl;
        return;
    }

    for(int i = 0; i < DATA_NUM; i++)
    {
        temp[i] = i + 1;
    }

    srand(time(NULL));//���õ�ǰʱ��Ϊ���������

    for(int i = 0; i < DATA_NUM; i++)
    {
        int ii = (rand() * RAND_MAX + rand()) % DATA_NUM;
        int jj = (rand() * RAND_MAX + rand()) % DATA_NUM;
        swap(temp[ii], temp[jj]);
    }

    //д���ļ�
    FILE *fp;
    fp = fopen(SOURCE, "w");
    if(fp == NULL)
    {
        cout << "fopen() error in make_data()." << endl;
    }
    for(int i = 0; i < DATA_NUM; i++)
    {
        fprintf(fp, "%d ", temp[i]);
    }
    fclose(fp);
    cout << "������ļ����ɳɹ�." << endl;
}

void BitMapSort()
{
    bitset<DATA_NUM + 1> bitmap;
    bitmap.reset();    //����ÿλΪ0

    FILE *fpsrc;
    fpsrc = fopen(SOURCE, "r");
    if(fpsrc == NULL)
    {
        cout << "fopen() error in BitMapSort()" << endl;
        return;
    }

    int data;
    while(fscanf(fpsrc, "%d ", &data) != EOF)
    {
        if(data <= DATA_NUM)
        {
            bitmap.set(data, 1);
        }
    }

    //������õ���д�뵽����ļ���
    FILE *fpdst;
    fpdst = fopen(RESULT, "w");
    if(fpdst == NULL)
    {
        cout << "fopen() error in BitMapSort()." << endl;
        return;
    }

    for(int i = 0; i <= DATA_NUM; i++)
    {
        if(bitmap[i] == 1)
        {
            fprintf(fpdst, "%d ", i);
        }
    }

    cout << "����ɹ�." << endl;
    fclose(fpdst);
}


int main()
{
    make_data(DATA_NUM);

    clock_t start = clock();
    BitMapSort();
    clock_t finish = clock();

    cout << "��������ʱ��Ϊ:" << (finish - start) * 1.0 / CLOCKS_PER_SEC << "s" << endl;
    return 0;
}
