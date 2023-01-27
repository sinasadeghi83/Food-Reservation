// include guard for TestCase.h
#ifndef TESTCASE_H
#define TESTCASE_H

#include "../sinastd/Param.h"
#include "../sinastd/Error.h"

typedef struct TestCase TestCase;

struct TestCase
{
    int id;
    char *title;
    Param **params;
};

void TestCaseInitialise(TestCase *this, int id, char *title, Param **params);
TestCase *TestCaseCreate(int id, char *title, Param **params);
TestCase *CreateTestCaseFromString(char *str);
void TestCaseFree(TestCase *test_case);
Error *TestCaseValidate(TestCase *this);
TestCase **TestCaseFind(int id, char *title);
int TestCaseCallback(void *data, int argc, char **argv, char **azColName);

#endif