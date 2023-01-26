// include guard for DbManager.h
#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <sqlite3.h>
#include <stdbool.h>

int openDb();
int closeDb();
sqlite3 *getDb();
bool DbInsert(const char *table, const char *cols[], const char *values[]);

int openMigrationDb();
int closeMigrationDb();
sqlite3 *getMigrationDb();

#endif