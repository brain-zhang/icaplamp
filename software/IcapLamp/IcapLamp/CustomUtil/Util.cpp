#include "afx.h"
#include "tchar.h"

#include "Util.h"

string CUtil::EncodeUtf8(wstring in)
{
	string s(in.length() * 3, ' ');  
	size_t len = ::WideCharToMultiByte(CP_UTF8, 0, in.c_str(), in.length(), &s[0], s.length(), NULL, NULL);  
	s.resize(len);  
	return s;
}

wstring CUtil::DecodeUtf8(string in)  
{  
	wstring s(in.length(), _T(' '));
	size_t len = ::MultiByteToWideChar(CP_UTF8, 0, in.c_str(), in.length(), &s[0],s.length());  
	s.resize(len);  
	return s;  
}

string CUtil::URLEncode(const string &sIn)
{
	string sOut;
	for( size_t ix = 0; ix < sIn.size(); ix++ )
	{      
		unsigned char buf[4];
		memset( buf, 0, 4 );
		if( isalnum( (unsigned char)sIn[ix] ) )
		{      
			buf[0] = sIn[ix];
		}
		//else if ( isspace( (unsigned char)sIn[ix] ) ) //貌似把空格编码成%20或者+都可以
		//{
		//    buf[0] = '+';
		//}
		else
		{
			buf[0] = '%';
			buf[1] = toHex( (unsigned char)sIn[ix] >> 4 );
			buf[2] = toHex( (unsigned char)sIn[ix] % 16);
		}
		sOut += (char *)buf;
	}
	return sOut;
};

string CUtil::URLDecode(const string &sIn)
{
	string sOut;
	for( size_t ix = 0; ix < sIn.size(); ix++ )
	{
		unsigned char ch = 0;
		if(sIn[ix]=='%')
		{
			ch = (fromHex(sIn[ix+1])<<4);
			ch |= fromHex(sIn[ix+2]);
			ix += 2;
		}
		else if(sIn[ix] == '+')
		{
			ch = ' ';
		}
		else
		{
			ch = sIn[ix];
		}
		sOut += (char)ch;
	}
	return sOut;
}


//unicode to ascii
void CUtil::GetAnsiString(const CString &s, char dest[], size_t total)
{
	size_t nSize = s.GetLength();
	ASSERT(nSize < total);
	wcstombs(dest, s, nSize+1);
}

//输入参数 unicode80编码的CString
//输出参数 gb2312编码的字符串
void  CUtil::GetGB2312String(CString &strInUnicodeStr, char strOutGb2312Str[], size_t size)
{
	ASSERT(size > strInUnicodeStr.GetLength());
	UINT nCodePage = 936; //GB2312 
	int nLength=WideCharToMultiByte(nCodePage, 0, (LPCWSTR)strInUnicodeStr.GetBuffer(), -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(nCodePage, 0, (LPCWSTR)strInUnicodeStr.GetBuffer(), -1, strOutGb2312Str, nLength, NULL, NULL);
	strInUnicodeStr.ReleaseBuffer();
	/*
	for(pU=(wchar_t *) (LPCTSTR)strInUnicodeStr; *pU; ++pU)
	{
		::WideCharToMultiByte(CP_ACP, NULL, pU, 1, wout, 2, NULL, NULL);
		strOutGb2312Str[strlen(strOutGb2312Str)] = wout[0];
		strOutGb2312Str[strlen(strOutGb2312Str)] = wout[1];
	}
	*/
}

//获取当前程序路径+文件名
void CUtil::GetCurrentExecPath(char * fileName,char* filePath)
{
	ASSERT(fileName != NULL);
	char path[1024];
	memset(path, 0, sizeof(path));
	::GetModuleFileNameA(NULL,path,sizeof(path));

	int strLenth=strlen(path);
	int i = 1;	//从字符串后面开始遍历，直到遇到'\'
	while(path[strLenth-i] != '\\')
	{
		path[strLenth-i]='\0';
		i++;
	}

	strcpy(filePath,path);
	strcat(filePath,fileName);
}

//去除字符串左右空白
std::string CUtil::trim(const std::string& str)
{
	std::string::size_type pos = str.find_first_not_of(' ');
	if (pos == std::string::npos)
	{
		return str;
	}
	std::string::size_type pos2 = str.find_last_not_of(' ');
	if (pos2 != std::string::npos)
	{
		return str.substr(pos, pos2 - pos + 1);
	}
	return str.substr(pos);
}

//根据指定字符分割
int CUtil::split(const std::string& str, std::vector<std::string>& ret_, std::string sep)
{
	if (str.empty())
	{
		return 0;
	}

	string tmp;
	string::size_type pos_begin = str.find_first_not_of(sep);
	string::size_type comma_pos = 0;

	while (pos_begin != string::npos)
	{
		comma_pos = str.find(sep, pos_begin);
		if (comma_pos != string::npos)
		{
			tmp = str.substr(pos_begin, comma_pos - pos_begin);
			pos_begin = comma_pos + sep.length();
		}
		else
		{
			tmp = str.substr(pos_begin);
			pos_begin = comma_pos;
		}

		if (!tmp.empty())
		{
			ret_.push_back(tmp);
			tmp.clear();
		}
	}
	return 0;
}

//用指定字符替换
std::string CUtil::replace(const std::string& str, const std::string& src, const std::string& dest)
{
	std::string ret;

	std::string::size_type pos_begin = 0;
	std::string::size_type pos = str.find(src);
	while (pos != std::string::npos)
	{
		//cout <<"replacexxx:" << pos_begin <<" " << pos <<"\n";
		ret.append(str.data() + pos_begin, pos - pos_begin);
		ret += dest;
		pos_begin = pos + 1;
		pos = str.find(src, pos_begin);
	}
	if (pos_begin < str.length())
	{
		ret.append(str.begin() + pos_begin, str.end());
	}
	return ret;
}

//计算CRC值
unsigned char CUtil::CRCCacu(const unsigned char *ptr,  unsigned int size)
{
	unsigned char cdt = 0, i = 0;

	for (i = 0; i < size; i++)
	{
		cdt = CDT_BCH[cdt ^ *(ptr+i)];
	}
	return (~cdt);
}