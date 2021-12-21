#include <windows.h>
#include <fstream>
#include <iostream>
using namespace std;

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
#define INF 800000
#define MAXN 99

int Chinese[MAXN][MAXN];

int judge()
{
	int format = 0;
    unsigned char check;
    ifstream File("Input.txt", ios::binary);

    File.read((char *)&check, sizeof(unsigned char));//读取第一个字节，然后左移8位
    int code = check << 8;

    File.read((char *)&check, sizeof(unsigned char));//读取第二个字节
    code |= check;

    switch (code)
    {
	    case 0xfffe:  //65534
	        format = 1; //Unicode
	        break;
	    case 0xfeff:  //65279
	        format = 2; //Unicode big endian
	        break;
	    case 0xe5ad:
	        format = 3; //UTF-8
	        break;
	 	default:
	        format = 4; //ANSI
	        break;
     }
     File.close();
     return format;
}

string UTF8ToGB(const char* str)
{
    string result;
    WCHAR *strSrc;
    LPSTR szRes;

    int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    strSrc = new WCHAR[i+1];
    MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

    i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
    szRes = new CHAR[i+1];
    WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

    result = szRes;
    delete [] strSrc;
    delete [] szRes;

    return result;
}

char *read(void)
{
	cout << "********************使用说明********************" << endl;
	cout << "*                                              *" << endl;
	cout << "* 1.请将要检索的文本放入“input”文档中        *" << endl;
	cout << "*                                              *" << endl;
	cout << "* 2.确认放入后，按下回车                       *" << endl;
	cout << "*                                              *" << endl;
	cout << "************************************************" << endl << endl; 
	while (!KEY_DOWN(13));

	ifstream File("Input.txt");
	while (!File.is_open())
	{
		cout << "无法找到文件!" << endl << endl;
		cout << "********************使用方法********************" << endl;
		cout << "*                                              *" << endl;
		cout << "* 1. 在相同文件夹中创建 Input.txt 文件         *" << endl;
		cout << "*                                              *" << endl;
		cout << "* 2. 将文章复制到 Input.txt 文件中             *" << endl;
		cout << "*                                              *" << endl;
		cout << "* 3. 按下回车键继续执行程序                    *" << endl;
		cout << "*                                              *" << endl;
		cout << "************************************************" << endl << endl;
		while (!KEY_DOWN(13));

		File.open("Input.txt");
	}
	cout << "正在读取文件..." << endl << endl;
	char *s = new char[INF];
	File.getline(s, INF, 0);
	File.close();
	return s;
}

void find(char *x)
{
	string a = x;
	int len = a.size();
	if (judge() == 3)
	{
		a = UTF8ToGB(x);
	}
	for (int i=0;i<len;)
	{
		if ((int)a[i] >= 0)
		{
			i++;
		}
		else
		{
			Chinese[-x[i]][-x[i+1]]++;
			i += 2;
		}
	}
}

void output()
{
	cout << "正在输出结果..." << endl << endl;
	ofstream OutFile("Output.txt");

	/* B0A1 - FEA0 */
	for (int i=0;i<=MAXN;i++)
	{
		for (int j=0;j<=MAXN;j++)
		{
			if ((i * 100 + j <= 290) || (i * 100 + j >= 8716))
			{
				continue;
			}
			if (Chinese[i][j])
			{
				OutFile << (char)-i << (char)-j << " : " << Chinese[i][j] << " 次" << endl;
			}
		}
	}
}

int main(void)
{
	char *s = read();
	find(s);
	output();
	cout << "**********************完成**********************" << endl;
	cout << "*                                              *" << endl;
	cout << "*                检索工作已完成                *" << endl;
	cout << "*                                              *" << endl;
	cout << "*             请在output中查询结果             *" << endl;
	cout << "*                                              *" << endl;
	cout << "************************************************" << endl << endl;
}

