#include <stdio.h>
#include "models/User.h"
#include "sinastd/Date.h"
// #include "database/Migration.h"
int main()
{
    // initialiseMigration();
    // migrateUp();
    User *user = UserCreate("admin", "admin", "admin", "admin", "000000000", CreateDateFromString("1970-01-01"), USER_MALE, USER_ADMIN);
    UserSave(user);
    return 0;
}