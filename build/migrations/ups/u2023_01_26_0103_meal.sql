CREATE TABLE "meal" (
	"id"	INTEGER NOT NULL,
	"self_id"	INTEGER NOT NULL,
	"food_id"	INTEGER NOT NULL,
	"type"	VARCHAR(255) NOT NULL DEFAULT 'lunch' CHECK(type IN ('lunch', 'dinner')),
	"count"	INTEGER NOT NULL DEFAULT 0,
	"date"	DATETIME NOT NULL,
	PRIMARY KEY("id" AUTOINCREMENT),
	FOREIGN KEY("self_id") REFERENCES "self"("id"),
	FOREIGN KEY("food_id") REFERENCES "food"("id")
);