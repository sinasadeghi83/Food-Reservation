// include guard for UserController.h
#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include "../sinastd/Error.h"
#include "../models/User.h"
#include <stdlib.h>

static User *SessionUser = NULL;

Error *UserActionLogin(char *username, char *password);
Error *UserActionLogout();
Error *UserActionRegister(char *username, char *password, char *fname, char *lname, char *national_code, char *birth_date, UserGender gender, UserType type);
User *UserGetSessionUser();

static bool UserIsLoggedIn();

#endif