/*
��������1��
��·�鲢����


*/

#include <iostream>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <ctime>
using namespace std;

#define MAX 10000                //    �������������޸�
#define MAX_ONCE 2500        //    �ڴ�����MAX_ONCE������
#define FILENAME_LEN 20

//range:��Χ
//num :����
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

    //Ԥ����
    for(j = 0; j < range; j++)
    {
        int ii = (rand()*RAND_MAX+rand()) % range,
            jj = (rand()*RAND_MAX+rand()) % range;
        swap(a[ii], a[jj]);
    }//for

    dist_file.open("data.txt", ios::out);

    //д���ļ�
    for(i = 0; i < num; i++)
    {
        dist_file << a[i] << " ";
    }

    //����
    delete []a;
    dist_file.close();
}

//index: �ļ����±�
char *create_filename(int index)
{
    char *a = new char[FILENAME_LEN];
    sprintf(a, "data %d.txt", index);
    return a;
}

//num��ÿ�ζ����ڴ��������
void mem_sort(int num)
{
    fstream fs("data.txt",ios::in);
    int temp[MAX_ONCE];        //�ڴ������ݴ�
    int file_index = 0;        //�ļ��±�

    int count;                //ʵ�ʶ����ڴ�������
    bool eof_flag = false;    //�ļ�ĩβ��ʶ

    while(!fs.eof())
    {
        count = 0;
        for(int i = 0; i < MAX_ONCE; i++)
        {
            fs >> temp[count];

            //����һ�����ݺ��ж��Ƿ���ĩβ
            if(fs.peek() == EOF)
            {
                eof_flag = true;
                break;
            }//if

            count++;
        }//for

        if(eof_flag)    //��������ļ�ĩβ�Ͳ�������ȥִ������Ĳ���
        {
            break;
        }

        //�ڴ�����
        //sort(temp, temp + count, comp);
        sort(temp, temp + count);

        //д���ļ�
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
        fs[i].open(create_filename(index++), ios::in);//�򿪷ֶ��ļ�
    }

    for(int i = 0; i < filecount; i++)
    {
        fs[i] >> temp[i];
    }
//�ֱ�򿪷ֶ��ļ���ÿ�θ���ȡһ������Ȼ��Ƚϣ�����С����д���ļ���ֱ�����зֶ��ļ�������ȡ�ꡣ
    while(eofcount < filecount)
    {
        int j = 0;

        //�ҵ���һ��δ����������ļ�
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

        //ĩβ�ж�
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
    cout << "��ʱ" << cost << "s" << endl;
    return 0;
}
