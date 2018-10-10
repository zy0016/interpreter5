#ifndef _H_GETINPUT_
#define _H_GETINPUT_

#include "stdio.h"
#include "string.h"
#include "Parameter.h"
#include "cVariableInfo.h"

#pragma once

class CGetInput//读取数据类
{
public:
    CGetInput(cVariableInfo *VariableInfo);
    int ReadStandardInput(void);
    int ReadStandardInput(const char * pfilename);
    
public:
    ~CGetInput(void);
private:
    cVariableInfo * VarInfo;
};

#endif
