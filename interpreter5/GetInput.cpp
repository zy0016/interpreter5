#include "GetInput.h"

CGetInput::CGetInput(cVariableInfo *VariableInfo)//standard input
{
    VarInfo = VariableInfo;
}

CGetInput::~CGetInput(void)
{
}

int CGetInput::ReadStandardInput(void)//从标准输入输出读取数据
{
    char cVariable[VARIABLE_MAXLEN + 1] = "",ch;
    int i = 0;

    while (1)
    {
		i = 0;
        memset(cVariable,0x00,sizeof(cVariable));
        ch = getchar();
        if ((ch == '\n') || (ch == '\r'))//输入结束
            break;

        while (1)
        {
            if (ch == '\r' || ch == '\n')//一行结束
            {
                cVariable[i++] = ch;
                break;
            }
            cVariable[i++] = ch;
            ch = getchar();
        }
        TrimString(cVariable);
        if ((strlen(cVariable) == 0) ||
            (strlen(cVariable) == 1 && (cVariable[0] == '\r' || cVariable[0] == '\n')))
        {
            continue;
        }
        if (VarInfo->AddNode(cVariable) == -1)
            return 0;
    }
    return 1;
}

int CGetInput::ReadStandardInput(const char * pfilename)//从文件中读取数据
{
    FILE * pfile;
    char cVariable[VARIABLE_MAXLEN + 1] = "",ch;
    int i;

    pfile = fopen(pfilename,"r");
    if (pfile == NULL)
        return 0;

    i = 0;
    while (!feof(pfile))
    {
        fread(&ch,sizeof(ch),1,pfile);
        if (ch == '\r' || ch == '\n')
        {
            cVariable[i++] = ch;
            TrimString(cVariable);
            if (strlen(cVariable) == 1)
            {
                i = 0;
                memset(cVariable,0x00,sizeof(cVariable));
                continue;
            }
            if (VarInfo->AddNode(cVariable) == -1)
                return 0;
            i = 0;
            memset(cVariable,0x00,sizeof(cVariable));
        }
        else
        {
            cVariable[i++] = ch;
        }
    }
    fclose(pfile);
    return 1;
}
