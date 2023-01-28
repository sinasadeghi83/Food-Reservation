// include guard for test.h
#ifndef TEST_H
#define TEST_H

#include "../models/TestCase.h"
#include "../sinastd/Param.h"
#include "../sinastd/Error.h"
#include "../controllers/UserController.h"
#include "../controllers/SelfController.h"

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
    {"approve", UserActionApprove},
    {"change-student-pass", UserActionChangeStdPass},
    {"remove-student", UserActionRemoveStd},
    {"deactivate", UserActionDeactivate},
    {"define-self", SelfActionAdd},
    {NULL, NULL}};

void TestActionMain();

void TestActionRun(TestCase *testCase);
#endif