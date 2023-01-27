#include "Error.h"
#include <string.h>
#include <stdlib.h>

// Create Error instance
Error *ErrorCreate(bool isAny, char *msg, char *testMsg)
{
    Error *error = malloc(sizeof(Error));
    ErrorInitialise(error, isAny, msg, testMsg);
    return error;
}

// Error initialiser
void ErrorInitialise(Error *this, bool isAny, char *msg, char *testMsg)
{
    this->isAny = isAny;
    if (msg != NULL)
    {
        this->msg = malloc(strlen(msg) + 1);
        strcpy(this->msg, msg);
    }
    else
    {
        this->msg = NULL;
    }
    if (testMsg != NULL)
    {
        this->testMsg = malloc(strlen(testMsg) + 1);
        strcpy(this->testMsg, testMsg);
    }
    else
    {
        this->testMsg = NULL;
    }
}

// Free the error data
void ErrorFree(Error *this)
{
    if (this->testMsg == NULL)
    {
        return;
    }
    // if (this->msg != NULL)
    // {
    //     // free(this->msg);
    // }
    free(this);
}