CREATE TABLE IF NOT EXISTS user (
    username VARCHAR(255) PRIMARY KEY,
    password VARCHAR(255) NOT NULL,
    type VARCHAR(255) CHECK( type IN ('admin', 'student') ) NOT NULL DEFAULT 'student',
    fname VARCHAR(255) NOT NULL,
    lname VARCHAR(255) NOT NULL,
    national_code VARCHAR(255) NOT NULL,
    gender INT DEFAULT 0,
    birth_date DATETIME NOT NULL,
    UNIQUE (username, national_code)
);