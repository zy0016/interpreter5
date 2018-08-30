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
int CStack<T>::GetElementAmount()//�������ڵ���Ŀ
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
int CStack<T>::ElementPush(T ch)//Ԫ����ջ
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
int CStack<T>::ElementPull(T * pch)//Ԫ�س�ջ
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
int CStack<T>::ElementGetTop(T * pch)//���ջ��Ԫ��,���ǲ��ᵼ��Ԫ�س�ջ
{
    if (pElementList == NULL)
        return 0;

    if (pch != NULL)
        * pch = pElementList->cOperator;
    return 1;
}

template <class T>
int CStack<T>::ReleaseElementList(void)//�ͷ���Դ
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
int CStack<T>::GetOperatorArithmetic(void)//���Ԫ������ĳ��Ԫ�ص���Ŀ
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

    while (pDataList != NULL)//�ͷ���Դ
    {
        p         = pDataList;
        pDataList = pDataList->pNext;
        delete p;
    }
    pDataList = NULL;

    while (pOutputDataList != NULL)//�ͷ���Դ
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

int cVariableInfo::OutPutNode(const char * pfilename)//����������ݵ��ļ����߱�׼�������
{
    FILE * pfile = NULL;
    if (pfilename != NULL)
    {
        pfile = fopen(pfilename,"w+");//�����ļ�
    }

    DataNode * pCur = NULL, * pOut = NULL;

    pCur = pOutputDataList;
    while (pCur != NULL)
    {
        if (strcmp(pCur->cData,STRING_WELL) == 0)//�ҵ������־
        {
            pOut = pOutputDataList;
            while (pOut != pCur)
            {
                if (strcmp(pOut->cData,STRING_WELL) != 0)//�ڵ㲻�������־�Ļ����������
                {
                    if (strlen(pOut->cData) == 0)//������ʾ����
                    {
                        if (pfile != NULL)
                            fprintf(pfile,"%d\r\n",pOut->iDataValue);
                        else
                            printf("%d\r\n",pOut->iDataValue);
                    }
                    else//��ʾ������������ֵ
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
//������ֵ�Ĺ�ϵ����,�ú�������1��ʾ����ɹ�,0��ʾ����ʧ��,result����������
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
int cVariableInfo::GetOutputDataNodeAmount(void)//�����������ڵ���Ŀ
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
int cVariableInfo::AddOutputNode(const char * pData,int ivalue)//��������ӵ����������ȥ
{
    int iCount = 0;
    DataNode * p = NULL,* pnext = NULL;

    p = new DataNode;
    memset(p,0x00,sizeof(DataNode));//��������
    strncpy(p->cData,pData,sizeof(p->cData) - 1);
    p->iDataValue = ivalue;

    if (GetOutputDataNodeAmount() == 0)//����ͷ�ڵ�
    {
        pOutputDataList = p;
        p->pNext  = NULL;
        return 0;
    }
    iCount = 1;
    pnext  = pOutputDataList;
    while (pnext->pNext != NULL)//�����������β
    {
        pnext = pnext->pNext;
        iCount++;
    }
    pnext->pNext = p;//���½��Ľڵ���ӵ������β
    p->pNext     = NULL;
    return iCount;
}
//////////////////////////////////////////////////////////////////////////
int cVariableInfo::GetDataNodeAmount(void)//�����������ڵ���Ŀ
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
int cVariableInfo::GetDataNodeValue(const char * pData,int * pValue)//������������Ѱ�ұ�����ֵ,�ҵ�֮����pValue����
{
    DataNode * p = pDataList;
    while (p != NULL)
    {
        if (strcmp(pData,p->cData) == 0)//�ҵ�������
        {
            if (pValue != NULL)
                * pValue = p->iDataValue;
            return 1;//�ҵ�
        }
        p = p->pNext;
    }
    return 0;//û���ҵ�
}
//�������в���pData,�ҵ��Ļ����ýڵ��ֵ���²�����1,���򷵻�0
int cVariableInfo::GetAndUpdateValueNode(const char * pData,int ivalue)
{
    DataNode * p = pDataList;
    while (p != NULL)
    {
        if (strcmp(pData,p->cData) == 0)//�ҵ�������
        {
            p->iDataValue = ivalue;
            return 1;
        }
        p = p->pNext;
    }
    return 0;
}
int cVariableInfo::AddNode(const char * pLineData)//��ȡÿһ������,�������õ����ݼ�������
{
    int iCount = 0,ivalue = 0;
    DataNode * p = NULL,* pnext = NULL;
    char cVariable[VARIABLE_MAXLEN + 1] = "";

    p = new DataNode;
    memset(p,0x00,sizeof(DataNode));//��������

    //��Ҫ����һ�е����ݣ�Ȼ�󽫽����������,�����Ǳ���+����,����ֻ�����ֻ�����#
    if (-1 == ExpressionLine(pLineData,cVariable,sizeof(cVariable),&ivalue))
    {
        delete p;
        return -1;//ʧ��
    }
    AddOutputNode(cVariable,ivalue);//�����������ӵ����������ȥ
    //�����������в���һ����������Ƿ��Ѿ�����,����Ѿ����ڵĻ�ֻ��Ҫ���¸�ֵ�Ϳ�����
    if ((strlen(cVariable) != 0) && (strcmp(cVariable,STRING_WELL) != 0))
    {
        if (GetAndUpdateValueNode(cVariable,ivalue) == 1)
        {
            //��������Ѿ��������,ɾ�������Ľڵ�,ֱ�ӷ���
            delete p;
            return GetDataNodeAmount();
        }
    }
    strncpy(p->cData,cVariable,sizeof(p->cData) - 1);
    p->iDataValue = ivalue;

    if (GetDataNodeAmount() == 0)//����ͷ�ڵ�
    {
        pDataList = p;
        p->pNext  = NULL;
        return 0;
    }
    iCount = 1;
    pnext  = pDataList;
    while (pnext->pNext != NULL)//�����������β
    {
        pnext = pnext->pNext;
        iCount++;
    }
    pnext->pNext = p;//���½��Ľڵ���ӵ������β
    p->pNext     = NULL;
    return iCount;
}
//////////////////////////////////////////////////////////
OTPERATOR_TYPE cVariableInfo::Precede(char operator1,char operator2)//��������ȼ��Ƚ�
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

int cVariableInfo::RemoveNewLine(const char * pLineData,char * pOutLine,int iBufferlen)//ɾ���س����з�
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
int cVariableInfo::GetBracketNumber(const char * pString,DATA_TYPE eDatatype)//��������Ż��������ŵĸ���
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
//����һ���ַ���,pVariable:���ر�������,���ܷ��ؿմ�,ibufflen:pVariable��С,
//iValue:���ظı�����Ӧ����ֵ
int cVariableInfo::ExpressionLine(const char * pLineData,char * pVariable,int ibufflen,int * iValue)
{
    char cData[VARIABLE_MAXLEN + 1] = "",cVariable[VARIABLE_MAXLEN + 1] = "";
    char ErrorString[VARIABLE_MAXLEN + 1] = "";//������ʾ
    char * pLine = (char*)pLineData;
    int iReadlen,iDataNumber,iReadAmount,istep;
    int ivalue;//��������ջֵ
    int iExpressionType = 0;//��һ�����ݵ�����,0:�������ʽ��1:��ֵ���
    DATA_TYPE eDatatype,old_datatype = DATA_NONE;

    if (NULL == pLine)
        return -1;
    if (strlen(pLine) == 0)//����,���ý���
        return 1;
    memset(pVariable,0x00,ibufflen);

    iVariableNum    = 0;
    iReadlen        = 0;//�������ݵĳ���
    iReadAmount     = 0;//��ȡ�������ݵĳ���
    iDataNumber     = 0;//��¼���б��ʽ���ݵĸ���

    pStackOperatorList->ReleaseElementList();//������Դ��λ
    pStackNumberList->ReleaseElementList();//������Դ��λ

    if (GetBracketNumber(pLineData,DATA_BRACKET_LEFT) != GetBracketNumber(pLineData,DATA_BRACKET_RIGHT))//���Ÿ�����ƥ��
        return -1;

    printf("%s\r\n",pLineData);

    while (1)
    {
        memset(cData,0x00,sizeof(cData));
        iReadlen = GetSingleData(pLine + iReadAmount,cData,sizeof(cData),&istep);//���һ����������
        iReadAmount += istep;
        if (iReadlen != 0)
            iDataNumber++;

        eDatatype = GetDataType(cData);//�����������
        if (eDatatype == DATA_NEWLINE)//������β
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
            eDatatype == DATA_OPERATOR)//�����
        {
            if (eDatatype == DATA_OPERATOR && old_datatype == DATA_OPERATOR)//���������������Ͷ��Ƿ���,��ʾ�ñ��ʽ�Ƿ�
                return -1;
            if (JudgeOperator(cData[0],pLineData) == -1)
                return -1;
            //continue;
        }
        if (eDatatype == DATA_EQUALMAKR)
        {
            if (ValidEqualmarkPlace(pLineData) != 1)//�жϵȺŵ�λ���Ƿ�Ϸ�
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
        if (pStackNumberList->GetElementAmount() == 0)//if (GetNumberAmount() == 0)//��ֵջ��û����ֵ,���Ǹ������� +5����-9�����
        {
            pStackNumberList->ElementPush(0);//NumberPush(0);//��ֵջ��"0"��Ϊ��һ������

            printf("t%d = %d\r\n",iVariableNum++,0);
        }
        else
        {
            //if (GetOperatorArithmetic() + 1 > GetNumberAmount())//�����ջ������ֵջ,������-(+3 * 6)�����
            if (pStackOperatorList->GetOperatorArithmetic() + 1 > pStackNumberList->GetElementAmount())
            {
                pStackNumberList->ElementPush(0);//NumberPush(0);

                printf("t%d = %d\r\n",iVariableNum++,0);
            }
        }
    }
    else if (copeator_new == '*' || copeator_new == '/')
    {
        if (pStackNumberList->GetElementAmount() == 0)//if (GetNumberAmount() == 0)//��ֵջ��û����ֵ,���Ǹ������� "*5"����"/9"�����,��Ϊ���ʽ�Ƿ�
        {
            RemoveNewLine(pLineData,ErrorString,sizeof(ErrorString));
            printf("The %c is incorrect in this \"%s\"!\r\n",copeator_new,ErrorString);
            return -1;
        }
    }
    if (pStackOperatorList->ElementGetTop(&copeator_old) == 0)//if (OperatorGetCh(&copeator_old) == 0)//ջΪ��
    {
        pStackOperatorList->ElementPush(copeator_new);//OperatorPush(copeator_new);
    }
    else//����ջ��Ϊ��
    {
        oType = Precede(copeator_old,copeator_new);//������Ƚ�
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
            if (Expression(ivalue2,ivalue1,copeator_old,&ivalue) == 0)//��ֵ
            {
                RemoveNewLine(pLineData,ErrorString,sizeof(ErrorString));
                printf("Can not compute the \"%s\"!\r\n",ErrorString);
                return -1;
            }

            pStackOperatorList->ElementPull(NULL);//OperatorPull(NULL);//�������ջ
            if (pStackOperatorList->ElementGetTop(&coperator_temp) == 1)//if (OperatorGetCh(&coperator_temp) == 1)//ȡ��ջ�������
            {
                if (coperator_temp == '(' && copeator_new == ')')//����������"(",�ٴγ�ջ�˳����"(",��Ϊ���Ѿ�û������
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
                pStackOperatorList->ElementPush(copeator_new);//OperatorPush(copeator_new);//�µ������������ջ
            break;
        case OTPERATOR_LOW:
            pStackOperatorList->ElementPush(copeator_new);//OperatorPush(copeator_new);//�������ջ
            break;
        case OTPERATOR_EQUAL://���ų�ջ
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

    if (pStackOperatorList->GetElementAmount())//while (GetOperatorAmount())//���ʽ��δ�������
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
    if (iDataNumber == 1)//�ñ������������ĵ�һ�����ݲ���,��Ҫ����������һ���ַ��Ƿ��ǵȺ�,���ǵȺŵĻ���Ҫ������������Ƿ�������ֵ
    {
        memset(pVariable,0x00,iVariableBuf);
        strncpy(pVariable,pData,iVariableBuf - 1);
        p = (char*)pLine + iReadAmount;

        while (* p == ' ' || * p == '\t')
            p++;

        if (* p != '=')//���ǵȺŵĻ���Ҫ������������Ƿ�������ֵ
        {
            if (strstr(p,"=") != 0)//����"a 3 ="�����ĸ�ֵ���
            {
                * iExpressionType = 1;
                return 0;
            }
            if (GetDataNodeValue(pVariable,&ivalue) == 0)//�������δ����ֵ
            {
                RemoveNewLine(pLine,ErrorString,sizeof(ErrorString));
                printf("Can not find the %s in the \"%s\"!\r\n",pVariable,ErrorString);
                return -1;
            }
            if (CountCurrentValue(ivalue,&ivalue) == -1)//������ֵ
            {
                RemoveNewLine(pLine,ErrorString,sizeof(ErrorString));
                printf("The equalmark is incorrect in the \"%s\"!\r\n",ErrorString);
                return -1;
            }
            pStackNumberList->ElementPush(ivalue);//NumberPush(ivalue);

            printf("t%d = %d\r\n",iVariableNum++,ivalue);
        }
        else//��һ���ַ��ǵȺţ����Ǹ�ֵ���
        {
            * iExpressionType = 1;
        }
    }
    else//�ñ��������������ĵ�һ�����ݲ���,��Ҫ������������Ƿ�������ֵ
    {
        if (GetDataNodeValue(pData,&ivalue) == 0)//�������δ����ֵ
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
//���������֮ǰ��������ǳ˺Ż��߳��ŵ�ʱ��,��Ҫ������������,�����ý������,�˵�ǰһ�������
//����-1:��ʾ����,����0��ʾ�����ջΪ�ջ�����ֵջΪ��,����1��ʾ�������֮ǰ��������ǳ˺Ż��߳���
//��������������
int cVariableInfo::CountCurrentValue(int icurvalue,int * presult)
{
    char coperator_temp;
    int inumberpullresult2,ivalue2,ivalue;

    if (pStackOperatorList->ElementGetTop(&coperator_temp))//if (OperatorGetCh(&coperator_temp) == 1)
    {
        //ǰһ��������ǳ˻��߳�,������������
        if (coperator_temp == '*' || coperator_temp == '/')
        {
            inumberpullresult2 = pStackNumberList->ElementPull(&ivalue2);//NumberPull(&ivalue2);
            if (inumberpullresult2 == 0)
            {
                * presult = icurvalue;
                return 0;
            }
            if (Expression(ivalue2,icurvalue,coperator_temp,&ivalue) == 0)//��ֵ
                return -1;

            pStackOperatorList->ElementPull(NULL);//OperatorPull(NULL);//�˵�ǰһ�������
            * presult = ivalue;
            return 1;
        }
    }
    * presult = icurvalue;
    return 0;
}

static const char cSeparator[] = {' ','\r','\n','\t','(',')','+','-','*','/','=','#'};

int cVariableInfo::GetSingleData(const char * pLine,char * pBuffer,
                                 int iBufferlen,int * pStep)//���һ����������,���ظ����ݵĳ���,pstep����ָ��ƫ����
{
    char cVariable[VARIABLE_MAXLEN + 1] = "", * p = NULL;
    int i = 0,j = 0;

    if (strlen(pLine) == 0)
        return 0;

    p = (char*)pLine;
    while (* p == ' ' || * p == '\t')//�����ո�
        p++,j++;

    while (* p)
    {
        if (InCharArray(* p,cSeparator,sizeof(cSeparator)) >= 0)
        {
            if (strlen(cVariable) == 0)//ͷһ���ַ����Ƿָ���
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
        strncpy(pBuffer,cVariable,iBufferlen - 1);//��������
    }
    * pStep = j;
    return j;
}

DATA_TYPE cVariableInfo::GetDataType(const char * pData)//�����������
{
    if (strlen(pData) == 0)//���ݳ���Ϊ��,��Ϊ�����Ѿ�����,����DATA_NEWLINE���ߵ��ú���������β
        return DATA_NEWLINE;
    if (StringIsNumber(pData))
        return DATA_NUMBER;//������������
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
    return DATA_VARIABLE;//�������Ǳ���
}

//�жϵȺų��ֵ�λ���Ƿ�Ϸ�
//����-1��ʾ�Ⱥ�λ�÷Ƿ���0��ʾû�еȺţ�1�Ⱥ�λ�úϷ�
int cVariableInfo::ValidEqualmarkPlace(const char * pLine)
{
    if (strstr(pLine,"=") == NULL)//û�еȺ�
        return 0;

    char cData[VARIABLE_MAXLEN + 1] = "",* p;
    int iReadlen = 0,iReadAmount = 0,istep = 0,iequalnum = 0,iDataNumber;
    DATA_TYPE eDatatype;

    for (int i = 0;i < (int)strlen(pLine);i++)
    {
        if (*(pLine + i) == '=')//����ȺŸ���
            iequalnum++;
    }
    if (iequalnum > 1)//�ȺŸ�������һ,��Ϊ�ñ��ʽ�Ƿ�
        return -1;

    iDataNumber = 0;
    while(1)
    {
        memset(cData,0x00,sizeof(cData));
        iReadlen = GetSingleData(pLine + iReadAmount,cData,sizeof(cData),&istep);//���һ����������
        iReadAmount += istep;
        if (iReadlen != 0)
            iDataNumber++;

        eDatatype = GetDataType(cData);//�����������
        if (eDatatype == DATA_EQUALMAKR)
        {
            if (iDataNumber == 2)//�Ⱥų����ڵڶ����ݵ�λ��
                return 1;
            //�ȺŲ��ǳ����ڵڶ������ݵ�λ�ã�ɨ��Ⱥ�֮����ַ�������Ⱥ�֮����ַ��ǿո�\t�����߻س����У�����Ϊ�õȺ�λ�úϷ�
            //������Ϊ�Ⱥ�λ�÷Ƿ�
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
