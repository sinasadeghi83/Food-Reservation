#include <sqlite3.h>
#include <stdlib.h>
#include <stdio.h>
#include "DbManager.h"
#include "DbManager.h"

sqlite3 *db, *migrationDb;
static int isopen = 0, migrationIsOpen = 0;

int openDb()
{
    if (isopen == 0)
    {
        isopen = sqlite3_open("database.db", &db);
        // Prints error message if db is not opened
        if (isopen != SQLITE_OK)
        {
            isopen = 0;
            const char *error = sqlite3_errmsg(db);
            printf("Error opening database: %s", error);
        }
        else
        {
            isopen = 1;
        }
        return isopen;
    }
}

int closeDb()
{
    if (isopen)
    {
        int res = sqlite3_close(db);
        // Prints error message if db is not closed, else isopen = 0
        if (res == SQLITE_OK)
        {
            isopen = 0;
        }
        else
        {
            const char *error = sqlite3_errmsg(db);
            printf("Error closing database: %s", error);
        }

        return res;
    }
}

sqlite3 *getDb()
{
    if (isopen == 0)
    {
        openDb();
    }
    // If database is not opened retuens NULL
    if (isopen != 1)
    {
        return NULL;
    }
    return db;
}

int openMigrationDb()
{
    if (migrationIsOpen == 0)
    {
        migrationIsOpen = sqlite3_open("migration.db", &migrationDb);
        // Prints the error message if the database is not opened
        if (migrationIsOpen != SQLITE_OK)
        {
            migrationIsOpen = 0;
            const char *error = sqlite3_errmsg(migrationDb);
            printf("Error opening database: %s", error);
        }
        else
        {
            migrationIsOpen = 1;
        }
        return migrationIsOpen;
    }
}

int closeMigrationDb()
{
    if (migrationIsOpen)
    {
        int res = sqlite3_close(migrationDb);
        if (res == SQLITE_OK)
        {
            migrationIsOpen = 0;
        }
        else
        {
            const char *error = sqlite3_errmsg(migrationDb);
            printf("Error closing database: %s", error);
        }
        return res;
    }
}

sqlite3 *getMigrationDb()
{
    if (migrationIsOpen == 0)
    {
        openMigrationDb();
    }
    if (migrationIsOpen != 1)
    {
        return NULL;
    }
    return migrationDb;
}