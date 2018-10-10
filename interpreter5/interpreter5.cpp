#include "stdio.h"
#include "string.h"

#include "GetInput.h"
#include "cVariableInfo.h"

int main(int argc, char * argv[])
{
	cVariableInfo VarInfo; 
	CGetInput cgi(&VarInfo);
#if 0
    if (argc == 1)//��׼����
    {
        printf("Standard input:\r\n");
        char ch;
        
        if (cgi.ReadStandardInput() == 0)
        {
            printf("Wrong parameters!\r\n");
            return 0;
        }
        printf("Standard output:\r\n");
        VarInfo.OutPutNode();//�������
        printf("Output over!\r\n");
        scanf("%c",&ch);
    }
    else if (argc == 3)//�����в���,��Ϊ�ڶ��������������ļ���,����������Ϊ����ļ���
    {
        if ((strlen(argv[1]) == 0) || (strlen(argv[2]) == 0))//�ļ������Ϸ�
        {
            printf("Wrong parameters number!\r\n");
            return 0;
        }
        if (FileIsExist(argv[1]) == 0)//�����ļ����ɶ�
        {
            printf("Can not open %s.\r\n",argv[1]);
            return 0;
        }
        
        if (cgi.ReadStandardInput(argv[1]) == 0)//argv[1]
        {
            printf("Wrong parameters!\r\n");
            return 0;
        }
        if (VarInfo.OutPutNode(argv[2]) == 1)//�������
            printf("Output is over!\r\n");
        else
            printf("Some error in the file!\r\n");
    }
    else
    {
        printf("Wrong parameters number!\r\n");
    }
#else
    const char * pint = "yufa.txt";
    const char * pout = "out.txt";

    if (cgi.ReadStandardInput(pint) == 0)//argv[1]
    {
        printf("Wrong parameters!\r\n");
        return 0;
    }
    if (VarInfo.OutPutNode(pout) == 1)//�������
        printf("Output is over!\r\n");
    else
        printf("Some error in the file!\r\n");

#endif
    return 0;
}

int FileIsExist(const char * filename)//�����ļ��Ƿ�ɶ�
{
    FILE * p;
    p = fopen(filename,"r");//���ļ�
    if (p == NULL)//�ļ����ɶ�
        return 0;
    fclose(p);
    return 1;
}

NUMBER_TYPE StringIsNumber(const char * p)//�ַ����Ƿ����ת��Ϊ������
{
    int ilen;
    char cNumberD[] = "0123456789",cNumberO[] = "01234567",cNubmerB[] = "01",cNumberH[] = "0123456789abcdefABCDEF";
    char * pNumber = NULL;
    NUMBER_TYPE eNumberType;

    if (p == NULL)
        return INVALID;

    ilen = strlen(p);
    if (ilen == 0)
        return INVALID;

    if (* p == '+' || * p == '-')
    {
        if (ilen == 1)
            return INVALID;
        p++;
    }

    if (* p == '0')
    {
        if ((* (p + 1) == 'x') || (* (p + 1) == 'X'))//���ַ���������16������
        {
            pNumber = cNumberH;
            p += 2;
            eNumberType = HEX;
        }
        else if ((* (p + 1) == 'b') || (* (p + 1) == 'B'))//���ַ���������2������
        {
            pNumber = cNubmerB;
            p += 2;
            eNumberType = BINARY;
        }
        else//���ַ���������8������
        {
            pNumber = cNumberO;
            p++;
            eNumberType = OCTAL;
        }
    }
    else//���ַ���������10������
    {
        pNumber     = cNumberD;
        eNumberType = DECIMAL;
    }

    while (* p)
    {
        if (InCharArray(* p,pNumber,strlen(pNumber)) == -1)
            return INVALID;
        p++;
    }
    return eNumberType;
}

int InCharArray(char ch,const char * pArray,int count)
{
    int iArrayNum,i;
    char * p;

    p         = (char*)pArray;
    iArrayNum = count / sizeof(char);//�������

    for (i = 0;i < iArrayNum;i++)
    {
        if (* p == ch)
            return i;
        p++;
    }
    return -1;
}
/********************************************************************
* Function   TrimString  
* Purpose    ɾ���ַ�����ͷ��β�Ŀո��ַ�
* Params     
* Return     
* Remarks    
**********************************************************************/
char * TrimString(char * pStr)
{
    unsigned char * pBegin = NULL,* pSetp = NULL,* pEnd = NULL;
    unsigned int iLen,i = 0;

    if (pStr == NULL)
        return NULL;

    iLen = strlen(pStr);
    if (0 == iLen)
        return pStr;

    pBegin = (unsigned char*)pStr;
    if (* pBegin == ' ' || * pBegin == '\t')//��ͷ�пո�
    {
        pSetp = (unsigned char*)pStr;
        while ((* pSetp == ' ' || * pSetp == '\t') && (i < iLen))//���ɨ��,���ҷǿո��ַ�
        {
            pSetp++;
            i++;
        }
        if (i == iLen)//�ַ���ȫ�ǿո�
        {
            * pBegin = '\0';
            return pStr;
        }
        //�ҵ����ǿո���ַ�
        while (* pSetp != '\0')
        {
            * pBegin++ = * pSetp;
            * pSetp++ = ' ';
        }
    }

    iLen = strlen(pStr);
    //�����β�Ƿ��пո�
    pEnd = (unsigned char*)pStr + iLen - 1;
    while ((* pEnd == ' ' || * pEnd == '\t') && (iLen-- > 0))
        * pEnd-- = '\0';

    return pStr;
}
