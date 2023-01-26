CREATE TABLE IF NOT EXISTS self (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    name VARCHAR(255) NOT NULL,
    location VARCHAR(255) NOT NULL,
    capacity INTEGER NOT NULL DEFAULT 0,
    type VARCHAR(255) CHECK( type IN ('boyish', 'girlish') ) NOT NULL DEFAULT 'boyish',
    meal VARCHAR(255) CHECK( meal IN ('lunch', 'dinner', 'both') ) NOT NULL DEFAULT 'both',
    lunch_time VARCHAR(255) NOT NULL,
    dinner_time VARCHAR(255) NOT NULL
);