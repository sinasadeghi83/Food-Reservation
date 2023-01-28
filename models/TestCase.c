#include "TestCase.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "../sinastd/Error.h"
#include "../sinastd/Parser.h"
#include "../sinastd/Param.h"
#include "../database/DbManager.h"

#define TEST_CASE_TABLE_NAME "testcase"
#define TEST_CASE_MAX_FETCH 100

static int callbackIndex = 0;

// TestCase columns on Database in an array of strings
char *TEST_CASE_COLS[] = {"id", "title", "valid_params", NULL};

// TestCase creator
TestCase *TestCaseCreate(int id, char *title, Param **params)
{
    TestCase *test_case = malloc(sizeof(TestCase));
    TestCaseInitialise(test_case, id, title, params);
    return test_case;
}

// TestCase constructor
void TestCaseInitialise(TestCase *this, int id, char *title, Param **params)
{
    this->id = id;
    this->title = malloc(strlen(title) + 1);
    strcpy(this->title, title);
    this->params = params;
}

// TestCase destructor
void TestCaseFree(TestCase *test_case)
{
    free(test_case->title);
    ParamsFree(test_case->params);
    free(test_case);
}

// TestCase creator from string
// String sample : "1001#login#user:hossein|password:1234"
TestCase *CreateTestCaseFromString(char *str)
{
    char *token = strtok(str, "#");
    int id = atoi(token);
    token = strtok(NULL, "#");
    char *title = token;
    token = strtok(NULL, "#");
    if (token == NULL)
    {
        return TestCaseCreate(id, title, NULL);
    }
    char params_str[strlen(token) + 1];
    strcpy(params_str, token);
    char *param_token = strtok(params_str, "|");
    int param_count = 0;
    while (param_token != NULL)
    {
        param_count++;
        param_token = strtok(NULL, "|");
    }
    Param **params = malloc(sizeof(Param *) * (param_count + 1));
    char *(*strtok_k)(char *, const char *, char **);
// OS Sepecific strtok_k (Windows and Linux)
#ifdef __CYGWIN__
    strtok_k = strtok_s;
#elif defined(__linux__)
    strtok_k = strtok_r;
#endif
    strcpy(params_str, token);
    char *endParam;
    param_token = strtok_k(params_str, "|", &endParam);
    // param_token = strtok(params_str, "|");
    int i = 0;
    while (param_token != NULL)
    {
        char *endName;
        char *name = strtok_k(param_token, ":", &endName);
        char *value = strtok_k(NULL, ":", &endName);
        params[i] = ParamCreate(name, value);
        i++;
        param_token = strtok_k(NULL, "|", &endParam);
    }
    params[i] = NULL;
    return TestCaseCreate(id, title, params);
}

// Validate test case from database
// Check test case params' names according to valid_params in database and params structs in TestCase using DbManager
// valid_params is a string of params names separated by comma
// Return Error with isAny=true and msg=ERR_INVALID if not found or if params' names are not valid
// Table name : testcase
Error *TestCaseValidate(TestCase *this)
{
    Error *err = ErrorCreate(false, NULL, NULL);
    // Find test case in database
    TestCase **testCases = TestCaseFind(this->title);
    if (testCases == NULL)
    {
        err->isAny = true;
        err->msg = ERR_INVALID;
        err->testMsg = ERR_INVALID;
        return err;
    }
    // Check if params are NULL
    if (this->params == NULL)
    {
        err->isAny = true;
        err->msg = ERR_INVALID;
        err->testMsg = ERR_INVALID;
        return err;
    }
    // Check params' names
    Param **params = testCases[0]->params;
    for (int i = 0; this->params[i] != NULL; i++)
    {
        bool found = false;
        for (int j = 0; params[j] != NULL; j++)
        {
            if (strcmp(this->params[i]->name, params[j]->name) == 0)
            {
                if (this->params[i]->value == NULL)
                {
                    err->isAny = true;
                    err->msg = ERR_INVALID;
                    err->testMsg = ERR_INVALID;
                    return err;
                }
                found = true;
                break;
            }
        }
        if (!found)
        {
            err->isAny = true;
            err->msg = ERR_INVALID;
            err->testMsg = ERR_INVALID;
            return err;
        }
    }
    return err;
}

// Callback function for DbSelect
// Returns zero
int TestCaseCallback(void *data, int argc, char **argv, char **azColName)
{
    TestCase **test_cases = (TestCase **)data;
    int id = atoi(argv[0]);
    char *title = argv[1];
    char *valid_params = argv[2];
    char *param_token = malloc(strlen(argv[2]) + 1);
    strcpy(param_token, argv[2]);
    param_token = strtok(param_token, ",");
    int param_count = 0;
    while (param_token != NULL)
    {
        param_count++;
        param_token = strtok(NULL, ",");
    }
    Param **params = malloc(sizeof(Param *) * (param_count + 1));
    param_token = strtok(valid_params, ",");
    int i = 0;
    while (param_token != NULL)
    {
        params[i] = ParamCreate(param_token, NULL);
        i++;
        param_token = strtok(NULL, ",");
    }
    free(param_token);
    params[i] = NULL;
    test_cases[callbackIndex] = TestCaseCreate(id, title, params);
    callbackIndex++;
    return 0;
}

// Find TestCase from Db
// Return NULL if there is not any
TestCase **TestCaseFind(char *title)
{
    TestCase **test_cases = calloc(TEST_CASE_MAX_FETCH, sizeof(TestCase *));
    test_cases[0] = NULL;
    bool res = DbSelect(TEST_CASE_TABLE_NAME, (const char *[]){"title", NULL}, (const char *[]){title, NULL}, TestCaseCallback, (void *)test_cases);
    callbackIndex = 0;
    if (test_cases[0] == NULL)
    {
        return NULL;
    }
    return test_cases;
}
