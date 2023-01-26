#include <sqlite3.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "DbManager.h"
#include "DbManager.h"

#define MAX_SQL 10000

sqlite3 *db, *migrationDb;
static int isopen = 0, migrationIsOpen = 0;

int openDb()
{
    if (isopen == 0)
    {
        isopen = sqlite3_open("database.db", &db);
        // Prints the error message if the database is not opened
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

// Insert a row into the database according to the table name, columns and values
bool DbInsert(const char *table, const char *cols[], const char *values[])
{
    openDb();
    if (isopen != 1)
    {
        return false;
    }

    // Creating sql statement
    char sql[MAX_SQL];
    memset(sql, '\0', MAX_SQL);
    sprintf(sql, "INSERT INTO %s (", table);
    int i = 0;
    while (cols[i] != NULL)
    {
        strcat(sql, "'");
        strcat(sql, cols[i]);
        strcat(sql, "'");
        if (cols[i + 1] != NULL)
        {
            strcat(sql, ", ");
        }
        i++;
    }
    strcat(sql, ") VALUES (");
    i = 0;
    while (values[i] != NULL)
    {
        strcat(sql, "'");
        strcat(sql, values[i]);
        strcat(sql, "'");
        if (values[i + 1] != NULL)
        {
            strcat(sql, ", ");
        }
        i++;
    }
    strcat(sql, ");");

    // Executing sql statement
    char *err;
    int res = sqlite3_exec(db, sql, NULL, NULL, &err);
    closeDb();
    if (res != SQLITE_OK)
    {
        fprintf(stderr, "Error inserting into database: %s\nCommand:%s\n", err, sql);
        return false;
    }
    return true;
}