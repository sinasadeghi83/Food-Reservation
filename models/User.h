// include guard for USER_H
#ifndef USER_H
#define USER_H
#include <stdbool.h>
#include "../sinastd/Date.h"
#include "../sinastd/Error.h"
typedef enum UserGender UserGender;
typedef enum UserType UserType;
typedef struct User User;

enum UserGender
{
    USER_MALE,
    USER_FEMALE,
};

enum UserType
{
    USER_ADMIN,
    USER_STUDENT,
};

struct User
{
    int id;
    char *username;
    char *password;
    char *fname;
    char *lname;
    char *national_code;
    Date *birth_date;
    UserGender gender;
    UserType type;
    bool approved;
};

// User initialiser
void UserInitialise(User *this, char *username, char *password, char *fname, char *lname, char *national_code, Date *birth_date, UserGender gender, UserType type, bool approved);
User *UserCreate(char *username, char *password, char *fname, char *lname, char *national_code, Date *birth_date, UserGender gender, UserType type, bool approved);
void UserFree(User *this);
void UserFreeArray(User **users);
void UserFreeFromArray(User **users, int n);
void UserSetPassword(User *this, char *password);
bool UserVerifyPassword(User *this, char *password);
char *UserTypeString(User *this);
char *UserGenderString(User *this);
UserType UserStringToType(char *type);
UserGender UserStringToGender(char *gender);
Error *UserSave(User *this);
Error *UserUpdate(User *this);
User **UserFind(const char *whereCols[], const char *whereValues[]);
#endif