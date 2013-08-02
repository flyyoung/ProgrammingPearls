/*
������᣺��2�±�λ��

*/
#include <iostream>
#include <set>
#include <stdio.h>
#include <sys/stat.h>
#include <fstream>
#include <map>
#include <cstdlib>
#include <cstring>

using namespace std;

#define SOURCEFILE  "big.txt"     //ԭ�ļ�������С˵��һ���֣�Ϊ�˳�ȡ����
#define DESTFILE    "dict.txt"	  //�γɵĴʵ�  ��ǩ ����
#define RESULTFILE  "result.txt"  //����ļ�
#define MAXNUM      100			  //���ʵ���󳤶�
#define DEBUG

set<string> dict;				 //���洦��õ��ֵ�

//��Ҫɾ�����ַ�
const char delim[] = ".,:;`'/\"+-_(){}[]<>*&^%$#@!?~/|\\=1234567890 \t\n";

/*
 * �������ܣ����ַ����е��ַ���ȫ��ת��ΪСд
 * ����ֵ  ��ת������ַ���ָ��
 * ����    ��
 *	@prama *word ָ��Դ�ַ�����ָ��
 */
static char *strtolower(char *word)
{
	char *s;
	if(word == NULL)
	{
		return NULL;
	}
	for(s = word; *s != '\0'; s++)
	{
		*s = tolower(*s);
	}

	return word;
}

/*
 * �������ܣ���ȡ�ļ��еĹؼ���
 * ����ֵ  ��ת���ɹ�����1����֮������0
 * ����    ��
 *   @prama  ��
 */
static int read_file()
{
	char *file, *word, *w;
	FILE *fp = fopen(SOURCEFILE, "r");
	struct stat sb;

	if(!fp)
	{
		return 0;
	}
	 /*
	  int stat(const char *file_name, struct stat *buf)
	        ͨ���ļ���filename��ȡ�ļ���Ϣ����������buf��ָ�Ľṹ��stat��
	        ִ�гɹ��򷵻�0��ʧ�ܷ���-1������������errno
	*/
	if(stat(SOURCEFILE, &sb))
	{
		return 0;
	}

	file = (char *)malloc(sb.st_size);
	if(!file)
	{
		fclose(fp);
		return 0;
	}

	/*
	    size_t fread ( void   *buffer,  size_t size,  size_t count,  FILE *stream)
	    ��һ���ļ����ж����ݣ�����ȡcount��Ԫ�أ�ÿ��Ԫ��size�ֽڣ�������óɹ�����ʵ�ʶ�ȡ����Ԫ�ظ�����������ɹ����� 0��
	    */

	fread(file, sizeof(char), sb.st_size, fp);
	 /*
	    char *strtok(char s[], const char *delim)
	    �ֽ��ַ���Ϊһ���ַ�����sΪҪ�ֽ���ַ�����delimΪ�ָ����ַ���

	    extern char *strdup(char *s)
	    strdup()���ڲ�������malloc()Ϊ���������ڴ棬����Ҫʹ�÷��ص��ַ���ʱ����Ҫ��free()�ͷ���Ӧ���ڴ�ռ䣬���������ڴ�й©��
	    ����һ��ָ��,ָ��Ϊ�����ַ�������Ŀռ�;�������ռ�ʧ��,�򷵻�NULLֵ��
	    */
	word = strtok(file, delim);
	while(word != NULL)
	{
		w = strtolower(strdup(word));

		string str = w;
		dict.insert(str);
      /*
       * ��ÿ���ҵ�һ���ָ�����һ���գ�NULL���ͱ��ŵ��ָ����������Դ�ʱNULLָ��ָ�������ַ���
       */
		word = strtok(NULL, delim);
	}

	free(file);
	fclose(fp);
	return 1;
}

/*
 * �������ܣ�����qsort()����
 */
int comp(const void *a, const void *b)
{
	return *(char *)a - *(char *)b;
}

/*
 * �������ܣ����ֵ��еĵ��ʱ��浽�ļ���, ���Ұѱ�ǩ��������
 * ����ֵ  ������ɹ�����1����֮���򷵻�0
 * ����    ��
 *	  @prama  ��
 *
 *		�����ʽ����ǩ  ����
 *				  ��ǩ  ����
 *				  ....  ....
 */
int SaveToFile()
{
	FILE *out;
	out = fopen(DESTFILE, "w");
	if(out == NULL)
	{
		cout << "fopen() error in SaveToFile()." << endl;
		return 0;
	}

	set<string>::iterator iter = dict.begin();
	for( ; iter != dict.end(); iter++)
	{
		char tmpone[MAXNUM] = {'\0'};
		char tmptwo[MAXNUM] = {'\0'};
		/*
		        char * strncpy(char *dest, char *src,size_t num)
		        ����src�е����ݣ��ַ������֡�����....����dest�����ƶ�����num��ֵ����������ָ��dest��ָ�롣
		        */
		strncpy(tmpone, (*iter).c_str(), MAXNUM);
		strncpy(tmptwo, (*iter).c_str(), MAXNUM);

		qsort(tmpone, strlen(tmpone), sizeof(char), comp);

		#ifdef DEBUG
			cout << tmpone << " "<< *iter << endl;
		#endif

		fprintf(out, "%s %s\n", tmpone, tmptwo);
	}

	fclose(out);

	cout << "�ֵ䱣�浽�ļ��ɹ�." << endl;
	return 1;
}

/*
 * �������ܣ���ȡ����õ��ֵ��ļ�������ѹ������ȡ���յĽ��
 * ����ֵ  ����ȡ�ɹ�������1;��֮������0
 * ����    ��
 *	  @prama  ��
 */
int squash()
{
	//��dict.txt�ļ�
	ifstream infile(DESTFILE, ios::in);
	if(infile == NULL)
	{
		return 0;
	}

	//��ȡ�ļ����ݣ����浽multimap<string, string>��
	multimap<string, string> res;
	string sig, word;
	while(infile >> sig >> word)
	{
		//cout<<sig<<"....."<<word<<endl;
		res.insert(make_pair(sig, word));
	}

	//�򿪱������ս�����ļ�
	ofstream outfile(RESULTFILE, ios::out);
	if(outfile == NULL)
	{
		return 0;
	}

	//���д���
	multimap<string, string>::iterator iter = res.begin();
	if(iter == res.end())
	{
		return 0;
	}

	sig = iter->first;
	word = iter->second;
	outfile << " " << word;

	for( ; iter != res.end(); iter++)
	{
		char tmpone[MAXNUM] = {'\0'};
		char tmptwo[MAXNUM] = {'\0'};

		strncpy(tmpone, (iter->first).c_str(), MAXNUM);
		strncpy(tmptwo, (iter->second).c_str(), MAXNUM);

		//cout<<tmpone<<"******"<<tmptwo<<endl;

		if(strcmp(sig.c_str(), tmpone) == 0)
		{
			if(strcmp(word.c_str(), tmptwo) == 0)
			{
				//same word here
				continue;
			}
			else
			{
				outfile << " " << tmptwo;
			}
		}
		else
		{
			outfile << endl;
			outfile  << " " << tmptwo;
			sig = tmpone;
			word = tmptwo;
		}
	}
	infile.close();
	outfile.close();
	return 1;
}

int main()
{

	//��ȡ�ļ��������ʵ�
	if(!read_file())
	{
		return EXIT_FAILURE;
	}

	//�����ֵ䵽�ļ���
	if(!SaveToFile())
	{
		return EXIT_FAILURE;
	}

	//�������������
	if(!squash())
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
