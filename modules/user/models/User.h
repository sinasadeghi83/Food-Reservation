typedef enum Gender Gender;
typedef enum UserType UserType;
typedef struct User User;
#include "String.h"

User *CreateUser(String *username, String *password, String *fname, String *lname, String *national_code, Gender gender, UserType type);
void userFree(User *this);
void setPassword(User *this, String *password);
bool verifyPassword(User *this, String *password);
