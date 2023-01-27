// include guard for Error.h
#ifndef ERROR_H
#define ERROR_H

#define ERR_INVALID "invalid"
#define ERR_PERM "permission-denied"
#define ERR_404 "not-found"

#include <stdbool.h>

typedef struct Error Error;

struct Error
{
    bool isAny;
    char *msg;
    char *testMsg;
};

Error *ErrorCreate(bool isAny, char *msg, char *testMsg);
void ErrorInitialise(Error *this, bool isAny, char *msg, char *testMsg);

void ErrorFree(Error *this);
#endif