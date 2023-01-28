#include "MealController.h"
#include "UserController.h"
#include "../sinastd/Error.h"
#include "../sinastd/Param.h"
#include "../models/Meal.h"
#include "../models/Self.h"
#include <stdlib.h>
#include <string.h>

Error *MealActionAdd(Param **params)
{
    // Check if the user is logged in
    if (UserGetSessionUser() == NULL)
    {
        return ErrorCreate(true, "You must be logged in to add a meal", ERR_PERM);
    }

    // Check if the user is admin
    if (UserGetSessionUser()->type != USER_ADMIN)
    {
        return ErrorCreate(true, "You must be an admin to add a meal", ERR_PERM);
    }

    // Declare variables
    int selfId = 0;
    int foodId = 0;
    SelfMeal type = SelfMeal_INVALID;
    int count = 0;
    Date *date = NULL;
    // Get the parameters
    for (int i = 0; params[i] != NULL; i++)
    {
        if (strcmp(params[i]->name, "self-id") == 0)
        {
            selfId = atoi(params[i]->value);
        }
        else if (strcmp(params[i]->name, "food-id") == 0)
        {
            foodId = atoi(params[i]->value);
        }
        else if (strcmp(params[i]->name, "type") == 0)
        {
            type = SelfMealFromString(params[i]->value);
        }
        else if (strcmp(params[i]->name, "count") == 0)
        {
            count = atoi(params[i]->value);
        }
        else if (strcmp(params[i]->name, "date") == 0)
        {
            date = CreateDateFromString(params[i]->value);
        }
    }
    // Create the meal
    Meal *meal = MealCreate(0, selfId, foodId, type, count, date);
    // Save the meal
    Error *err = MealSave(meal);
    // Free the meal
    MealFree(meal);
    // Return the error
    return err;
}