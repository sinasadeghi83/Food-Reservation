CREATE TABLE "testcase" (
	"id"	INTEGER NOT NULL UNIQUE,
	"title"	VARCHAR(255) NOT NULL UNIQUE,
	"valid_params"	VARCHAR(255) DEFAULT NULL,
	PRIMARY KEY("id")
);
INSERT INTO "main"."testcase" ("id", "title", "valid_params") VALUES ('1001', 'login', 'user,password');
INSERT INTO "main"."testcase" ("id", "title", "valid_params") VALUES ('1002', 'logout', 'user');
INSERT INTO "main"."testcase" ("id", "title", "valid_params") VALUES ('1003', 'change-pass', 'user,old-pass,new-pass');
INSERT INTO "main"."testcase" ("id", "title", "valid_params") VALUES ('1004', 'approve', 'user');
INSERT INTO "main"."testcase" ("id", "title", "valid_params") VALUES ('1005', 'change-student-pass', 'user,new-pass');
INSERT INTO "main"."testcase" ("id", "title", "valid_params") VALUES ('1006', 'remove-student', 'user');
INSERT INTO "main"."testcase" ("id", "title", "valid_params") VALUES ('1007', 'deactivate', 'user');