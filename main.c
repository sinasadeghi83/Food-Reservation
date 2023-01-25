#include <stdio.h>
#include "database/Migration.h"

int main()
{
    initialiseMigration();
    migrateUp();
    return 0;
}