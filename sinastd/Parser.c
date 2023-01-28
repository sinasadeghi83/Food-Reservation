#include "Parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

char *parseInt(int num)
{
    char *str = malloc(10);
    sprintf(str, "%d", num);
    return str;
}

char *strappend(char *dest, char *src)
{
    char *result = malloc(strlen(dest) + strlen(src) + 1);
    strcpy(result, dest);
    strcat(result, src);
    return result;
}

// This function checks if the given string is numeric
bool isNumeric(char *str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}

// This function checks if the given string is alphanumeric
bool isAlphaNumeric(char *str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (!isalnum(str[i]))
        {
            return false;
        }
    }
    return true;
}

// This function checks if the given string is alphabetical
bool isAlpha(char *str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (!isalpha(str[i]))
        {
            return false;
        }
    }
    return true;
}