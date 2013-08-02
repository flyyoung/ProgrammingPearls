/*
编程珠玑第1章
位图

*/

#include <iostream>
#include <algorithm>
#include <ctime>
#include <bitset>
#include <cstdio>
using namespace std;

#define DATA_NUM 10000         //生成的随机数的个数
#define SOURCE "data.txt"      //保存随机数的文件名称
#define RESULT "result.txt"    //保存排序结果的文件名称

//功能：生成随机数文件
//参数：num 生成随机数的个数
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

    srand(time(NULL));//设置当前时间为随机数种子

    for(int i = 0; i < DATA_NUM; i++)
    {
        int ii = (rand() * RAND_MAX + rand()) % DATA_NUM;
        int jj = (rand() * RAND_MAX + rand()) % DATA_NUM;
        swap(temp[ii], temp[jj]);
    }

    //写入文件
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
    cout << "随机数文件生成成功." << endl;
}

void BitMapSort()
{
    bitset<DATA_NUM + 1> bitmap;
    bitmap.reset();    //设置每位为0

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

    //将排序好的数写入到结果文件中
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

    cout << "排序成功." << endl;
    fclose(fpdst);
}


int main()
{
    make_data(DATA_NUM);

    clock_t start = clock();
    BitMapSort();
    clock_t finish = clock();

    cout << "排序所用时间为:" << (finish - start) * 1.0 / CLOCKS_PER_SEC << "s" << endl;
    return 0;
}
