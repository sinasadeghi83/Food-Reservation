// include guard for test.h
#ifndef TEST_H
#define TEST_H

#include "../models/TestCase.h"
#include "../sinastd/Param.h"
#include "../sinastd/Error.h"
#include "../controllers/UserController.h"

typedef struct ActionLookup ActionLookup;

struct ActionLookup
{
    char *name;
    Error *(*action)(Param **params);
};

static ActionLookup testActionLookup[] = {
    {
        "login",
        UserActionLogin,
    },
    {"logout", UserActionLogout},
    {"register", UserActionRegister},
    {"change-pass", UserActionChangePass},
    {NULL, NULL}};

void TestActionMain();

void TestActionRun(TestCase *testCase);
#endif