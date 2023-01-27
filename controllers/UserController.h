// include guard for UserController.h
#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include "../sinastd/Error.h"
#include "../models/User.h"
#include "../sinastd/Param.h"
#include <stdlib.h>

static User *SessionUser = NULL;

Error *UserActionLogin(Param **params);
Error *UserActionLogout(Param **params);
Error *UserActionRegister(Param **params);
Error *UserActionChangePass(Param **params);
Error *UserActionApprove(Param **params);
Error *UserActionChangeStdPass(Param **params);
Error *UserActionRemoveStd(Param **params);
User *UserGetSessionUser();

static bool UserIsLoggedIn();

#endif