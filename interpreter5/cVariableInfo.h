#ifndef _H_CVARIABLEINFO_
#define _H_CVARIABLEINFO_

#pragma once

#include "Parameter.h"
#include "string.h"
#include "stdlib.h"
#include <stdio.h>

/////////////////////////////////////////////////////////////////////
template <class T>
struct tagElement
{
    T cOperator;
    tagElement * pNext;
};

template <class T>
class CStack  
{
public:
    int ElementPush(T ch);//元素入栈
    int ElementPull(T * pch);//元素出栈
    int ElementGetTop(T * pch);//获得栈顶元素,但是不会导致元素出栈
    int ReleaseElementList(void);//释放资源
    int GetOperatorArithmetic(void);//获得元素链表某个元素的数目
    int GetElementAmount();//获得元素链表节点数目
    int GetVariableNumber(void);
	CStack();
	virtual ~CStack();
private:
    tagElement<T> * pElementList;//元素链表指针
};
////////////////////////////////////////////////////////////////////
typedef enum
{
    DATA_NONE,//无
    DATA_NUMBER,//数字
    DATA_VARIABLE,//变量
    DATA_BRACKET_LEFT,//括号
    DATA_BRACKET_RIGHT,//括号
    DATA_NEWLINE,//换行
    DATA_OUTPUTINDICATE,//就是“#”
    DATA_EQUALMAKR,//等号
    DATA_OPERATOR,//运算符
    DATA_ERROR //出错
}DATA_TYPE;

typedef enum
{
    OTPERATOR_HIGH,//高
    OTPERATOR_LOW,//低
    OTPERATOR_EQUAL,//相等
    OTPERATOR_ERROR//出错
}OTPERATOR_TYPE;//运算符优先级
typedef struct tagData
{
    char cData[VARIABLE_MAXLEN + 1];//保存参数名
    int iDataValue;//将cData转换成整型数
    struct tagData * pNext;
}tag_Data;
typedef struct tagData DataNode;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class cVariableInfo//参数类
{
public:
    cVariableInfo(void);
    int AddNode(const char * pLineData);
    int OutPutNode(const char * pfilename = NULL);
public:
    ~cVariableInfo(void);

private:
    int iVariableNum;
    //////////////////////////
    DataNode * pDataList;//数据链表指针
    DataNode * pOutputDataList;//数据链表指针
    //////////////////////////////////////////////////////////////////////////
    CStack<char> *pStackOperatorList;
    CStack<int>  *pStackNumberList;
    //////////////////////////////////////////////////////////////////////////
    int GetSingleData(const char * pLine,char * pBuffer,int iBufferlen,int * pStep);
    DATA_TYPE cVariableInfo::GetDataType(const char * pData);
    int ExpressionLine(const char * pLineData,char * pVariable,int ibufflen,int * iValue);
    int GetDataNodeAmount(void);//获得链表节点数目
    //////////////////////////////////////////////////
    OTPERATOR_TYPE Precede(char operator1,char operator2);
    int Expression(int value1,int value2,char coperator,int * result);

    int GetDataNodeValue(const char * pData,int * pValue);//从链表中寻找变量的值
    int GetAndUpdateValueNode(const char * pData,int ivalue);
    int CountCurrentValue(int icurvalue,int * presult);
    int RemoveNewLine(const char * pLineData,char * pOutLine,int iBufferlen);
    //////////////////////////////////////////////////////////////////////////
    int AddOutputNode(const char * pData,int ivalue);
    int ValidEqualmarkPlace(const char * pLine);
    ///////////////////////////////////
    int JudgeVariable(int iDataNumber,const char * pLine,const char * pData,
        int iReadAmount,char * pVariable,int iVariableBuf,int * iExpressionType);
    int JudgeNewLine(const char * pLineData);
    int JudgeOperator(char copeator_new,const char * pLineData);
    int GetBracketNumber(const char * pString,DATA_TYPE eDatatype);
};

#endif
