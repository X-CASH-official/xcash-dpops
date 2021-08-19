#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <stdio.h>
#include "md5.h"
#include "define_macro_functions.h"

int get_data(mongoc_client_t *client, const char *db_name, const char *field_name, char *data)
{
    bson_t *query;
    bson_t *opts;
    mongoc_collection_t *collection;

    mongoc_cursor_t *cursor;
    bson_error_t error;
    const bson_t *doc;
    bson_iter_t iter;
    bson_iter_t field;
    const char *hash_data;
    int result = -1;
    uint32_t len = 0;

    collection = mongoc_client_get_collection(client, "XCASH_PROOF_OF_STAKE", "hashes");

    query = BCON_NEW("db_name", db_name);

    opts = BCON_NEW("projection", "{",
                    field_name, BCON_BOOL(true),
                    "_id", BCON_BOOL(false),
                    "}");

    cursor = mongoc_collection_find_with_opts(collection, query, opts, NULL);

    while (mongoc_cursor_next(cursor, &doc))
    {
        if (bson_iter_init(&iter, doc) && bson_iter_find_descendant(&iter, field_name, &field) && BSON_ITER_HOLDS_UTF8(&field))
        {
            hash_data = bson_iter_utf8(&field, &len);
            strcpy(data, hash_data);
            result = 0;
        }
    }

    bson_destroy(query);
    bson_destroy(opts);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);

    return result;
}

int get_db_hashes(mongoc_client_t *client, const char *db_name, char *hash, char *db_hash)
{
    bson_t *query;
    bson_t *opts;
    mongoc_collection_t *collection;

    mongoc_cursor_t *cursor;
    bson_error_t error;
    const bson_t *doc;
    bson_iter_t iter;
    bson_iter_t field;
    const char *hash_data;
    int result = -1;
    uint32_t len = 0;

    collection = mongoc_client_get_collection(client, "XCASH_PROOF_OF_STAKE", "hashes");

    query = BCON_NEW("db_name", db_name);

    opts = BCON_NEW("projection", "{",
                    "hash", BCON_BOOL(true),
                    "db_hash", BCON_BOOL(true),
                    "_id", BCON_BOOL(false),
                    "}");

    cursor = mongoc_collection_find_with_opts(collection, query, opts, NULL);

    while (mongoc_cursor_next(cursor, &doc))
    {
        result = 0;
        if (bson_iter_init(&iter, doc) && bson_iter_find_descendant(&iter, "hash", &field) && BSON_ITER_HOLDS_UTF8(&field))
        {
            strcpy(hash, bson_iter_utf8(&field, &len));
        }
        else
        {
            result = -2;
            fprintf(stderr, "Failed to parse hash for %s\n", db_name);
        }

        if (result == 0 && bson_iter_init(&iter, doc) && bson_iter_find_descendant(&iter, "db_hash", &field) && BSON_ITER_HOLDS_UTF8(&field))
        {
            strcpy(db_hash, bson_iter_utf8(&field, &len));
        }
        else
        {
            result = -3;
            fprintf(stderr, "Failed to parse db_hash for %s\n", db_name);
        }
    }

    bson_destroy(query);
    bson_destroy(opts);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);

    return result;
}

int calc_db_hashes(mongoc_client_t *client, const char *db_name, char *hash, char *db_hash)
{
    bson_t *command;
    bson_error_t error;
    mongoc_collection_t *collection;
    bson_t reply;

    bson_iter_t iter;
    bson_iter_t field;
    const char *l_hash = NULL;
    const char *l_db_hash = NULL;
    uint32_t len = 0;

    char param[512];

    int result = 0;
    // char *str;

    collection = mongoc_client_get_collection(client, "XCASH_PROOF_OF_STAKE", db_name);

    command = BCON_NEW("dbHash",
                       BCON_INT32(1),
                       "collections",
                       "[",
                       BCON_UTF8(db_name),
                       "]");

    // command = BCON_NEW("dbHash",
    //                    BCON_INT32(1));

    // command = bson_new_from_json("{\"dbHash\":1,\"collections\":[\"delegates\"]}", -1, &error);

    // str = bson_as_canonical_extended_json(command, NULL);
    // bson_free(str);

    if (mongoc_collection_command_simple(
            collection, command, NULL, &reply, &error))
    {
        if (bson_iter_init(&iter, &reply) && bson_iter_find_descendant(&iter, "md5", &field) && BSON_ITER_HOLDS_UTF8(&field))
        {
            l_hash = bson_iter_utf8(&field, &len);
            // padding with zeroes
            memset(hash, '0', 96);
            // copy md5 hash including \0
            strcpy(hash + 96, l_hash);
        }
        else
        {
            result = -2;
            fprintf(stderr, "Failed to parse md5 for %s\n", db_name);
        }

        snprintf(param, sizeof(param), "collections.%s", db_name);
        if (result == 0 && bson_iter_init(&iter, &reply) && bson_iter_find_descendant(&iter, param, &field) && BSON_ITER_HOLDS_UTF8(&field))
        {
            l_db_hash = bson_iter_utf8(&field, &len);
            // copy md5 hash including \0
            strcpy(db_hash, l_db_hash);
        }
        else
        {
            // for empty collection there is no db_hash, so, make it empty
            memset(db_hash, 0, 32);

            // result = -3;
            // fprintf(stderr, "Failed to parse db_hash for %s\n", db_name);
        }
    }
    else
    {
        result = -1;
        fprintf(stderr, "Failed to run command dbHash for %s: %s\n", db_name, error.message);
    }

    bson_destroy(command);
    bson_destroy(&reply);
    mongoc_collection_destroy(collection);
    return result;
}

int update_hashes(mongoc_client_t *client, const char *db_name, const char *hash, const char *db_hash)
{
    bson_t *filter;
    bson_t *update;
    bson_t *opts;
    mongoc_collection_t *collection;

    mongoc_cursor_t *cursor;
    bson_error_t error;
    int result = 0;

    collection = mongoc_client_get_collection(client, "XCASH_PROOF_OF_STAKE", "hashes");

    filter = BCON_NEW("db_name", db_name);

    opts = BCON_NEW("upsert", BCON_BOOL(true));

    update = BCON_NEW("$set",
                      "{",
                      "db_hash",
                      BCON_UTF8(db_hash),
                      "hash",
                      BCON_UTF8(hash),
                      "}");

    if (!mongoc_collection_update_one(collection, filter, update, opts, NULL, &error))
    {
        fprintf(stderr, "Update hashes %s failed %s\n", db_name, error.message);
        result = -1;
    }

    bson_destroy(filter);
    bson_destroy(update);
    bson_destroy(opts);
    mongoc_collection_destroy(collection);

    return result;
}

int get_hash(mongoc_client_t *client, const char *db_name, char *hash)
{
    char l_hash[129];
    char l_db_hash[33];
    int result = 0;

    result = get_data(client, db_name, "hash", hash);
    if (result != 0)
    {
        // recalculate hashes
        result = calc_db_hashes(client, db_name, l_hash, l_db_hash);
        if (result != 0)
            return -1;

        // and now update hashes
        result = update_hashes(client, db_name, l_hash, l_db_hash);
        if (result != 0)
            return -2;

        strcpy(hash, l_hash);
    }

    return result;
}

int get_dbhash(mongoc_client_t *client, const char *db_name, char *db_hash)
{
    char l_hash[129];
    char l_db_hash[33];
    int result = 0;

    result = get_data(client, db_name, "db_hash", db_hash);
    if (result != 0)
    {
        // recalculate hashes
        result = calc_db_hashes(client, db_name, l_hash, l_db_hash);
        if (result != 0)
            return -1;

        // and now update hashes
        result = update_hashes(client, db_name, l_hash, l_db_hash);
        if (result != 0)
            return -2;

        strcpy(db_hash, l_db_hash);
    }

    return result;
}

int del_hash(mongoc_client_t *client, const char *db_name)
{
    mongoc_collection_t *collection;
    bson_error_t error;
    bson_t *filter;
    int result = 0;

    collection = mongoc_client_get_collection(client, "XCASH_PROOF_OF_STAKE", "hashes");

    filter = BCON_NEW("db_name", db_name);

    if (!mongoc_collection_delete_one(
            collection, filter, NULL, NULL, &error))
    {
        fprintf(stderr, "Delete hashes %s failed: %s\n", db_name, error.message);
        result = -1;
    }

    bson_destroy(filter);
    mongoc_collection_destroy(collection);

    return result;
}

void bin_to_hex(const unsigned char *bin_data, int data_size, char *buf)
{
    const char *hex = "0123456789abcdef";

    const unsigned char *p_in = bin_data;
    char *p_out = buf;
    int bin_size = data_size;
    while (bin_size-- > 0)
    {
        *p_out++ = hex[(*p_in >> 4) & 0xF];
        *p_out++ = hex[*p_in & 0xF];
        p_in++;
    }
    *p_out = 0;
}

void md5_hex(const char *src, char *dest)
{
    MD5_CTX md5;
    unsigned char md5_bin[16];

    MD5_Init(&md5);
    MD5_Update(&md5, src, strlen(src));
    MD5_Final(md5_bin, &md5);

    bin_to_hex(md5_bin, sizeof(md5_bin), dest);
    // printf("%s\n", dest);
}

int calc_multi_hash(mongoc_client_t *client, const char *db_prefix, int max_index, char *hash)
{
    int result = -1;
    size_t reserve_bytes_index = 100;
    MD5_CTX md5;

    char l_db_hash[33];
    unsigned char md5_bin[16];

    // this is more than enough for name+index
    char db_name[64];

    MD5_Init(&md5);

    for (int i = 1; i <= max_index; i++)
    {
        snprintf(db_name, sizeof(db_name), "%s_%d", db_prefix, i);
        if (get_dbhash(client, db_name, l_db_hash) != 0)
        {
            fprintf(stderr, "Get hash for %s", db_name);
            return -1;
        }
        printf("%s", l_db_hash);
        MD5_Update(&md5, l_db_hash, strlen(l_db_hash));
    }
    MD5_Final(md5_bin, &md5);
    memset(hash, '0', 96);
    bin_to_hex(md5_bin, sizeof(md5_bin), hash+96);
    return 0;
}

// #define TOTAL_RESERVE_PROOFS_DATABASES 50
// 0 - error, 1 ok
int get_multi_hash(mongoc_client_t *client, const char *db_prefix, char *hash)
{

    int result;
    size_t reserve_bytes_index;

    if (strcmp(db_prefix, "reserve_bytes") == 0)
    {
        // get_reserve_bytes_database(reserve_bytes_index, 0);
        get_reserve_bytes_database(reserve_bytes_index,0);        
        result = calc_multi_hash(client, db_prefix, reserve_bytes_index, hash);
    }
    else if (strcmp(db_prefix, "reserve_proofs") == 0)
    {
        result = calc_multi_hash(client, db_prefix, TOTAL_RESERVE_PROOFS_DATABASES, hash);
    }
    else
    {
        result = get_hash(client, db_prefix, hash);
    }

    // make result compatibility with DPOPS code
    return result == 0 ? 1 : 0;
}
/*
int main(int argc, char *argv[])
{
    mongoc_client_t *client;
    mongoc_collection_t *collection;
    const char *str;
    unsigned char hash[129];
    unsigned char db_hash[33];

    md5_hex("blablabla", db_hash);

    mongoc_init();

    client = mongoc_client_new("mongodb://localhost:27017/?appname=find-example");

    get_multi_hash(client, "reserve_proofs", hash);

    calc_db_hashes(client, "reserve_bytes_115", hash, db_hash);

    printf("%d : %s\n", strlen(hash), hash);

    printf("%d : %s\n", strlen(db_hash), db_hash);

    // str = get_hash(collection, "reserve_proofs_115");

    // if (str)
    // {
    //     printf("%s\n", str);
    //     bson_free((void *)str);
    // }

    mongoc_client_destroy(client);
    mongoc_cleanup();

    return 0;
}
*/