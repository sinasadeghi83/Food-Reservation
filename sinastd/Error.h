// include guard for Error.h
#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>

typedef struct Error Error;

struct Error
{
    bool isAny;
    char *msg;
};

Error *ErrorCreate(bool isAny, char *msg);
void ErrorInitialise(Error *this, bool isAny, char *msg);

void ErrorFree(Error *this);
#endif