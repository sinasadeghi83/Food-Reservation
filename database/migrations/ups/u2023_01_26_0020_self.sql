CREATE TABLE "self" (
	"id"	INTEGER NOT NULL,
	"name"	VARCHAR(255) NOT NULL,
	"location"	VARCHAR(255) NOT NULL,
	"capacity"	INTEGER NOT NULL DEFAULT 0,
	"type"	VARCHAR(255) NOT NULL DEFAULT 'boyish' CHECK("type" IN ('boyish', 'girlish')),
	"meal"	VARCHAR(255) NOT NULL DEFAULT 'both' CHECK("meal" IN ('lunch', 'dinner', 'both')),
	"lunch_time"	VARCHAR(255) DEFAULT NULL,
	"dinner_time"	VARCHAR(255) DEFAULT NULL,
	PRIMARY KEY("id" AUTOINCREMENT)
);