#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "UserController.h"
#include "../models/User.h"
#include "../sinastd/Error.h"
#define USER_MAX_USERS 100

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

// This function changes a user's password
Error *UserActionChangePass(Param **params)
{
    char *username, *oldPass, *newPass = NULL;
    username = oldPass = newPass = NULL;
    // Create error
    Error *err = ErrorCreate(false, NULL, NULL);
    // Get params
    for (int i = 0; params[i] != NULL; i++)
    {
        if (strcmp(params[i]->name, "user") == 0)
        {
            username = params[i]->value;
        }
        else if (strcmp(params[i]->name, "old-pass") == 0)
        {
            oldPass = params[i]->value;
        }
        else if (strcmp(params[i]->name, "new-pass") == 0)
        {
            newPass = params[i]->value;
        }
    }
    // Check if user is logged in
    if (!UserIsLoggedIn())
    {
        // Return error
        err->isAny = true;
        err->msg = "You are not logged in!";
        err->testMsg = ERR_404;
        return err;
    }

    // Check if username is the same as session user
    if (strcmp(username, SessionUser->username) != 0)
    {
        // Return error
        err->isAny = true;
        err->msg = "You are not logged in as this user!";
        err->testMsg = ERR_404;
        return err;
    }

    // Check if old password is correct
    if (!UserVerifyPassword(SessionUser, oldPass))
    {
        // Return error
        err->isAny = true;
        err->msg = "Old password is incorrect!";
        err->testMsg = ERR_PERM;
        return err;
    }

    // Change password
    UserSetPassword(SessionUser, newPass);

    // Check if the password has been set
    if (!UserVerifyPassword(SessionUser, newPass))
    {
        // Return error
        err->isAny = true;
        err->msg = "Error while changing password!";
        err->testMsg = NULL;
        return err;
    }

    // Update user
    err = UserUpdate(SessionUser);
    return err;
}

// This function approves some users by only admin
Error *UserActionApprove(Param **params)
{
    // Create Error
    Error *err = ErrorCreate(false, NULL, NULL);
    char *usernames[USER_MAX_USERS];
    int userCount = 0;
    // Get params
    for (int i = 0; params[i] != NULL; i++)
    {
        if (strcmp(params[i]->name, "user") == 0)
        {
            usernames[userCount] = params[i]->value;
            userCount++;
        }
    }
    // Check if user is logged in and is admin
    if (!UserIsLoggedIn() || SessionUser->type != USER_ADMIN)
    {
        // Return error
        err->isAny = true;
        err->msg = "You are not logged in as admin!";
        err->testMsg = ERR_PERM;
        return err;
    }
    // Approve users
    for (int i = 0; i < userCount; i++)
    {
        // Get users
        User **users = UserFind((const char *[]){"username", NULL}, (const char *[]){usernames[i], NULL});
        // Check if user exists
        if (users == NULL)
        {
            // Return error
            err->isAny = true;
            err->msg = "User does not exist!";
            err->testMsg = ERR_404;
            continue;
        }
        // Get user
        User *user = users[0];
        // Free users
        UserFreeFromArray(users, 1);
        // Approve user
        user->approved = true;
        // Update user
        Error *errUser = UserUpdate(user);
        // Free user
        UserFree(user);
        // Check if there is error
        if (errUser->isAny)
        {
            // Return error
            err->isAny = true;
            err->msg = err->msg == NULL ? "Error while approving user!" : err->msg;
            err->testMsg = err->testMsg == NULL ? ERR_INVALID : err->testMsg;
            return err;
        }
        ErrorFree(errUser);
    }
    return err;
}

// This function deletes some student users by only admin
Error *UserActionRemoveStd(Param **params)
{
    // Create error
    Error *err = ErrorCreate(false, NULL, NULL);
    char *usernames[USER_MAX_USERS];
    int userCount = 0;
    // Get params
    for (int i = 0; params[i] != NULL; i++)
    {
        if (strcmp(params[i]->name, "user") == 0)
        {
            usernames[userCount] = params[i]->value;
            userCount++;
        }
    }
    // Check if user is logged in and is admin
    if (!UserIsLoggedIn() || SessionUser->type != USER_ADMIN)
    {
        // Return error
        err->isAny = true;
        err->msg = "You are not logged in as admin!";
        err->testMsg = ERR_PERM;
        return err;
    }
    // Delete users
    for (int i = 0; i < userCount; i++)
    {
        // Get users
        User **users = UserFind((const char *[]){"username", NULL}, (const char *[]){usernames[i], NULL});
        // Check if user exists
        if (users == NULL)
        {
            // Return error
            err->isAny = true;
            err->msg = "User does not exist!";
            err->testMsg = ERR_404;
            continue;
        }
        // Get user
        User *user = users[0];
        // Free users
        UserFreeFromArray(users, 1);
        // Check if user is admin
        if (user->type == USER_ADMIN)
        {
            // Return error
            err->isAny = true;
            err->msg = "You can not delete an admin!";
            err->testMsg = ERR_404;
            continue;
        }
        // Delete user
        Error *errUser = UserDelete(user);
        // Free user
        UserFree(user);
        // Check if there is error
        if (errUser->isAny)
        {
            // Return error
            err->isAny = true;
            err->msg = err->msg == NULL ? "Error while deleting user!" : err->msg;
            err->testMsg = err->testMsg == NULL ? ERR_INVALID : err->testMsg;
            return err;
        }
        ErrorFree(errUser);
    }
    return err;
}
// This function change a student user password by only admin
Error *UserActionChangeStdPass(Param **params)
{
    // Create error
    Error *err = ErrorCreate(false, NULL, NULL);
    char *username, *newPass = NULL;
    username = newPass = NULL;
    // Get params
    for (int i = 0; params[i] != NULL; i++)
    {
        if (strcmp(params[i]->name, "user") == 0)
        {
            username = params[i]->value;
        }
        else if (strcmp(params[i]->name, "new-pass") == 0)
        {
            newPass = params[i]->value;
        }
    }
    // Check if user is logged in and is admin
    if (!UserIsLoggedIn() || SessionUser->type != USER_ADMIN)
    {
        // Return error
        err->isAny = true;
        err->msg = "You are not logged in as admin!";
        err->testMsg = ERR_PERM;
        return err;
    }
    // Get users
    User **users = UserFind((const char *[]){"username", NULL}, (const char *[]){username, NULL});
    // Check if user exists
    if (users == NULL)
    {
        // Show error
        err->isAny = true;
        err->msg = "User does not exist!";
        err->testMsg = ERR_404;
        return err;
    }
    // Get user
    User *user = users[0];
    // Free users
    UserFreeFromArray(users, 1);
    // Check if user is student
    if (user->type != USER_STUDENT)
    {
        // Show error
        err->isAny = true;
        err->msg = "User is not a student!";
        err->testMsg = ERR_404;
        return err;
    }
    // Change password
    UserSetPassword(user, newPass);
    // Check if the password has been set
    if (!UserVerifyPassword(user, newPass))
    {
        // Return error
        err->isAny = true;
        err->msg = "Error while changing password!";
        err->testMsg = NULL;
        return err;
    }
    // Update user
    err = UserUpdate(user);
    return err;
}

// This function deactivate(Disapprove) some student users by only admin
Error *UserActionDeactivate(Param **params)
{
    // Create error
    Error *err = ErrorCreate(false, NULL, NULL);
    char *usernames[USER_MAX_USERS];
    int userCount = 0;
    // Get params
    for (int i = 0; params[i] != NULL; i++)
    {
        if (strcmp(params[i]->name, "user") == 0)
        {
            usernames[userCount] = params[i]->value;
            userCount++;
        }
    }
    // Check if user is logged in and is admin
    if (!UserIsLoggedIn() || SessionUser->type != USER_ADMIN)
    {
        // Return error
        err->isAny = true;
        err->msg = "You are not logged in as admin!";
        err->testMsg = ERR_PERM;
        return err;
    }
    // Disapprove users
    for (int i = 0; i < userCount; i++)
    {
        // Get users
        User **users = UserFind((const char *[]){"username", NULL}, (const char *[]){usernames[i], NULL});
        // Check if user exists
        if (users == NULL)
        {
            // Return error
            err->isAny = true;
            err->msg = "User does not exist!";
            err->testMsg = ERR_404;
            continue;
        }
        // Get user
        User *user = users[0];
        // Free users
        UserFreeFromArray(users, 1);
        // Check if user is admin
        if (user->type == USER_ADMIN)
        {
            // Return error
            err->isAny = true;
            err->msg = "You can not disapprove an admin!";
            err->testMsg = ERR_404;
            continue;
        }
        // Disapprove user
        user->approved = false;
        // Update user
        Error *errUser = UserUpdate(user);
        // Free user
        UserFree(user);
        // Check if there is error
        if (errUser->isAny)
        {
            // Return error
            err->isAny = true;
            err->msg = err->msg == NULL ? "Error while disapproving user!" : err->msg;
            err->testMsg = err->testMsg == NULL ? ERR_INVALID : err->testMsg;
            return err;
        }
        ErrorFree(errUser);
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