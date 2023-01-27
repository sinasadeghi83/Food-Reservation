#include "Param.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Param creator
Param *ParamCreate(char *name, char *value)
{
    Param *param = malloc(sizeof(Param));
    ParamInitialise(param, name, value);
    return param;
}

// Param constructor
void ParamInitialise(Param *this, char *name, char *value)
{
    // Check if name is NULL
    if (name == NULL)
    {
        this->name = NULL;
    }
    else
    {
        this->name = malloc(strlen(name) + 1);
    }
    strcpy(this->name, name);
    // Check if value is NULL
    if (value == NULL)
    {
        this->value = NULL;
    }
    else
    {
        this->value = malloc(strlen(value) + 1);
        strcpy(this->value, value);
    }
}

// Param destructor
void ParamFree(Param *param)
{
    // Check if param is NULL
    if (param == NULL)
    {
        return;
    }
    // Check if param name is NULL
    if (param->name != NULL)
    {
        free(param->name);
    }
    // Check if param value is NULL
    if (param->value == NULL)
    {
        free(param->value);
    }
    free(param);
}

// Params destructor
void ParamsFree(Param **params)
{
    // Check if params is NULL
    if (params == NULL)
    {
        return;
    }
    for (int i = 0; params[i] != NULL; i++)
    {
        ParamFree(params[i]);
    }
    free(params);
}

// Param validator
bool ParamValidate(Param *param)
{
    // Check if param is NULL
    if (param == NULL)
    {
        return false;
    }
    // Check if param name is NULL
    if (param->name == NULL)
    {
        return false;
    }
    // Check if param value is NULL
    if (param->value == NULL)
    {
        return false;
    }
    // Check if param name is empty
    if (strlen(param->name) == 0)
    {
        return false;
    }
    // Check if param value is empty
    if (strlen(param->value) == 0)
    {
        return false;
    }
    // Param is valid
    return true;
}

// Params validator
bool ParamsValidate(Param **params)
{
    // Check if params is NULL
    if (params == NULL)
    {
        return false;
    }
    // Check if params is empty
    if (params[0] == NULL)
    {
        return false;
    }
    // Check if params are valid
    for (int i = 0; params[i] != NULL; i++)
    {
        if (!ParamValidate(params[i]))
        {
            return false;
        }
    }
    // Params are valid
    return true;
}