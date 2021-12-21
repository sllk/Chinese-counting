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

    File.read((char *)&check, sizeof(unsigned char));//��ȡ��һ���ֽڣ�Ȼ������8λ
    int code = check << 8;

    File.read((char *)&check, sizeof(unsigned char));//��ȡ�ڶ����ֽ�
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
	cout << "********************ʹ��˵��********************" << endl;
	cout << "*                                              *" << endl;
	cout << "* 1.�뽫Ҫ�������ı����롰input���ĵ���        *" << endl;
	cout << "*                                              *" << endl;
	cout << "* 2.ȷ�Ϸ���󣬰��»س�                       *" << endl;
	cout << "*                                              *" << endl;
	cout << "************************************************" << endl << endl; 
	while (!KEY_DOWN(13));

	ifstream File("Input.txt");
	while (!File.is_open())
	{
		cout << "�޷��ҵ��ļ�!" << endl << endl;
		cout << "********************ʹ�÷���********************" << endl;
		cout << "*                                              *" << endl;
		cout << "* 1. ����ͬ�ļ����д��� Input.txt �ļ�         *" << endl;
		cout << "*                                              *" << endl;
		cout << "* 2. �����¸��Ƶ� Input.txt �ļ���             *" << endl;
		cout << "*                                              *" << endl;
		cout << "* 3. ���»س�������ִ�г���                    *" << endl;
		cout << "*                                              *" << endl;
		cout << "************************************************" << endl << endl;
		while (!KEY_DOWN(13));

		File.open("Input.txt");
	}
	cout << "���ڶ�ȡ�ļ�..." << endl << endl;
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
	cout << "����������..." << endl << endl;
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
				OutFile << (char)-i << (char)-j << " : " << Chinese[i][j] << " ��" << endl;
			}
		}
	}
}

int main(void)
{
	char *s = read();
	find(s);
	output();
	cout << "**********************���**********************" << endl;
	cout << "*                                              *" << endl;
	cout << "*                �������������                *" << endl;
	cout << "*                                              *" << endl;
	cout << "*             ����output�в�ѯ���             *" << endl;
	cout << "*                                              *" << endl;
	cout << "************************************************" << endl << endl;
}

