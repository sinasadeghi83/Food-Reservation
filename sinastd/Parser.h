// include guard for Parser.h
#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

char *parseInt(int num);
char *strappend(char *dest, char *src);
bool isNumeric(char *str);
bool isAlphaNumeric(char *str);
bool isAlpha(char *str);

#endif