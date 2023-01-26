CREATE TABLE "reserve" (
	"self_id"	INTEGER NOT NULL,
	"meal"	INTEGER NOT NULL,
	"created_at"	DATETIME NOT NULL,
	FOREIGN KEY("self_id") REFERENCES "self"("id"),
	FOREIGN KEY("meal") REFERENCES "meal"("id")
);