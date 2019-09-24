#ifndef THREAD_FUNCTIONS_H_   /* Include guard */
#define THREAD_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

void* read_file_thread(void* parameters);
void* write_file_thread(void* parameters);
void* insert_document_into_collection_json_thread(void* parameters);
void* insert_multiple_documents_into_collection_json_thread(void* parameters);
void* read_document_from_collection_thread(void* parameters);
void* read_document_field_from_collection_thread(void* parameters);
void* read_document_all_fields_from_collection_thread(void* parameters);
void* read_multiple_documents_all_fields_from_collection_thread(void* parameters);
void* update_document_from_collection_thread(void* parameters);
void* update_all_documents_from_collection_thread(void* parameters);
void* delete_document_from_collection_thread(void* parameters);
void* delete_collection_from_database_thread(void* parameters);
void* count_documents_in_collection_thread(void* parameters);
void* count_all_documents_in_collection_thread(void* parameters);
void* get_database_data_thread(void* parameters);
void* get_database_data_hash_thread(void* parameters);
int thread_settings(pthread_t thread_id);
#endif