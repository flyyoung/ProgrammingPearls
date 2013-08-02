/*
编程珠玑第1章
多路归并排序


*/

#include <iostream>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <ctime>
using namespace std;

#define MAX 10000                //    总数据量，可修改
#define MAX_ONCE 2500        //    内存排序MAX_ONCE个数据
#define FILENAME_LEN 20

//range:范围
//num :个数
void Random(int range, int num)
{
    int *a = new int[range];
    int i, j;

    srand(time(NULL));
    fstream dist_file;

    for(i = 0; i < range; i++)
    {
        a[i] = i + 1;
    }

    //预处理
    for(j = 0; j < range; j++)
    {
        int ii = (rand()*RAND_MAX+rand()) % range,
            jj = (rand()*RAND_MAX+rand()) % range;
        swap(a[ii], a[jj]);
    }//for

    dist_file.open("data.txt", ios::out);

    //写入文件
    for(i = 0; i < num; i++)
    {
        dist_file << a[i] << " ";
    }

    //回收
    delete []a;
    dist_file.close();
}

//index: 文件的下标
char *create_filename(int index)
{
    char *a = new char[FILENAME_LEN];
    sprintf(a, "data %d.txt", index);
    return a;
}

//num：每次读入内存的数据量
void mem_sort(int num)
{
    fstream fs("data.txt",ios::in);
    int temp[MAX_ONCE];        //内存数据暂存
    int file_index = 0;        //文件下标

    int count;                //实际读入内存数据量
    bool eof_flag = false;    //文件末尾标识

    while(!fs.eof())
    {
        count = 0;
        for(int i = 0; i < MAX_ONCE; i++)
        {
            fs >> temp[count];

            //读入一个数据后判断是否到了末尾
            if(fs.peek() == EOF)
            {
                eof_flag = true;
                break;
            }//if

            count++;
        }//for

        if(eof_flag)    //如果到达文件末尾就不会再再去执行下面的部分
        {
            break;
        }

        //内存排序
        //sort(temp, temp + count, comp);
        sort(temp, temp + count);

        //写入文件
        char *filename = create_filename(++file_index);
        fstream fs_temp(filename, ios::out);
        for(int i = 0; i < count; i++)
        {
            fs_temp << temp[i] << " ";
        }
        fs_temp.close();
        delete []filename;
    }//while

    fs.close();
}

void merge_sort(int filecount)
{
    fstream *fs = new fstream[filecount];
    fstream result("result.txt", ios::out);

    int index = 1;
    int temp[MAX_ONCE];
    int eofcount = 0;
    bool *eof_flag = new bool[filecount];
    memset(eof_flag, false, filecount *sizeof(bool));

    for(int i = 0; i < filecount; i++)
    {
        fs[i].open(create_filename(index++), ios::in);//打开分段文件
    }

    for(int i = 0; i < filecount; i++)
    {
        fs[i] >> temp[i];
    }
//分别打开分段文件，每次各读取一个数，然后比较，将最小的数写入文件，直到所有分段文件都被读取完。
    while(eofcount < filecount)
    {
        int j = 0;

        //找到第一个未结束处理的文件
        while(eof_flag[j])
        {
            j++;
        }

        int min = temp[j];
        int fileindex = 0;
        for(int i = j + 1; i < filecount; i++)
        {
            if(temp[i] < min && !eof_flag[i])
            {
                min = temp[i];
                fileindex = i;
            }
        }//for

        result << min << " ";
        fs[fileindex] >> temp[fileindex];

        //末尾判断
        if(fs[fileindex].peek() == EOF)
        {
            eof_flag[fileindex] = true;
            eofcount++;
        }
    }//while

    delete []fs;
    delete []eof_flag;
    result.close();
}

int main()
{
    Random(MAX, MAX);
    clock_t start = clock();
    mem_sort(MAX);
    merge_sort(4);
    clock_t end = clock();

    double cost = (end - start) * 1.0 / CLK_TCK;
    cout << "耗时" << cost << "s" << endl;
    return 0;
}
