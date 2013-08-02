/*
编程珠玑：第2章变位词

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

#define SOURCEFILE  "big.txt"     //原文件，就是小说的一部分，为了抽取单词
#define DESTFILE    "dict.txt"	  //形成的词典  标签 单词
#define RESULTFILE  "result.txt"  //结果文件
#define MAXNUM      100			  //单词的最大长度
#define DEBUG

set<string> dict;				 //保存处理好的字典

//需要删除的字符
const char delim[] = ".,:;`'/\"+-_(){}[]<>*&^%$#@!?~/|\\=1234567890 \t\n";

/*
 * 函数功能：将字符串中的字符串全部转化为小写
 * 返回值  ：转换后的字符串指针
 * 参数    ：
 *	@prama *word 指向源字符串的指针
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
 * 函数功能：读取文件中的关键字
 * 返回值  ：转化成功返回1；反之，返回0
 * 参数    ：
 *   @prama  无
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
	        通过文件名filename获取文件信息，并保存在buf所指的结构体stat中
	        执行成功则返回0，失败返回-1，错误代码存于errno
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
	    从一个文件流中读数据，最多读取count个元素，每个元素size字节，如果调用成功返回实际读取到的元素个数，如果不成功返回 0。
	    */

	fread(file, sizeof(char), sb.st_size, fp);
	 /*
	    char *strtok(char s[], const char *delim)
	    分解字符串为一组字符串。s为要分解的字符串，delim为分隔符字符串

	    extern char *strdup(char *s)
	    strdup()在内部调用了malloc()为变量分配内存，不需要使用返回的字符串时，需要用free()释放相应的内存空间，否则会造成内存泄漏。
	    返回一个指针,指向为复制字符串分配的空间;如果分配空间失败,则返回NULL值。
	    */
	word = strtok(file, delim);
	while(word != NULL)
	{
		w = strtolower(strdup(word));

		string str = w;
		dict.insert(str);
      /*
       * 即每次找到一个分隔符后，一个空（NULL）就被放到分隔符处，所以此时NULL指针指向后面的字符串
       */
		word = strtok(NULL, delim);
	}

	free(file);
	fclose(fp);
	return 1;
}

/*
 * 函数功能：用于qsort()函数
 */
int comp(const void *a, const void *b)
{
	return *(char *)a - *(char *)b;
}

/*
 * 函数功能：将字典中的单词保存到文件中, 并且把标签给求解出来
 * 返回值  ：保存成功，返1；反之，则返回0
 * 参数    ：
 *	  @prama  无
 *
 *		保存格式：标签  单词
 *				  标签  单词
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
		        复制src中的内容（字符，数字、汉字....）到dest，复制多少由num的值决定，返回指向dest的指针。
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

	cout << "字典保存到文件成功." << endl;
	return 1;
}

/*
 * 函数功能：读取保存好的字典文件，进行压缩，获取最终的结果
 * 返回值  ：读取成功，返回1;反之，返回0
 * 参数    ：
 *	  @prama  无
 */
int squash()
{
	//打开dict.txt文件
	ifstream infile(DESTFILE, ios::in);
	if(infile == NULL)
	{
		return 0;
	}

	//读取文件内容，保存到multimap<string, string>中
	multimap<string, string> res;
	string sig, word;
	while(infile >> sig >> word)
	{
		//cout<<sig<<"....."<<word<<endl;
		res.insert(make_pair(sig, word));
	}

	//打开保存最终结果的文件
	ofstream outfile(RESULTFILE, ios::out);
	if(outfile == NULL)
	{
		return 0;
	}

	//进行处理
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

	//读取文件，创建词典
	if(!read_file())
	{
		return EXIT_FAILURE;
	}

	//保存字典到文件中
	if(!SaveToFile())
	{
		return EXIT_FAILURE;
	}

	//进行最后结果处理
	if(!squash())
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
