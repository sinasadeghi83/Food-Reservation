// include guard for console.h
#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdbool.h>

void ConsoleActionMain();
bool ConsoleActionMenu();
bool ConsoleActionLogin();
void ConsoleActionRegister();
void ConsoleScanPassword(char *password);
bool ConsoleActionLoggedInMenu();

#endif