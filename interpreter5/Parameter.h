#ifndef _H_PARAMETER_
#define _H_PARAMETER_

#define VARIABLE_MAXLEN 256//������󳤶�
#define DATA_MAXLEN 128//������󳤶�
#define CHARACTER_MAXLEN 1

#define CHARACTER_WELL '#'
#define STRING_WELL "#"

typedef enum
{
    INVALID = 0,//������ֵ
    BINARY  = 2,//��������
    OCTAL   = 8,//�˽�����
    DECIMAL = 10,//ʮ������
    HEX     = 16 //ʮ��������
}NUMBER_TYPE;

NUMBER_TYPE StringIsNumber(const char * pString);
int InCharArray(char ch,const char * pArray,int count);
char * TrimString(char * pStr);
int FileIsExist(const char * filename);//�����ļ��Ƿ�ɶ�
#endif
