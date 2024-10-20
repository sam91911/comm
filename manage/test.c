#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sqlite3.h>

#include "account.h"

int main(int argc, char **argv){
	sqlite3 *db;
	int rc;
	char *test = "test";
	rc = sqlite3_open("test.db", &db);
	commAccount_init(db);
	commAccount_insert(db, test, 4);
	sqlite3_close(db);
	return 0;
}
