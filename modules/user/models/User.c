#include "modules/user/models/User_internal.h"
#include "libbcrypt/bcrypt.h"
#include <assert.h>

User *CreateUser(String *username, String *password, String *fname, String *lname, String *national_code, Gender gender, UserType type)
{
    User *this = malloc(sizeof(User));
    InitialiseUser(this, username, password, fname, lname, national_code, gender, type);
    return this;
}

// User initialiser
void InitialiseUser(User *this, String *username, String *password, String *fname, String *lname, String *national_code, Gender gender, UserType type)
{
    // Initialise User data according to given parameters
    this->username = username;
    setPassword(this, password);
    this->fname = fname;
    this->lname = lname;
    this->national_code = national_code;
    this->gender = gender;
    this->type = type;
}

// Free the user data
void freeUser(User *this)
{
    // Free all the strings
    free(this->username);
    free(this->fname);
    free(this->lname);
    free(this->national_code);
    // Free the user
    free(this);
}

// Set the password (hashed with bcrypt)
void setPassword(User *this, String *password)
{
    int ret;
    char salt[BCRYPT_HASHSIZE];
    char hash[BCRYPT_HASHSIZE];
    ret = bcrypt_gensalt(12, salt);
    assert(ret == 0);
    ret = bcrypt_hashpw(getStr(password), salt, hash);
    assert(ret == 0);
    this->password = CreateString(hash);
}

// Verify hashed password using bcrypt
bool verifyPassword(User *this, String *password)
{
    int ret;
    ret = bcrypt_checkpw(getStr(password), this->password);
    assert(ret != -1);
    return ret == 0;
}