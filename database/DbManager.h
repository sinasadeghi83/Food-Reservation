// include guard for DbManager.h
#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <sqlite3.h>

int openDb();
int closeDb();
sqlite3 *getDb();

int openMigrationDb();
int closeMigrationDb();
sqlite3 *getMigrationDb();

#endif