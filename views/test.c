#include "test.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../models/TestCase.h"
#include "../sinastd/Param.h"
#include "../sinastd/Error.h"

#define MAX_STR 1000
#define MAX_TEST_CASES 1000

static FILE *inStream;
static FILE *outStream;
static TestCase *testCases[MAX_TEST_CASES];
static int testCount = 0;

// Parsing test cases from file using TestCase struct
// Checking each test case with its database record using TestCase struct
// Running test cases by calling their actions in controllers
void TestActionMain()
{
    inStream = fopen("files/input.txt", "r");
    outStream = fopen("files/output.txt", "w");
    char line[MAX_STR];
    bool isDone = false;
    memset(line, '\0', MAX_STR);
    fgets(line, MAX_STR, inStream);
    while (!feof(inStream))
    {
        if (line[strlen(line) - 1] == '\n')
        {
            line[strlen(line) - 1] = '\0';
        }
        testCases[testCount++] = CreateTestCaseFromString(line);
        memset(line, '\0', MAX_STR);
        fgets(line, MAX_STR, inStream);
    }
    rewind(inStream);
    for (int i = 0; i < testCount; i++)
    {
        TestActionRun(testCases[i]);
        TestCaseFree(testCases[i]);
    }
}

// TestCase runner
void TestActionRun(TestCase *testCase)
{
    // Validate test case
    Error *err = TestCaseValidate(testCase);
    if (err->isAny)
    {
        fprintf(outStream, "%d#%s\n", testCase->id, err->msg ? err->msg : "Unknown error");
        return;
    }
    ActionLookup *action = testActionLookup;
    while (action->name != NULL)
    {
        if (strcmp(action->name, testCase->title) == 0)
        {
            Error *err = action->action(testCase->params);
            if (err->isAny)
            {
                fprintf(outStream, "%d#%s\n", testCase->id, err->testMsg ? err->testMsg : "Unknown error");
            }
            else
            {
                fprintf(outStream, "%d#%s\n", testCase->id, err->testMsg ? err->testMsg : "success");
            }
            break;
        }
        action++;
    }
}
