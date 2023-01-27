#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "models/User.h"
#include "sinastd/Date.h"
#include "sinastd/Error.h"
#include "views/console.h"
#include "views/test.h"
// #include "database/Migration.h"
int main()
{
    // initialiseMigration();
    // migrateUp();
    // Asking user to choose between console and testcases
    printf("Choose between console and testcases (c/t): ");
    char choice;
    scanf("%c", &choice);
    if (choice == 'c')
    {
        // Start console app
        ConsoleActionMain();
    }
    else if (choice == 't')
    {
        // Start testcases
        TestActionMain();
    }
    else
    {
        printf("Invalid choice\n");
    }
    return 0;
}