#ifndef _H_PARAMETER_
#define _H_PARAMETER_

#define VARIABLE_MAXLEN 256//参数最大长度
#define DATA_MAXLEN 128//参数最大长度
#define CHARACTER_MAXLEN 1

#define CHARACTER_WELL '#'
#define STRING_WELL "#"

typedef enum
{
    INVALID = 0,//不是数值
    BINARY  = 2,//二进制数
    OCTAL   = 8,//八进制数
    DECIMAL = 10,//十进制数
    HEX     = 16 //十六进制数
}NUMBER_TYPE;

NUMBER_TYPE StringIsNumber(const char * pString);
int InCharArray(char ch,const char * pArray,int count);
char * TrimString(char * pStr);
int FileIsExist(const char * filename);//考察文件是否可读
#endif
