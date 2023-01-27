// include guard for Param.h
#ifndef PARAM_H
#define PARAM_H

#include <stdbool.h>

typedef struct Param Param;

struct Param
{
    char *name;
    char *value;
};

void ParamInitialise(Param *this, char *name, char *value);
Param *ParamCreate(char *name, char *value);
void ParamFree(Param *param);
void ParamsFree(Param **params);
bool ParamValidate(Param *param);
bool ParamsValidate(Param **params);
#endif