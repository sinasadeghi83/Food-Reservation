#include "User.h"
// #include <gcrypt.h>
#include "../libbcrypt/bcrypt.h"
#include "../database/DbManager.h"
#include "../sinastd/Date.h"
#include <sqlite3.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define USER_TABLE "user"
#define MAX_USER_FETCH 100

const char *USER_COLS[9] = {
    "username",
    "password",
    "type",
    "fname",
    "lname",
    "national_code",
    "gender",
    "birth_date",
    NULL};

User *UserCreate(char *username, char *password, char *fname, char *lname, char *national_code, Date *birth_date, UserGender gender, UserType type)
{
    User *this = malloc(sizeof(User));
    UserInitialise(this, username, password, fname, lname, national_code, birth_date, gender, type);
    return this;
}

// User initialiser
void UserInitialise(User *this, char *username, char *password, char *fname, char *lname, char *national_code, Date *birth_date, UserGender gender, UserType type)
{
    // Initialise User data according to given parameters
    this->username = malloc(strlen(username) + 1);
    strcpy(this->username, username);
    UserSetPassword(this, password);
    this->fname = malloc(strlen(fname) + 1);
    strcpy(this->fname, fname);
    this->lname = malloc(strlen(lname) + 1);
    strcpy(this->lname, lname);
    this->national_code = malloc(strlen(national_code) + 1);
    strcpy(this->national_code, national_code);
    this->birth_date = malloc(sizeof(Date));
    memcpy(this->birth_date, birth_date, sizeof(Date));
    this->gender = gender;
    this->type = type;
}

// Free the user data
void freeUser(User *this)
{
    // Free all the chars
    free(this->username);
    free(this->password);
    free(this->fname);
    free(this->lname);
    free(this->national_code);
    // Free the user
    free(this);
}

// Set the password (hashed with bcrypt)
void UserSetPassword(User *this, char *password)
{
    int ret;
    char salt[BCRYPT_HASHSIZE];
    char hash[BCRYPT_HASHSIZE];
    ret = bcrypt_gensalt(12, salt);
    assert(ret == 0);
    ret = bcrypt_hashpw(password, salt, hash);
    assert(ret == 0);
    this->password = malloc(strlen(hash));
    strcpy(this->password, hash);
}

// Verify hashed password using bcrypt
bool UserVerifyPassword(User *this, char *password)
{
    int ret;
    ret = bcrypt_checkpw(password, this->password);
    assert(ret != -1);
    return ret == 0;
}

// Returns user type as string
char *UserTypeString(User *this)
{
    switch (this->type)
    {
    case USER_ADMIN:
        return "admin";

    case USER_STUDENT:
        return "student";
    }
}

// Returns user gender as string
char *UserGenderString(User *this)
{
    switch (this->gender)
    {
    case USER_MALE:
        return "male";

    case USER_FEMALE:
        return "female";
    }
}

// Save the user to the database using DbManager
bool UserSave(User *this)
{
    const char *values[] = {
        this->username,
        this->password,
        UserTypeString(this),
        this->fname,
        this->lname,
        this->national_code,
        UserGenderString(this),
        DateToString(this->birth_date),
        NULL};
    return DbInsert(USER_TABLE, USER_COLS, values);
}

// Callback function for DbSelect
// This function will fill in users array with the result of the query
int UserSelectCallback(void *data, int argc, char **argv, char **azColName)
{
    static int index = 0;
    User **users = (User **)(data);
    UserGender gender;
    if (argv[6] == "female")
    {
        gender = USER_FEMALE;
    }
    else
    {
        gender = USER_MALE;
    }

    UserType type;
    if (argv[2] == "admin")
    {
        type = USER_ADMIN;
    }
    else
    {
        type = USER_STUDENT;
    }
    users[index++] = UserCreate(argv[0], argv[1], argv[3], argv[4], argv[5], CreateDateFromString(argv[7]), gender, type);
    return 0;
}

User **UserFind(const char *whereCols[], const char *whereValues[])
{
    User **users = calloc(MAX_USER_FETCH, sizeof(User *)); // database result
    int i = 0;
    bool status = DbSelect(USER_TABLE, whereCols, whereValues, UserSelectCallback, (void *)users);
    return status ? users : NULL;
}

// // Set the password without encyrption
// void UserSetPassword(User *this, char *password)
// {
//     this->password = malloc(strlen(password) + 1);
//     strcpy(this->password, password);
// }

// // Verify password without encyrption
// bool UserVerifyPassword(User *this, char *password)
// {
//     return strcmp(this->password, password) == 0;
// }

// bool saveUser(User *this)
// {
// }

// // Set the password using gcrypt
// void UserSetPassword(User *this, char *password)
// {
//     // Hash the password using SHA512
//     gcry_md_hd_t hd;
//     gcry_error_t err;
//     err = gcry_md_open(&hd, GCRY_MD_SHA512, 0);
//     assert(!err);
//     gcry_md_write(hd, password, strlen(password));
//     char *hash = gcry_md_read(hd, GCRY_MD_SHA512);
//     assert(hash);
//     // Set the password
//     this->password = malloc(gcry_md_get_algo_dlen(GCRY_MD_SHA512));
//     memcpy(this->password, hash, gcry_md_get_algo_dlen(GCRY_MD_SHA512));
//     gcry_md_close(hd);
// }

// // Verify hashed password using gcrypt
// bool UserVerifyPassword(User *this, char *password)
// {
//     // Hash the password using SHA512
//     gcry_md_hd_t hd;
//     gcry_error_t err;
//     err = gcry_md_open(&hd, GCRY_MD_SHA512, 0);
//     assert(!err);
//     gcry_md_write(hd, password, strlen(password));
//     char *hash = gcry_md_read(hd, GCRY_MD_SHA512);
//     assert(hash);
//     // Verify the password
//     bool ret = memcmp(hash, this->password, gcry_md_get_algo_dlen(GCRY_MD_SHA512)) == 0;
//     gcry_md_close(hd);
//     return ret;
// }