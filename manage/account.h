#ifndef COMM_MANAGE_ACCOUNT
#define COMM_MANAGE_ACCOUNT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <sqlite3.h>
#include <openssl/evp.h>

int commAccount_init(sqlite3 *db);
int commAccount_insert(sqlite3 *db, char *account, uint8_t len);
int commAccount_check(sqlite3 *db, char *account);
int commAccount_remove(sqlite3 *db, char *account);
int commAccount_abbr(sqlite3 *db, char *account, char *result);
#endif
