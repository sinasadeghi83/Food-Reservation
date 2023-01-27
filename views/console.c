#include "console.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../controllers/UserController.h"
#include "../models/User.h"

// OS specific headers (MacOS X is not supported)
#ifdef __CYGWIN__
// Windows only
#include <conio.h>
#elif defined(__linux__)
// Linux only
#include <termios.h>
#include <unistd.h>
#endif

#define MAX_STR 100

void ConsoleActionMain()
{
    bool isLoggedIn = false;
    while (true)
    {
        if (isLoggedIn)
        {
            isLoggedIn = ConsoleActionLoggedInMenu();
        }
        else
        {
            isLoggedIn = ConsoleActionMenu();
        }
    }
}

// This function shows menu to user
bool ConsoleActionMenu()
{
    printf("Menu:\n");
    printf("1. Login\n");
    printf("2. Register\n");
    printf("3. Exit\n");
    printf("Your choice: ");

    int choice;
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        return ConsoleActionLogin();
        break;
    case 2:
        ConsoleActionRegister();
        break;
    case 3:
        exit(0);
        break;
    default:
        printf("Invalid choice\n");
        break;
    }
    return false;
}

// This function shows login form to user
// Returns status of loggin in
bool ConsoleActionLogin()
{
    printf("Login:\n");
    printf("Username: ");
    char username[MAX_STR];
    scanf("%s", username);
    printf("Password: ");
    char password[MAX_STR];
    // Scan password without showing it
    ConsoleScanPassword(password);
    printf("\n");
    // Login user
    // Create params
    Param **params = (Param **)malloc(sizeof(Param *) * 3);
    params[0] = ParamCreate("user", username);
    params[1] = ParamCreate("password", password);
    params[2] = NULL;
    Error *err = ErrorCreate(false, NULL, NULL);
    if (ParamsValidate(params))
    {
        free(err);
        err = UserActionLogin(params);
    }
    else
    {
        err->isAny = true;
        err->msg = "Invalid inputs!";
    }
    // Free params
    ParamsFree(params);
    // Check if login was successful
    if (err->isAny)
    {
        printf("Error: %s\n", err->msg);
        free(err);
        return false;
    }
    else
    {
        printf("Login successful\n");
        free(err);
        return true;
    }
}

// This function shows register form to user
void ConsoleActionRegister()
{
    // Get user info
    printf("Register:\n");
    printf("Username: ");
    char username[MAX_STR];
    scanf("%s", username);
    printf("Password: ");
    char password[MAX_STR];
    // Scan password without showing it
    ConsoleScanPassword(password);
    printf("\n");
    printf("First name: ");
    char fname[MAX_STR];
    scanf("%s", fname);
    printf("Last name: ");
    char lname[MAX_STR];
    scanf("%s", lname);
    printf("National code: ");
    char national_code[MAX_STR];
    scanf("%s", national_code);
    printf("Birth date: ");
    char birth_date[MAX_STR];
    scanf("%s", birth_date);
    // Get gender by showing two options
    printf("Gender:\n");
    printf("1. Male\n");
    printf("2. Female\n");
    int genderIn;
    char *gender;
    scanf("%d", &genderIn);
    // Handle gender using switch-case
    switch (genderIn)
    {
    case 1:
        gender = "male";
        break;
    case 2:
        gender = "female";
        break;

    default:
        printf("Invalid input!\n");
        return;
    }

    // Get user type by showing two options
    printf("Type:\n");
    printf("1. Admin\n");
    printf("2. Student\n");
    int typeIn;
    char *type;
    scanf("%d", &typeIn);
    switch (typeIn)
    {
    case 1:
        type = "admin";
        break;
    case 2:
        type = "student";
        break;
    default:
        printf("Invalid input!");
        return;
    }
    // Register user
    // Create params
    Param **params = (Param **)malloc(sizeof(Param *) * 9);
    params[0] = ParamCreate("user-id", username);
    params[1] = ParamCreate("password", password);
    params[2] = ParamCreate("name", fname);
    params[3] = ParamCreate("family", lname);
    params[4] = ParamCreate("national-id-code", national_code);
    params[5] = ParamCreate("birthdate", birth_date);
    params[6] = ParamCreate("gender", gender);
    params[7] = ParamCreate("type", type);
    params[8] = NULL;
    // Validate params
    if (!ParamsValidate(params))
    {
        printf("Invalid inputs!\n");
        ParamsFree(params);
        return;
    }
    // Register user
    Error *err = UserActionRegister(params);
    // Free params
    ParamsFree(params);
    // Show error if there's any
    if (err->isAny)
    {
        printf("Error: %s\n", err->msg);
    }
    else
    {
        printf("\nRegister process done succecfully!\n\n");
    }
}

// This function shows logged in menu to user
bool ConsoleActionLoggedInMenu()
{
    printf("Menu:\n");
    printf("1. Logout\n");
    printf("2. Exit\n");
    User *SessionUser = UserGetSessionUser();
    if (SessionUser != NULL && SessionUser->type == USER_ADMIN)
    {
        printf("3. Register another user\n");
    }
    printf("Your choice: ");

    int choice;
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        // Create Params
        Param **params = (Param **)malloc(sizeof(Param *) * 2);
        params[0] = ParamCreate("user", SessionUser->username);
        params[1] = NULL;
        // Logout user
        Error *err = UserActionLogout(params);
        // Free params
        ParamsFree(params);
        if (err->isAny)
        {
            printf("Error: %s\n", err->msg);
        }
        else
        {
            printf("Logout successful\n");
        }
        free(err);
        return UserGetSessionUser() != NULL;
        break;
    case 2:
        // Exit from the program
        exit(0);
        break;
    case 3:
        // Register another user
        ConsoleActionRegister();
        break;
    default:
        printf("Invalid choice\n");
        break;
    }
    return true;
}

// Scan and return password without showing it
void ConsoleScanPassword(char *password)
{
#ifdef __CYGWIN__
    // Windows only
    char ch;
    int i = 0;
    while ((ch = getch()) != '\r')
    {
        if (ch == '\b')
        {
            if (i > 0)
            {
                i--;
                printf("\b \b");
            }
        }
        else
        {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
#elif defined(__linux__)
    // Linux only
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    scanf("%s", password);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
}