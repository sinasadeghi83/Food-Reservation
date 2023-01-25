#include "DbManager.h"
#include "Migration.h"
#include "../sinastd/Date.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include <sqlite3.h>
#include <string.h>
#include "../sinastd/Parser.h"

#define MAX_N 100

static int isIntialised = 0;
static int migCount = 0;
static int nameMigCount = 0;

struct Migration
{
    int id;
    char *name;
    Date *createdAt;
};

// Creates the migration table
void createMigrationTable()
{
    // create the migration table
    char *sql = "CREATE TABLE IF NOT EXISTS migration ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "name TEXT NOT NULL,"
                "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
                ");";
    char *errorMsg = 0;
    sqlite3_exec(getMigrationDb(), sql, NULL, NULL, &errorMsg);
    if (errorMsg != 0)
    {
        printf("Error creating migration table: %s", errorMsg);
        sqlite3_free(errorMsg);
    }
    else
    {
        printf("Migration table created successfully\n");
        isIntialised = 1;
    }
}

void initialiseMigration()
{
    // open the migration database
    openMigrationDb();
    // create the migration table
    createMigrationTable();
    // close the migration database
    closeMigrationDb();
}

// Fetching the migrations from the database
static int selectCallback(void *data, int argc, char **argv, char **azColName)
{
    static int migCount = 0;
    Migration **migrations = (Migration **)data;
    Migration *mig = malloc(sizeof(Migration));
    mig->id = atoi(argv[0]);
    mig->name = argv[1];
    int year, month, day;
    sscanf(argv[2], "%d-%d-%d", &year, &month, &day);
    mig->createdAt = CreateDate(year, month, day);
    migrations[migCount++] = mig;
    return 0;
}

// Fetch migration names from ./migations folder (Only works on POSIX systems)
static void fetchFileNames(char *migNames[MAX_N])
{
    DIR *d;
    struct dirent *dir;
    d = opendir("./migrations/ups");
    int i = 0;
    if (d != NULL)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (i < 2)
                i++;
            else
                migNames[nameMigCount++] = dir->d_name;
        }
        closedir(d);
    }
}

bool runMigrations()
{
    // Get the list of migrations from database
    char *sql = "SELECT * FROM migration ORDER BY created_at ASC";
    Migration *migs[MAX_N];
    char *errorMsg = 0;
    int res = sqlite3_exec(getMigrationDb(), sql, selectCallback, (void *)migs, &errorMsg);
    // Print error message if any
    if (res != SQLITE_OK)
    {
        fprintf(stderr, "Error selecting migrations: %s", errorMsg);
        sqlite3_free(errorMsg);
        return false;
    }
    // Get the list of migrations from migrations folder
    char *migNames[MAX_N];
    fetchFileNames(migNames);
    // Run the migrations
    for (int i = 0; i < nameMigCount; i++)
    {
        bool isMigrated = false;
        for (int j = 0; j < migCount; j++)
        {
            if (strcmp(migs[j]->name, migNames[i]) == 0)
            {
                isMigrated = true;
                break;
            }
        }
        if (!isMigrated)
        {
            // Run the migration
            char *migPath = strappend("./migrations/ups/", migNames[i]);
            int year, day, month;
            char *migName = malloc(strlen(migNames[i]));
            strcpy(migName, migNames[i]);
            sscanf(migName, "u%d_%d_%d", &year, &month, &day);
            Date *date = CreateDate(year, month, day);
            char *strDate = DateToString(date);
            FILE *fp = fopen(migPath, "r");
            if (fp == NULL)
            {
                fprintf(stderr, "Error opening migration file[%s]: %s", migName, migPath);
                return false;
            }
            char *sql = malloc(1000);
            fread(sql, 1000, 1, fp);
            fclose(fp);
            char *errorMsg = 0;
            res = sqlite3_exec(getDb(), sql, NULL, NULL, &errorMsg);
            free(sql);
            if (res != SQLITE_OK)
            {
                fprintf(stderr, "Error running migration[%s]: %s", migName, errorMsg);
                sqlite3_free(errorMsg);
                return false;
            }
            // Insert the migration into the database
            char *insertSql = strappend("INSERT INTO migration (name, created_at) VALUES ('", migName);
            strcat(insertSql, "', '");
            strcat(insertSql, strDate);
            strcat(insertSql, "');");
            free(strDate);
            DateFree(date);
            char *errorMsg2 = 0;
            res = sqlite3_exec(getMigrationDb(), insertSql, NULL, NULL, &errorMsg2);
            if (res != SQLITE_OK)
            {
                fprintf(stderr, "Error inserting migration[%s]: %s", migName, errorMsg2);
                sqlite3_free(errorMsg2);
                return false;
            }
            else
            {
                printf("Migration %s ran successfully\n", migName);
            }
        }
    }
    return true;
}

void migrateUp()
{
    // open the migration database
    openMigrationDb();
    // create the migration table if it does not exist
    if (!isIntialised)
    {
        createMigrationTable();
    }
    // run the migrations
    bool res = runMigrations();
    // print in stderr if any error occurs
    if (!res)
    {
        fprintf(stderr, "Error running migrations");
    }
    // close the migration database
    closeMigrationDb();
}