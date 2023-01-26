#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "UserController.h"
#include "../models/User.h"
#include "../sinastd/Error.h"

Error *UserActionLogin(char *username, char *password)
{
    Error *err = ErrorCreate(false, NULL);
    // Check if user already logged in
    if (UserIsLoggedIn())
    {
        err->isAny = true;
        err->msg = "You are already logged in!";
        return err;
    }
    // Find user
    User **users = UserFind((const char *[]){"username", NULL}, (const char *[]){username, NULL});
    // return error if user not found
    if (users == NULL)
    {
        err->isAny = true;
        err->msg = "User not found!";
        return err;
    }
    User *user = users[0];
    UserFreeFromArray(users, 1);
    // Check if password is correct
    if (!UserVerifyPassword(user, password))
    {
        err->isAny = true;
        err->msg = "Incorrect password!";
        return err;
    }
    // Set session user
    SessionUser = user;
    return err;
}

// This function logs out user
Error *UserActionLogout()
{
    Error *err = ErrorCreate(false, NULL);
    // Check if user is logged in
    if (!UserIsLoggedIn())
    {
        err->isAny = true;
        err->msg = "You are not logged in!";
        return err;
    }
    // Set session user to NULL
    free(SessionUser);
    SessionUser = NULL;
    return err;
}

// This function registers a new user
Error *UserActionRegister(char *username, char *password, char *fname, char *lname, char *national_code, char *birth_date, UserGender gender, UserType type)
{
    // Create error
    Error *err = ErrorCreate(false, NULL);
    // Check if user is logged in
    if (UserIsLoggedIn())
    {
        if (SessionUser->type != USER_ADMIN)
        {
            err->isAny = true;
            err->msg = "You are already logged in!";
            return err;
        }
    }
    // Create user
    User *user = UserCreate(username, password, fname, lname, national_code, CreateDateFromString(birth_date), gender, type);
    // Save user
    err = UserSave(user);
    // Free user
    UserFree(user);
    // Set error message if there is error with no message
    if (err->isAny)
    {
        err->msg = err->msg == NULL ? "Error while saving user!" : err->msg;
    }
    return err;
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