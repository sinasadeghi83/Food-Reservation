#include "SelfController.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../sinastd/Error.h"
#include "../sinastd/Param.h"
#include "../models/Self.h"
#include "../sinastd/Parser.h"
#include "../sinastd/TimePeriod.h"
#include "../models/User.h"
#include "UserController.h"

// Add a new self to the database according to the parameters only by admin
Error *SelfActionAdd(Param **params)
{
    // Create Error
    Error *error = ErrorCreate(false, NULL, NULL);
    // Check if the user logged in
    if (UserGetSessionUser() == NULL)
    {
        error->isAny = true;
        error->msg = "You must be logged in to perform this action";
        error->testMsg = ERR_PERM;
        return error;
    }

    // Check if the user is admin
    if (UserGetSessionUser()->type != USER_ADMIN)
    {
        error->isAny = true;
        error->msg = "You must be admin to perform this action";
        error->testMsg = ERR_PERM;
        return error;
    }
    // Define params
    int id;
    char *name;
    char *location;
    int capacity;
    SelfType type;
    SelfMeal meal;
    TimePeriod *lunchTime;
    TimePeriod *dinnerTime;
    // Get the params
    for (int i = 0; params[i] != NULL; i++)
    {
        if (strcmp(params[i]->name, "id") == 0)
        {
            id = atoi(params[i]->value);
        }
        else if (strcmp(params[i]->name, "name") == 0)
        {
            name = params[i]->value;
        }
        else if (strcmp(params[i]->name, "location") == 0)
        {
            location = params[i]->value;
        }
        else if (strcmp(params[i]->name, "capacity") == 0)
        {
            capacity = atoi(params[i]->value);
        }
        else if (strcmp(params[i]->name, "type") == 0)
        {
            type = SelfTypeFromString(params[i]->value);
        }
        else if (strcmp(params[i]->name, "meal") == 0)
        {
            meal = SelfMealFromString(params[i]->value);
        }
        else if (strcmp(params[i]->name, "lunch-time-limit") == 0)
        {
            lunchTime = TimePeriodFromString(params[i]->value);
        }
        else if (strcmp(params[i]->name, "dinner-time-limit") == 0)
        {
            dinnerTime = TimePeriodFromString(params[i]->value);
        }
    }

    free(error);

    // Create a new self
    Self *self = SelfCreate(id, name, location, capacity, type, meal, lunchTime, dinnerTime);
    // Add the self to the database
    error = SelfSave(self);
    // Free the self
    SelfFree(self);
    return error;
}