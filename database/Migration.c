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
#define MAX_SQL 10000

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
            {
                migNames[nameMigCount] = malloc(strlen(dir->d_name) + 1);
                strcpy(migNames[nameMigCount++], dir->d_name);
            }
        }
        closedir(d);
    }
}

bool runMigrations()
{
    sqlite3 *db = getDb();
    // Get the list of migrations from database
    char *sql = "SELECT * FROM migration ORDER BY created_at DESC";
    Migration *migs[MAX_N];
    memset(migs, '\0', MAX_N * sizeof(Migration *));
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
    memset(migNames, '\0', MAX_N * sizeof(char *));
    fetchFileNames(migNames);
    // Run the migrations
    for (int i = nameMigCount - 1; i >= 0; i--)
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
            sscanf(migNames[i], "u%d_%d_%d", &year, &month, &day);
            Date *date = CreateDate(year, month, day);
            char *strDate = DateToString(date);
            FILE *fp = fopen(migPath, "r");
            if (fp == NULL)
            {
                fprintf(stderr, "Error opening migration file[%s]: %s", migNames[i], migPath);
                return false;
            }
            char *sql = malloc(MAX_SQL);
            memset(sql, '\0', MAX_SQL);
            fread(sql, MAX_SQL, 1, fp);
            fclose(fp);
            sql = realloc(sql, strlen(sql));
            char *errorMsg = 0;
            printf("This is the sql: \n%s\n\n", sql);
            res = sqlite3_exec(db, sql, NULL, NULL, &errorMsg);
            free(sql);
            if (res != SQLITE_OK)
            {
                fprintf(stderr, "Error running migration[%s]: %s", migNames[i], errorMsg);
                sqlite3_free(errorMsg);
                return false;
            }
            // Insert the migration into the database
            char *insertSql = strappend("INSERT INTO migration (name, created_at) VALUES ('", migNames[i]);
            strcat(insertSql, "', '");
            strcat(insertSql, strDate);
            strcat(insertSql, "');");
            free(strDate);
            DateFree(date);
            char *errorMsg2 = 0;
            res = sqlite3_exec(getMigrationDb(), insertSql, NULL, NULL, &errorMsg2);
            free(insertSql);
            if (res != SQLITE_OK)
            {
                fprintf(stderr, "Error inserting migration[%s]: %s", migNames[i], errorMsg2);
                sqlite3_free(errorMsg2);
                return false;
            }
            else
            {
                printf("Migration %s ran successfully\n", migNames[i]);
            }
            free(migNames[i]);
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