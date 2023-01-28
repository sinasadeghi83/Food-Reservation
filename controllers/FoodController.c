#include "FoodController.h"
#include "../models/Food.h"
#include "../sinastd/Param.h"
#include "../sinastd/Error.h"
#include "../sinastd/Parser.h"
#include "UserController.h"
#include <stdlib.h>
#include <string.h>

// Food add action
Error *FoodActionAdd(Param **params)
{
    // Checking if user is logged in
    if (UserGetSessionUser() == NULL)
        return ErrorCreate(true, "You must be logged in to add food", ERR_PERM);
    // Checking if user is admin
    if (UserGetSessionUser()->type != USER_ADMIN)
        return ErrorCreate(true, "You must be an admin to add food", ERR_PERM);
    // Food params
    int id;
    char *name;
    char *type;
    int price;
    // Getting params
    for (int i = 0; params[i] != NULL; i++)
    {
        if (strcmp(params[i]->name, "id") == 0)
        {
            if (!isNumeric(params[i]->value))
            {
                return ErrorCreate(true, "id must be a number", ERR_INVALID);
            }
            id = atoi(params[i]->value);
        }
        else if (strcmp(params[i]->name, "name") == 0)
            name = params[i]->value;
        else if (strcmp(params[i]->name, "type") == 0)
            type = params[i]->value;
        else if (strcmp(params[i]->name, "price") == 0)
        {
            if (!isNumeric(params[i]->value))
            {
                return ErrorCreate(true, "Price must be a number", ERR_INVALID);
            }
            price = atoi(params[i]->value);
        }
    }
    FoodType foodType = FoodTypeFromString(type);
    Food *food = FoodCreate(id, name, foodType, price);
    Error *error = FoodSave(food);
    FoodFree(food);
    return error;
}