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
User *UserGetSessionUser();
Error *UserActionChangePass(Param **params);

static bool UserIsLoggedIn();

#endif