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
    char cVariableName[5];
    T cOperator;
    tagElement * pNext;
}/*tag_Operator*/;
//typedef struct tagElement OperatorNode;

template <class T>

class CStack  
{
public:
    int ElementPush(T ch);//Ԫ����ջ
    int ElementPull(T * pch);//Ԫ�س�ջ
    //int ElementPull(T * pch,char * pVariableNumber,int iBufflen);//Ԫ�س�ջ
    int ElementGetTop(T * pch);//���ջ��Ԫ��,���ǲ��ᵼ��Ԫ�س�ջ
    int ReleaseElementList(void);//�ͷ���Դ
    int GetOperatorArithmetic(void);//���Ԫ������ĳ��Ԫ�ص���Ŀ
    int GetElementAmount();//���Ԫ������ڵ���Ŀ
    int GetVariableNumber(void);
	CStack();
	virtual ~CStack();
private:
    tagElement<T> * pElementList;//Ԫ������ָ��
    //int iVariableNum;
};
////////////////////////////////////////////////////////////////////
#if 0
template <class T>
struct tagPoint
{
    T cElement;
    tagPoint * pLeft;
    tagPoint * pRight;
    tagPoint * pParent;
};
template <class T>
class InterpreterTree
{
public:
    InterpreterTree();
    int AddPoint(T ch);
    int AddNumber(int number);
    virtual ~InterpreterTree();
    int ReleaseResource();
private:
    tagPoint<T> * pTreeHead;
    int Order_back(T * p);
};
#endif
////////////////////////////////////////////////////////////////////
typedef enum
{
    DATA_NONE,//��
    DATA_NUMBER,//����
    DATA_VARIABLE,//����
    DATA_BRACKET_LEFT,//����
    DATA_BRACKET_RIGHT,//����
    DATA_NEWLINE,//����
    DATA_OUTPUTINDICATE,//���ǡ�#��
    DATA_EQUALMAKR,//�Ⱥ�
    DATA_OPERATOR,//�����
    DATA_ERROR //����
}DATA_TYPE;

typedef enum
{
    OTPERATOR_HIGH,//��
    OTPERATOR_LOW,//��
    OTPERATOR_EQUAL,//���
    OTPERATOR_ERROR//����
}OTPERATOR_TYPE;//��������ȼ�
typedef struct tagData
{
    char cData[VARIABLE_MAXLEN + 1];//���������
    int iDataValue;//��cDataת����������
    struct tagData * pNext;
}tag_Data;
typedef struct tagData DataNode;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class cVariableInfo//������
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
    DataNode * pDataList;//��������ָ��
    DataNode * pOutputDataList;//��������ָ��
    //////////////////////////////////////////////////////////////////////////
    CStack<char> *pStackOperatorList;
    CStack<int>  *pStackNumberList;
    //////////////////////////////////////////////////////////////////////////
    int GetSingleData(const char * pLine,char * pBuffer,int iBufferlen,int * pStep);
    DATA_TYPE cVariableInfo::GetDataType(const char * pData);
    int ExpressionLine(const char * pLineData,char * pVariable,int ibufflen,int * iValue);
    int GetDataNodeAmount(void);//�������ڵ���Ŀ
    //////////////////////////////////////////////////
    OTPERATOR_TYPE Precede(char operator1,char operator2);
    int Expression(int value1,int value2,char coperator,int * result);

    int GetDataNodeValue(const char * pData,int * pValue);//��������Ѱ�ұ�����ֵ
    int GetAndUpdateValueNode(const char * pData,int ivalue);
    int CountCurrentValue(int icurvalue,int * presult);
    int RemoveNewLine(const char * pLineData,char * pOutLine,int iBufferlen);
    //////////////////////////////////////////////////////////////////////////
    int GetOutputDataNodeAmount(void);//�����������ڵ���Ŀ
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
