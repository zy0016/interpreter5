#include "cVariableInfo.h"

template <class T>
CStack<T>::CStack()
{
    pElementList = NULL;
}

template <class T>
CStack<T>::~CStack()
{
    ReleaseElementList();
}

template <class T>
int CStack<T>::GetElementAmount()//获得链表节点数目
{
    int i = 0;
    tagElement<T> * p = pElementList;
    while (p != NULL)
    {
        p = p->pNext;
        i++;
    }
    return i;
}

template <class T>
int CStack<T>::ElementPush(T ch)//元素入栈
{
    tagElement<T> * p = NULL;

    p = new tagElement<T>;

    p->cOperator = ch;
    p->pNext     = NULL;

    if (pElementList == NULL)
    {
        pElementList = p;
        return 0;
    }
    p->pNext     = pElementList;
    pElementList = p;
    return GetElementAmount();
}

template <class T>
int CStack<T>::ElementPull(T * pch)//元素出栈
{
    tagElement<T> * p = NULL;
    if (pElementList == NULL)
    {
        return 0;
    }
    p = pElementList;
    if (pch != NULL)
        * pch = p->cOperator;

    pElementList = pElementList->pNext;
    delete p;
    return 1;
}

template <class T>
int CStack<T>::ElementGetTop(T * pch)//获得栈顶元素,但是不会导致元素出栈
{
    if (pElementList == NULL)
        return 0;

    if (pch != NULL)
        * pch = pElementList->cOperator;
    return 1;
}

template <class T>
int CStack<T>::ReleaseElementList(void)//释放资源
{
    tagElement<T> * p;
    while (pElementList != NULL)
    {
        p            = pElementList;
        pElementList = pElementList->pNext;
        delete p;
    }
    pElementList = NULL;
    return 1;
}

template <class T>
int CStack<T>::GetOperatorArithmetic(void)//获得元素链表某个元素的数目
{
    int i = 0;
    tagElement<T> * p = pElementList;
    while (p != NULL)
    {
        if (p->cOperator == '+' || p->cOperator == '-' || p->cOperator == '*' || p->cOperator == '/')
            i++;
        p = p->pNext;
    }
    return i;
}
template <class T>
int CStack<T>::GetVariableNumber(void)
{
    return 0;//iVariableNum;
}
//////////////////////////////////////////////////////////////////
cVariableInfo::cVariableInfo(void)
{
    iVariableNum = 0;

    pOutputDataList = NULL;
    pDataList 		= NULL;

    pStackOperatorList = new CStack<char>;
    pStackNumberList   = new CStack<int>;
}

cVariableInfo::~cVariableInfo(void)
{
    DataNode * p;

    while (pDataList != NULL)//释放资源
    {
        p         = pDataList;
        pDataList = pDataList->pNext;
        delete p;
    }
    pDataList = NULL;

    while (pOutputDataList != NULL)//释放资源
    {
        p         		= pOutputDataList;
        pOutputDataList = pOutputDataList->pNext;
        delete p;
    }
    pOutputDataList = NULL;

    pStackOperatorList->ReleaseElementList();
    pStackNumberList->ReleaseElementList();
    delete pStackOperatorList;
    delete pStackNumberList;
}

int cVariableInfo::OutPutNode(const char * pfilename)//输出链表内容到文件或者标准输入输出
{
    FILE * pfile = NULL;
    if (pfilename != NULL)
    {
        pfile = fopen(pfilename,"w+");//生成文件
    }

    DataNode * pCur = NULL, * pOut = NULL;

    pCur = pOutputDataList;
    while (pCur != NULL)
    {
        if (strcmp(pCur->cData,STRING_WELL) == 0)//找到输出标志
        {
            pOut = pOutputDataList;
            while (pOut != pCur)
            {
                if (strcmp(pOut->cData,STRING_WELL) != 0)//节点不是输出标志的话才输出数据
                {
                    if (strlen(pOut->cData) == 0)//仅仅显示数字
                    {
                        if (pfile != NULL)
                            fprintf(pfile,"%d\r\n",pOut->iDataValue);
                        else
                            printf("%d\r\n",pOut->iDataValue);
                    }
                    else//显示变量名和它的值
                    {
                        if (pfile != NULL)
                            fprintf(pfile,"%s = %d\r\n",pOut->cData,pOut->iDataValue);
                        else
                            printf("%s = %d\r\n",pOut->cData,pOut->iDataValue);
                    }
                }
                pOut = pOut->pNext;
            }
        }
        pCur = pCur->pNext;
    }

    if (pfile != NULL)
        fclose(pfile);
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//两个数值的关系运算,该函数返回1表示运算成功,0表示运算失败,result返回运算结果
int cVariableInfo::Expression(int value1,int value2,char coperator,int * result)
{
    switch (coperator)
    {
    case '+':
        * result = value1 + value2;
        return 1;
    case '-':
        * result = value1 - value2;
        return 1;
    case '*':
        * result = value1 * value2;
        return 1;
    case '/':
        if (value2 == 0)
            return 0;
        * result = value1 / value2;
        return 1;
    }
    return 0;
}
/////////////////////////////////////////////////////////
int cVariableInfo::GetOutputDataNodeAmount(void)//获得数据链表节点数目
{
    int i = 0;
    DataNode * p = pOutputDataList;
    while (p != NULL)
    {
        p = p->pNext;
        i++;
    }
    return i;
}
int cVariableInfo::AddOutputNode(const char * pData,int ivalue)//将数据添加到输出链表中去
{
    int iCount = 0;
    DataNode * p = NULL,* pnext = NULL;

    p = new DataNode;
    memset(p,0x00,sizeof(DataNode));//数据清零
    strncpy(p->cData,pData,sizeof(p->cData) - 1);
    p->iDataValue = ivalue;

    if (GetOutputDataNodeAmount() == 0)//加入头节点
    {
        pOutputDataList = p;
        p->pNext  = NULL;
        return 0;
    }
    iCount = 1;
    pnext  = pOutputDataList;
    while (pnext->pNext != NULL)//遍历到链表结尾
    {
        pnext = pnext->pNext;
        iCount++;
    }
    pnext->pNext = p;//将新建的节点添加到链表结尾
    p->pNext     = NULL;
    return iCount;
}
//////////////////////////////////////////////////////////////////////////
int cVariableInfo::GetDataNodeAmount(void)//获得数据链表节点数目
{
    int i = 0;
    DataNode * p = pDataList;
    while (p != NULL)
    {
        p = p->pNext;
        i++;
    }
    return i;
}
int cVariableInfo::GetDataNodeValue(const char * pData,int * pValue)//从数据链表中寻找变量的值,找到之后用pValue返回
{
    DataNode * p = pDataList;
    while (p != NULL)
    {
        if (strcmp(pData,p->cData) == 0)//找到变量名
        {
            if (pValue != NULL)
                * pValue = p->iDataValue;
            return 1;//找到
        }
        p = p->pNext;
    }
    return 0;//没有找到
}
//在链表中查找pData,找到的话将该节点的值更新并返回1,否则返回0
int cVariableInfo::GetAndUpdateValueNode(const char * pData,int ivalue)
{
    DataNode * p = pDataList;
    while (p != NULL)
    {
        if (strcmp(pData,p->cData) == 0)//找到变量名
        {
            p->iDataValue = ivalue;
            return 1;
        }
        p = p->pNext;
    }
    return 0;
}
int cVariableInfo::AddNode(const char * pLineData)//读取每一行数据,将解析好的数据加入链表
{
    int iCount = 0,ivalue = 0;
    DataNode * p = NULL,* pnext = NULL;
    char cVariable[VARIABLE_MAXLEN + 1] = "";

    p = new DataNode;
    memset(p,0x00,sizeof(DataNode));//数据清零

    //需要解析一行的数据，然后将结果放入链表,可能是变量+数字,可能只是数字或者是#
    if (-1 == ExpressionLine(pLineData,cVariable,sizeof(cVariable),&ivalue))
    {
        delete p;
        return -1;//失败
    }
    AddOutputNode(cVariable,ivalue);//将解析结果添加到输出链表当中去
    //首先在链表中查找一下这个变量是否已经存在,如果已经存在的话只需要重新赋值就可以了
    if ((strlen(cVariable) != 0) && (strcmp(cVariable,STRING_WELL) != 0))
    {
        if (GetAndUpdateValueNode(cVariable,ivalue) == 1)
        {
            //这个变量已经定义过了,删除创建的节点,直接返回
            delete p;
            return GetDataNodeAmount();
        }
    }
    strncpy(p->cData,cVariable,sizeof(p->cData) - 1);
    p->iDataValue = ivalue;

    if (GetDataNodeAmount() == 0)//加入头节点
    {
        pDataList = p;
        p->pNext  = NULL;
        return 0;
    }
    iCount = 1;
    pnext  = pDataList;
    while (pnext->pNext != NULL)//遍历到链表结尾
    {
        pnext = pnext->pNext;
        iCount++;
    }
    pnext->pNext = p;//将新建的节点添加到链表结尾
    p->pNext     = NULL;
    return iCount;
}
//////////////////////////////////////////////////////////
OTPERATOR_TYPE cVariableInfo::Precede(char operator1,char operator2)//运算符优先级比较
{
    switch (operator1)
    {
    case '(':
        if (operator2 == ')')
            return OTPERATOR_EQUAL;
		else
			return OTPERATOR_LOW;

    case ')':
        if (operator2 == '(')
            return OTPERATOR_ERROR;
		else
			return OTPERATOR_HIGH;

    case '+':
    case '-':
        if (operator2 == '+' || operator2 == '-')
            return OTPERATOR_HIGH;
        if (operator2 == '*' || operator2 == '/')
            return OTPERATOR_LOW;
        if (operator2 == '(')
            return OTPERATOR_LOW;
        if (operator2 == ')')
            return OTPERATOR_HIGH;

        break;

    case '*':
    case '/':
        if (operator2 == '+' || operator2 == '-' || operator2 == '*' || operator2 == '/')
            return OTPERATOR_HIGH;
        if (operator2 == '(')
            return OTPERATOR_LOW;
        if (operator2 == ')')
            return OTPERATOR_HIGH;

        break;
    }
    return OTPERATOR_ERROR;
}

int cVariableInfo::RemoveNewLine(const char * pLineData,char * pOutLine,int iBufferlen)//删除回车换行符
{
    char * p = (char*)pLineData;
    while (* p)
    {
        if (* p != '\r' && * p != '\n')
            * pOutLine = *p;
        p++;
        pOutLine++;
        iBufferlen--;
        if (iBufferlen <= 1)
            break;
    }
    * pOutLine = '\0';
    return 0;
}
int cVariableInfo::GetBracketNumber(const char * pString,DATA_TYPE eDatatype)//获得左括号或者右括号的个数
{
    int i = 0;
    while (* pString)
    {
        if ((* pString == '(' && eDatatype == DATA_BRACKET_LEFT) || (* pString == ')' && eDatatype == DATA_BRACKET_RIGHT))
            i++;
        pString++;
    }
    return i;
}
//解析一行字符串,pVariable:返回变量名称,可能返回空串,ibufflen:pVariable大小,
//iValue:返回改变量对应的数值
int cVariableInfo::ExpressionLine(const char * pLineData,char * pVariable,int ibufflen,int * iValue)
{
    char cData[VARIABLE_MAXLEN + 1] = "",cVariable[VARIABLE_MAXLEN + 1] = "";
    char ErrorString[VARIABLE_MAXLEN + 1] = "";//出错提示
    char * pLine = (char*)pLineData;
    int iReadlen,iDataNumber,iReadAmount,istep;
    int ivalue;//运算数出栈值
    int iExpressionType = 0;//这一行数据的类型,0:算数表达式，1:赋值语句
    DATA_TYPE eDatatype,old_datatype = DATA_NONE;

    if (NULL == pLine)
        return -1;
    if (strlen(pLine) == 0)//空行,不用解析
        return 1;
    memset(pVariable,0x00,ibufflen);

    iVariableNum    = 0;
    iReadlen        = 0;//单个数据的长度
    iReadAmount     = 0;//读取所有数据的长度
    iDataNumber     = 0;//记录该行表达式数据的个数

    pStackOperatorList->ReleaseElementList();//链表资源复位
    pStackNumberList->ReleaseElementList();//链表资源复位

    if (GetBracketNumber(pLineData,DATA_BRACKET_LEFT) != GetBracketNumber(pLineData,DATA_BRACKET_RIGHT))//括号个数不匹配
        return -1;

    printf("%s\r\n",pLineData);

    while (1)
    {
        memset(cData,0x00,sizeof(cData));
        iReadlen = GetSingleData(pLine + iReadAmount,cData,sizeof(cData),&istep);//获得一个单个数据
        iReadAmount += istep;
        if (iReadlen != 0)
            iDataNumber++;

        eDatatype = GetDataType(cData);//获得数据类型
        if (eDatatype == DATA_NEWLINE)//到达行尾
        {
            if (JudgeNewLine(pLineData) == -1)
                return -1;
            if (pStackNumberList->ElementPull(&ivalue) == 1)//if (NumberPull(&ivalue) == 1)
                * iValue = ivalue;
            break;
        }
        if (eDatatype == DATA_OUTPUTINDICATE)//#
        {
            * pVariable = CHARACTER_WELL;
            break;
        }
        if (eDatatype == DATA_BRACKET_LEFT || eDatatype == DATA_BRACKET_RIGHT || 
            eDatatype == DATA_OPERATOR)//运算符
        {
            if (eDatatype == DATA_OPERATOR && old_datatype == DATA_OPERATOR)//连续两个数据类型都是符号,表示该表达式非法
                return -1;
            if (JudgeOperator(cData[0],pLineData) == -1)
                return -1;
            //continue;
        }
        if (eDatatype == DATA_EQUALMAKR)
        {
            if (ValidEqualmarkPlace(pLineData) != 1)//判断等号的位置是否合法
            {
                RemoveNewLine(pLineData,ErrorString,sizeof(ErrorString));
                printf("The equalmark is incorrect in the \"%s\"!\r\n",ErrorString);
                return -1;
            }
            iExpressionType = 1;
            strncpy(pVariable,cVariable,ibufflen - 1);
            //continue;
        }
        if (eDatatype == DATA_NUMBER)
        {
            if (CountCurrentValue(atol(cData),&ivalue) == -1)
            {
                RemoveNewLine(pLineData,ErrorString,sizeof(ErrorString));
                printf("The string is incorrect in the \"%s\"!\r\n",ErrorString);
                return -1;
            }
            pStackNumberList->ElementPush(ivalue);//NumberPush(ivalue);

            printf("t%d = %d\r\n",iVariableNum++,ivalue);

            //continue;
        }
        if (eDatatype == DATA_VARIABLE)
        {
            if (JudgeVariable(iDataNumber,pLine,cData,iReadAmount,cVariable,sizeof(cVariable),&iExpressionType) == -1)
                return -1;

            //continue;
        }
        old_datatype = eDatatype;
    }
    pStackOperatorList->ReleaseElementList();
    pStackNumberList->ReleaseElementList();
    return 1;
}
int cVariableInfo::JudgeOperator(char copeator_new,const char * pLineData)
{
    char ErrorString[VARIABLE_MAXLEN + 1] = "",copeator_old,coperator,coperator_temp;
    int inumberpullresult1,inumberpullresult2,ivalue1,ivalue2,ivalue;
    OTPERATOR_TYPE oType;
    
    if (copeator_new == '+' || copeator_new == '-')
    {
        if (pStackNumberList->GetElementAmount() == 0)//if (GetNumberAmount() == 0)//数值栈中没有数值,这是个类似于 +5或者-9的情况
        {
            pStackNumberList->ElementPush(0);//NumberPush(0);//数值栈入"0"作为第一个数据

            printf("t%d = %d\r\n",iVariableNum++,0);
        }
        else
        {
            //if (GetOperatorArithmetic() + 1 > GetNumberAmount())//运算符栈大于数值栈,类似于-(+3 * 6)的情况
            if (pStackOperatorList->GetOperatorArithmetic() + 1 > pStackNumberList->GetElementAmount())
            {
                pStackNumberList->ElementPush(0);//NumberPush(0);

                printf("t%d = %d\r\n",iVariableNum++,0);
            }
        }
    }
    else if (copeator_new == '*' || copeator_new == '/')
    {
        if (pStackNumberList->GetElementAmount() == 0)//if (GetNumberAmount() == 0)//数值栈中没有数值,这是个类似于 "*5"或者"/9"的情况,认为表达式非法
        {
            RemoveNewLine(pLineData,ErrorString,sizeof(ErrorString));
            printf("The %c is incorrect in this \"%s\"!\r\n",copeator_new,ErrorString);
            return -1;
        }
    }
    if (pStackOperatorList->ElementGetTop(&copeator_old) == 0)//if (OperatorGetCh(&copeator_old) == 0)//栈为空
    {
        pStackOperatorList->ElementPush(copeator_new);//OperatorPush(copeator_new);
    }
    else//符号栈不为空
    {
        oType = Precede(copeator_old,copeator_new);//运算符比较
        switch (oType)
        {
        case OTPERATOR_HIGH:
            inumberpullresult1 = pStackNumberList->ElementPull(&ivalue1);//NumberPull(&ivalue1);
            inumberpullresult2 = pStackNumberList->ElementPull(&ivalue2);//NumberPull(&ivalue2);
            if (inumberpullresult1 == 0 && inumberpullresult2 == 0)
            {
                ivalue1 = ivalue2 = 0;
                coperator = '+';
            }
            else if (inumberpullresult1 != 0 && inumberpullresult2 == 0)
            {
                if (copeator_old == '+' || copeator_old == '-')
                    ivalue2 = 0;
            }
            if (Expression(ivalue2,ivalue1,copeator_old,&ivalue) == 0)//求值
            {
                RemoveNewLine(pLineData,ErrorString,sizeof(ErrorString));
                printf("Can not compute the \"%s\"!\r\n",ErrorString);
                return -1;
            }

            pStackOperatorList->ElementPull(NULL);//OperatorPull(NULL);//运算符出栈
            if (pStackOperatorList->ElementGetTop(&coperator_temp) == 1)//if (OperatorGetCh(&coperator_temp) == 1)//取得栈顶运算符
            {
                if (coperator_temp == '(' && copeator_new == ')')//如果运算符是"(",再次出栈退出这个"(",因为它已经没有用了
                {
                    pStackOperatorList->ElementPull(NULL);//OperatorPull(NULL);
                    if (CountCurrentValue(ivalue,&ivalue) == -1)
                    {
                        RemoveNewLine(pLineData,ErrorString,sizeof(ErrorString));
                        printf("The equalmark is incorrect in the \"%s\"!\r\n",ErrorString);
                        return -1;
                    }
                }
            }
            pStackNumberList->ElementPush(ivalue);//NumberPush(ivalue);

            printf("t%d = %d\r\n",iVariableNum++,ivalue);

            if (copeator_new == '+' || copeator_new == '-' || copeator_new == '*' || copeator_new == '/')
                pStackOperatorList->ElementPush(copeator_new);//OperatorPush(copeator_new);//新的运算符重新入栈
            break;
        case OTPERATOR_LOW:
            pStackOperatorList->ElementPush(copeator_new);//OperatorPush(copeator_new);//运算符入栈
            break;
        case OTPERATOR_EQUAL://括号出栈
            pStackOperatorList->ElementPull(NULL);//OperatorPull(NULL);
            inumberpullresult1 = pStackNumberList->ElementPull(&ivalue1);//NumberPull(&ivalue1);
            if (inumberpullresult1 == 1)
            {
                if (CountCurrentValue(ivalue1,&ivalue) == -1)
                {
                    RemoveNewLine(pLineData,ErrorString,sizeof(ErrorString));
                    printf("The equalmark is incorrect in the \"%s\"!\r\n",ErrorString);
                    return -1;
                }
            }
            pStackNumberList->ElementPush(ivalue);//NumberPush(ivalue);

            printf("t%d = %d\r\n",iVariableNum++,ivalue);

            break;
        case OTPERATOR_ERROR:
            printf("Can not precede the %c and %c.\r\n",copeator_old,copeator_new);
            return -1;
        }
    }
    return 1;
}

int cVariableInfo::JudgeNewLine(const char * pLineData)
{
    int inumberpullresult1,inumberpullresult2,ivalue1,ivalue2,ivalue;
    char coperator,coperator_temp;
    char ErrorString[VARIABLE_MAXLEN + 1] = "";

    if (pStackOperatorList->GetElementAmount())//while (GetOperatorAmount())//表达式尚未运算完毕
    {
        if (pStackOperatorList->ElementPull(&coperator) == 0)//if (OperatorPull(&coperator) == 0)
            return -1;
        if (pStackOperatorList->ElementGetTop(&coperator_temp) == 1)//if (OperatorGetCh(&coperator_temp) == 1)
        {
            if (coperator_temp == '(')
                pStackOperatorList->ElementPull(NULL);//OperatorPull(NULL);
        }
        inumberpullresult1 = pStackNumberList->ElementPull(&ivalue1);//NumberPull(&ivalue1);
        inumberpullresult2 = pStackNumberList->ElementPull(&ivalue2);//NumberPull(&ivalue2);
        if (inumberpullresult1 == 0 && inumberpullresult2 == 0)
        {
            ivalue1 = ivalue2 = 0;
            coperator = '+';
        }
        else if (inumberpullresult1 != 0 && inumberpullresult2 == 0)
        {
            if (coperator == '+' || coperator == '-')
            {
                ivalue2 = 0;
            }
            else
            {
                RemoveNewLine(pLineData,ErrorString,sizeof(ErrorString));
                printf("The %c is incorrect in the \"%s\"!\r\n",coperator,ErrorString);
                return -1;
            }
        }

        if (Expression(ivalue2,ivalue1,coperator,&ivalue) == 0)
        {
            RemoveNewLine(pLineData,ErrorString,sizeof(ErrorString));
            printf("Can not compute the \"%s\"!\r\n",ErrorString);
            return -1;
        }

        pStackNumberList->ElementPush(ivalue);//NumberPush(ivalue);

        printf("t%d = %d\r\n",iVariableNum++,ivalue);
    }
    return 1;
}
int cVariableInfo::JudgeVariable(int iDataNumber,const char * pLine,const char * pData,int iReadAmount,
                                 char * pVariable,int iVariableBuf,int * iExpressionType)
{
    char * p;
    int ivalue;
    char ErrorString[VARIABLE_MAXLEN + 1] = "";
    if (iDataNumber == 1)//该变量是这条语句的第一个数据部分,需要考察他的下一个字符是否是等号,不是等号的话需要考查这个变量是否曾经赋值
    {
        memset(pVariable,0x00,iVariableBuf);
        strncpy(pVariable,pData,iVariableBuf - 1);
        p = (char*)pLine + iReadAmount;

        while (* p == ' ' || * p == '\t')
            p++;

        if (* p != '=')//不是等号的话需要考查这个变量是否曾经赋值
        {
            if (strstr(p,"=") != 0)//存在"a 3 ="这样的赋值语句
            {
                * iExpressionType = 1;
                return 0;
            }
            if (GetDataNodeValue(pVariable,&ivalue) == 0)//这个变量未曾赋值
            {
                RemoveNewLine(pLine,ErrorString,sizeof(ErrorString));
                printf("Can not find the %s in the \"%s\"!\r\n",pVariable,ErrorString);
                return -1;
            }
            if (CountCurrentValue(ivalue,&ivalue) == -1)//计算数值
            {
                RemoveNewLine(pLine,ErrorString,sizeof(ErrorString));
                printf("The equalmark is incorrect in the \"%s\"!\r\n",ErrorString);
                return -1;
            }
            pStackNumberList->ElementPush(ivalue);//NumberPush(ivalue);

            printf("t%d = %d\r\n",iVariableNum++,ivalue);
        }
        else//下一个字符是等号，这是赋值语句
        {
            * iExpressionType = 1;
        }
    }
    else//该变量不是这条语句的第一个数据部分,需要考查这个变量是否曾经赋值
    {
        if (GetDataNodeValue(pData,&ivalue) == 0)//这个变量未曾赋值
        {
            RemoveNewLine(pLine,ErrorString,sizeof(ErrorString));
            printf("Can not find the %s in the \"%s\"!\r\n",pData,ErrorString);
            return -1;
        }
        if (CountCurrentValue(ivalue,&ivalue) == -1)
        {
            RemoveNewLine(pLine,ErrorString,sizeof(ErrorString));
            printf("The equalmark is incorrect in the \"%s\"!\r\n",ErrorString);
            return -1;
        }
        pStackNumberList->ElementPush(ivalue);//NumberPush(ivalue);

        printf("t%d = %d\r\n",iVariableNum++,ivalue);
    }
    return 1;
}
//当这个数字之前的运算符是乘号或者除号的时候,需要立即进行运算,并将该结果返回,退掉前一个运算符
//返回-1:表示出错,返回0表示运算符栈为空或者数值栈为空,返回1表示这个数字之前的运算符是乘号或者除号
//运算结果正常返回
int cVariableInfo::CountCurrentValue(int icurvalue,int * presult)
{
    char coperator_temp;
    int inumberpullresult2,ivalue2,ivalue;

    if (pStackOperatorList->ElementGetTop(&coperator_temp))//if (OperatorGetCh(&coperator_temp) == 1)
    {
        //前一个运算符是乘或者除,立即进行运算
        if (coperator_temp == '*' || coperator_temp == '/')
        {
            inumberpullresult2 = pStackNumberList->ElementPull(&ivalue2);//NumberPull(&ivalue2);
            if (inumberpullresult2 == 0)
            {
                * presult = icurvalue;
                return 0;
            }
            if (Expression(ivalue2,icurvalue,coperator_temp,&ivalue) == 0)//求值
                return -1;

            pStackOperatorList->ElementPull(NULL);//OperatorPull(NULL);//退掉前一个运算符
            * presult = ivalue;
            return 1;
        }
    }
    * presult = icurvalue;
    return 0;
}

static const char cSeparator[] = {' ','\r','\n','\t','(',')','+','-','*','/','=','#'};

int cVariableInfo::GetSingleData(const char * pLine,char * pBuffer,
                                 int iBufferlen,int * pStep)//获得一个单个数据,返回该数据的长度,pstep返回指针偏移量
{
    char cVariable[VARIABLE_MAXLEN + 1] = "", * p = NULL;
    int i = 0,j = 0;

    if (strlen(pLine) == 0)
        return 0;

    p = (char*)pLine;
    while (* p == ' ' || * p == '\t')//跳过空格
        p++,j++;

    while (* p)
    {
        if (InCharArray(* p,cSeparator,sizeof(cSeparator)) >= 0)
        {
            if (strlen(cVariable) == 0)//头一个字符就是分隔符
            {
                cVariable[i++] = * p;
                j++;
            }
            break;
        }
        cVariable[i++] = * p;
        j++;
        p++;
    }
    if (pBuffer != NULL)
    {
        memset(pBuffer,0x00,iBufferlen);
        strncpy(pBuffer,cVariable,iBufferlen - 1);//返回数据
    }
    * pStep = j;
    return j;
}

DATA_TYPE cVariableInfo::GetDataType(const char * pData)//获得数据类型
{
    if (strlen(pData) == 0)//数据长度为零,认为该行已经结束,返回DATA_NEWLINE告诉调用函数到达行尾
        return DATA_NEWLINE;
    if (StringIsNumber(pData))
        return DATA_NUMBER;//该类型是数字
    if (strlen(pData) == 1)
    {
        switch (* pData)
        {
        case '\r':
        case '\n':
            return DATA_NEWLINE;
        case '(':
            return DATA_BRACKET_LEFT;
        case ')':
            return DATA_BRACKET_RIGHT;
        case '=':
            return DATA_EQUALMAKR;
        case '#':
            return DATA_OUTPUTINDICATE;
        case '+':
        case '-':
        case '*':
        case '/':
            return DATA_OPERATOR;
        }
    }
    return DATA_VARIABLE;//该类型是变量
}

//判断等号出现的位置是否合法
//返回-1表示等号位置非法，0表示没有等号，1等号位置合法
int cVariableInfo::ValidEqualmarkPlace(const char * pLine)
{
    if (strstr(pLine,"=") == NULL)//没有等号
        return 0;

    char cData[VARIABLE_MAXLEN + 1] = "",* p;
    int iReadlen = 0,iReadAmount = 0,istep = 0,iequalnum = 0,iDataNumber;
    DATA_TYPE eDatatype;

    for (int i = 0;i < (int)strlen(pLine);i++)
    {
        if (*(pLine + i) == '=')//计算等号个数
            iequalnum++;
    }
    if (iequalnum > 1)//等号个数大于一,认为该表达式非法
        return -1;

    iDataNumber = 0;
    while(1)
    {
        memset(cData,0x00,sizeof(cData));
        iReadlen = GetSingleData(pLine + iReadAmount,cData,sizeof(cData),&istep);//获得一个单个数据
        iReadAmount += istep;
        if (iReadlen != 0)
            iDataNumber++;

        eDatatype = GetDataType(cData);//获得数据类型
        if (eDatatype == DATA_EQUALMAKR)
        {
            if (iDataNumber == 2)//等号出现在第二数据的位置
                return 1;
            //等号不是出现在第二个数据的位置，扫描等号之后的字符，如果等号之后的字符是空格，\t，或者回车换行，则认为该等号位置合法
            //否则认为等号位置非法
            p = (char*)pLine + iReadAmount;
            while (* p)
            {
                if (* p == ' ' || * p == '\t')
                {
                    p++;
                    continue;
                }
                if (* p == '\r' || * p == '\n')
                    return 1;

                return -1;
            }
        }
        if (eDatatype == DATA_NEWLINE)
            break;
    }
    return -1;
}
