#include "Error.h"
#include <string.h>
#include <stdlib.h>

// Create Error instance
Error *ErrorCreate(bool isAny, char *msg)
{
    Error *error = malloc(sizeof(Error));
    ErrorInitialise(error, isAny, msg);
    return error;
}

// Error initialiser
void ErrorInitialise(Error *this, bool isAny, char *msg)
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
}

// Free the error data
void ErrorFree(Error *this)
{
    free(this->msg);
    free(this);
}