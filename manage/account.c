#include "account.h"

static int search_check(void* data, int index, char **key, char **value){
	*(int *)data = 1;
	return 0;
}

int commAccount_init(sqlite3 *db){
	if(!db) return -1;
	//return value
	int rv;
	char *errMsg = 0;
	char *sql;
	sql =	"CREATE TABLE IF NOT EXISTS account("
		"id 		INTEGER PRIMARY KEY AUTOINCREMENT,"
		"account	TEXT	NOT NULL UNIQUE,"
		"abbr		TEXT	NOT NULL);";
	rv = sqlite3_exec(db, sql, 0, 0, &errMsg);
	if(rv != SQLITE_OK){
		fprintf(stderr, "commAccount_init: %s\n", errMsg);
	}
	sqlite3_free(errMsg);
	return 0;
}
int commAccount_insert(sqlite3 *db, char *account, uint8_t len){
	if(!db) return -1;
	if(!account) return -1;
	if(len > 128) return -1;
	char SQL_buffer[1024];
	char account_buffer[257];
	char hash_256_buffer[64];
	char abbr_buffer[129];
	for(int i = 0 ; i < len ; i++){
		sprintf(account_buffer+2*i, "%02X", account[i]);
	}
	EVP_Digest(account, len, hash_256_buffer, 0, EVP_sha3_256(), 0);
	for(int i = 0 ; i < 64 ; i++){
		sprintf(abbr_buffer+2*i, "%02X", hash_256_buffer[i]);
	}
	sprintf(SQL_buffer, "INSERT INTO account (account, abbr)"
			"VALUES ('%s', '%s');", account_buffer, abbr_buffer);
	int rc;
	char *errMsg;
	rc = sqlite3_exec(db, SQL_buffer, 0, 0, &errMsg);
	if(rc != SQLITE_OK){
		fprintf(stderr, "commAccount_insert: %s\n", errMsg);
	}
	sqlite3_free(errMsg);
	return 0;
}
int commAccount_check(sqlite3 *db, char *account){
	if(!db) return -1;
	if(!account) return -1;
	int rc;
	char *errMsg;
	char SQL_buffer[1024];
	char account_buffer[257];
	
}
int commAccount_remove(sqlite3 *db, char *account);
int commAccount_abbr(sqlite3 *db, char *account, char *result);
