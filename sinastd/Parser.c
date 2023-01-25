#include "Parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
