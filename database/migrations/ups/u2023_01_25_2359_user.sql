CREATE TABLE "user" (
	"username"	VARCHAR(255),
	"password"	VARCHAR(300) NOT NULL,
	"type"	VARCHAR(255) NOT NULL DEFAULT 'student' CHECK("type" IN ('admin', 'student')),
	"fname"	VARCHAR(255) NOT NULL,
	"lname"	VARCHAR(255) NOT NULL,
	"national_code"	VARCHAR(255) NOT NULL,
	"gender"	INTEGER DEFAULT 0,
	"birth_date"	DATETIME NOT NULL,
	"approved"	INTEGER NOT NULL DEFAULT 0,
	UNIQUE("username","national_code"),
	PRIMARY KEY("username")
);