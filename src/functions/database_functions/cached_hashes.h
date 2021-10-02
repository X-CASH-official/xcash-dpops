#ifndef CACHED_HASHES_H_
#define CACHED_HASHES_H_

int get_multi_hash(mongoc_client_t *client, const char *db_prefix, char *hash);
int del_hash(mongoc_client_t *client, const char *db_name);
int drop_all_hashes(mongoc_client_t *client);

#endif
