#ifndef MODELS_USER_INTERNAL_H
#define MODELS_USER_INTERNAL_H
#include "modules/user/models/User.h"
#include "String.h"

enum Gender
{
    MALE,
    FEMALE,
};

enum UserType
{
    ADMIN,
    STUDENT,
};

struct User
{
    int id;
    String *username;
    String *password;
    String *fname;
    String *lname;
    String *national_code;
    Gender gender;
    UserType type;
};

// User initialiser
void InitialiseUser(User *this, String *username, String *password, String *fname, String *lname, String *national_code, Gender gender, UserType type);

#endif