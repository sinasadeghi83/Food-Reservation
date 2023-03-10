CREATE TABLE IF NOT EXISTS food(
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    name VARCHAR(255) NOT NULL,
    type VARCHAR(255) CHECK( type IN ('food', 'dessert') ) NOT NULL DEFAULT 'food',
    price INTEGER NOT NULL
);