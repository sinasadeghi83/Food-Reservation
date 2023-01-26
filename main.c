#include <stdio.h>
#include <stdlib.h>
#include "models/User.h"
#include "sinastd/Date.h"
// #include "database/Migration.h"
int main()
{
    // initialiseMigration();
    // migrateUp();
    // User *user = UserCreate("admin", "admin", "admin", "admin", "000000000", CreateDateFromString("1970-01-01"), USER_MALE, USER_ADMIN);
    // UserSave(user);
    User **users = UserFind((const char *[]){"username", NULL}, (const char *[]){"admin", NULL});
    printf("%s", users[0]->username);
    return 0;
}