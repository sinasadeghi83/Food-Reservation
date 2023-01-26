#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "models/User.h"
#include "sinastd/Date.h"
#include "sinastd/Error.h"
// #include "database/Migration.h"
int main()
{
    // initialiseMigration();
    // migrateUp();
    User *user = UserCreate("admin2", "admin2", "admin2", "admin", "000000000", CreateDateFromString("1970-01-01"), USER_MALE, USER_ADMIN);
    Error *res = UserSave(user);
    if (res->isAny)
    {
        printf("\nThere's an error: %s\n", res->msg);
    }
    else
    {
        printf("\nUser saved successfully\n");
    }
    User **users = UserFind((const char *[]){"username", NULL}, (const char *[]){"admin", NULL});
    printf("%s", users[0]->username);
    return 0;
}