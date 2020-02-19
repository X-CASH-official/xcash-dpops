#ifndef XCASH_DPOPS_TEST_H_   /* Include guard */
#define XCASH_DPOPS_TEST_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

void initialize_data(void);
void create_overall_database_connection(void);
void get_delegates_data(void);
int set_parameters(int parameters_count, char* parameters[]);
void print_settings(void);
void database_sync_check(void);
int get_network_data_nodes_online_status(void);
void start_timer_threads(void);
void start_registration_mode(void);
int main(int parameters_count, char* parameters[]);
#endif