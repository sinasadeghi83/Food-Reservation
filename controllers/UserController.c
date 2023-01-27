#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "UserController.h"
#include "../models/User.h"
#include "../sinastd/Error.h"

// Error *UserActionLogin(char *username, char *password)
// {
//     Error *err = ErrorCreate(false, NULL);
//     // Check if user already logged in
//     if (UserIsLoggedIn())
//     {
//         err->isAny = true;
//         err->msg = "You are already logged in!";
//         return err;
//     }
//     // Find user
//     User **users = UserFind((const char *[]){"username", "approved", NULL}, (const char *[]){username, "1", NULL});
//     // return error if user not found
//     if (users == NULL)
//     {
//         err->isAny = true;
//         err->msg = "User not found!";
//         return err;
//     }
//     User *user = users[0];
//     UserFreeFromArray(users, 1);
//     // Check if password is correct
//     if (!UserVerifyPassword(user, password))
//     {
//         err->isAny = true;
//         err->msg = "Incorrect password!";
//         return err;
//     }
//     // Set session user
//     SessionUser = user;
//     return err;
// }

Error *UserActionLogin(Param **params)
{
    Error *err = ErrorCreate(false, NULL, NULL);
    // Get username and password from params
    char *username = NULL;
    char *password = NULL;
    for (int i = 0; params[i] != NULL; i++)
    {
        if (strcmp(params[i]->name, "user") == 0)
        {
            username = params[i]->value;
        }
        else if (strcmp(params[i]->name, "password") == 0)
        {
            password = params[i]->value;
        }
    }
    // Check if username and password are not NULL (Params Validation)
    if (username == NULL || password == NULL)
    {
        err->isAny = true;
        err->msg = "Username and password are required!";
        err->testMsg = ERR_INVALID;
        return err;
    }
    // Check if user already logged in
    if (UserIsLoggedIn())
    {
        err->isAny = true;
        err->msg = "You are already logged in!";
        err->testMsg = ERR_404;
        return err;
    }
    // Find user
    User **users = UserFind((const char *[]){"username", "approved", NULL}, (const char *[]){username, "1", NULL});
    // return error if user not found
    if (users == NULL)
    {
        err->isAny = true;
        err->msg = "User not found!";
        err->testMsg = ERR_404;
        return err;
    }
    User *user = users[0];
    UserFreeFromArray(users, 1);
    // Check if password is correct
    if (!UserVerifyPassword(user, password))
    {
        err->isAny = true;
        err->msg = "Incorrect password!";
        err->testMsg = ERR_404;
        return err;
    }
    // Set session user
    SessionUser = user;
    return err;
}

// This function logs out user
Error *UserActionLogout(Param **params)
{
    // Create error
    Error *err = ErrorCreate(false, NULL, NULL);
    // Get params
    for (int i = 0; params[i] != NULL; i++)
    {
        if (strcmp(params[i]->name, "user") == 0)
        {
            // Check if user is logged in
            if (!UserIsLoggedIn())
            {
                err->isAny = true;
                err->msg = "You are not logged in!";
                err->testMsg = ERR_404;
                return err;
            }
            // Check if user is correct
            if (strcmp(params[i]->value, SessionUser->username) != 0)
            {
                err->isAny = true;
                err->msg = "You are not logged in as this user!";
                err->testMsg = ERR_404;
                return err;
            }
        }
    }
    // Check if user is logged in
    if (!UserIsLoggedIn())
    {
        err->isAny = true;
        err->msg = "You are not logged in!";
        err->testMsg = ERR_PERM;
        return err;
    }
    // Set session user to NULL
    free(SessionUser);
    SessionUser = NULL;
    return err;
}

// This function registers a new user
// Error *UserActionRegister(char *username, char *password, char *fname, char *lname, char *national_code, char *birth_date, UserGender gender, UserType type)
// {
//     // Create error
//     Error *err = ErrorCreate(false, NULL);
//     // Check if user is logged in
//     if (UserIsLoggedIn())
//     {
//         if (SessionUser->type != USER_ADMIN)
//         {
//             err->isAny = true;
//             err->msg = "You are already logged in!";
//             return err;
//         }
//     }
//     // Create user
//     User *user = UserCreate(username, password, fname, lname, national_code, CreateDateFromString(birth_date), gender, type, SessionUser->type == USER_ADMIN);
//     // Save user
//     err = UserSave(user);
//     // Free user
//     UserFree(user);
//     // Set error message if there is error with no message
//     if (err->isAny)
//     {
//         err->msg = err->msg == NULL ? "Error while saving user!" : err->msg;
//     }
//     return err;
// }

Error *UserActionRegister(Param **params)
{
    // Create error
    Error *err = ErrorCreate(false, NULL, NULL);
    char *username = NULL;
    char *password = NULL;
    char *fname = NULL;
    char *lname = NULL;
    char *national_code = NULL;
    char *birth_date = NULL;
    char *genderStr = NULL;
    char *typeStr = NULL;
    UserGender gender;
    UserType type;
    // Get params
    for (int i = 0; params[i] != NULL; i++)
    {
        if (strcmp(params[i]->name, "user-id") == 0)
        {
            username = params[i]->value;
        }
        else if (strcmp(params[i]->name, "password") == 0)
        {
            password = params[i]->value;
        }
        else if (strcmp(params[i]->name, "name") == 0)
        {
            fname = params[i]->value;
        }
        else if (strcmp(params[i]->name, "family") == 0)
        {
            lname = params[i]->value;
        }
        else if (strcmp(params[i]->name, "national-id-code") == 0)
        {
            national_code = params[i]->value;
        }
        else if (strcmp(params[i]->name, "birthdate") == 0)
        {
            birth_date = params[i]->value;
        }
        else if (strcmp(params[i]->name, "gender") == 0)
        {
            genderStr = params[i]->value;
        }
        else if (strcmp(params[i]->name, "type") == 0)
        {
            typeStr = params[i]->value;
        }
    }
    // Cast genderStr and typeStr to UserGender and UserType
    gender = UserStringToGender(genderStr);
    type = UserStringToType(typeStr);
    // Check if user is logged in
    if (UserIsLoggedIn())
    {
        if (SessionUser->type != USER_ADMIN)
        {
            err->isAny = true;
            err->msg = "You are already logged in!";
            err->testMsg = ERR_PERM;
            return err;
        }
    }
    // Create user
    User *user = UserCreate(username, password, fname, lname, national_code, CreateDateFromString(birth_date), gender, type, SessionUser != NULL && SessionUser->type == USER_ADMIN);
    // Save user
    err = UserSave(user);
    // Free user
    UserFree(user);
    // Set error message if there is error with no message
    if (err->isAny)
    {
        err->msg = err->msg == NULL ? "Error while saving user!" : err->msg;
        err->testMsg = err->testMsg == NULL ? ERR_INVALID : err->testMsg;
    }
    return err;
}

// This function returns the current session user
User *UserGetSessionUser()
{
    return SessionUser;
}

// Check if user is logged in
static bool UserIsLoggedIn()
{
    if (SessionUser == NULL)
    {
        return false;
    }
    return true;
}