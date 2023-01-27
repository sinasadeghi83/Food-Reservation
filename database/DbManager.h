// include guard for DbManager.h
#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <sqlite3.h>
#include <stdbool.h>

int openDb();
int closeDb();
sqlite3 *getDb();
bool DbInsert(const char *table, const char *cols[], const char *values[]);
bool DbSelect(const char *table, const char *whereCols[], const char *whereValues[], int (*callback)(void *, int, char **, char **), void *data);
bool DbRawSelect(const char *table, const char *where, int (*callback)(void *, int, char **, char **), void *data);
bool DbUpdate(const char *table, const char *setCols[], const char *setValues[], const char *whereCols[], const char *whereValues[]);

int openMigrationDb();
int closeMigrationDb();
sqlite3 *getMigrationDb();

#endif