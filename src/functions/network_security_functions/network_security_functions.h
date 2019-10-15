#ifndef NETWORK_SECURITY_FUNCTIONS_H_   /* Include guard */
#define NETWORK_SECURITY_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int sign_data(char *message, const int HTTP_SETTINGS);
int verify_data(const char* MESSAGE, const int HTTP_SETTINGS, const int VERIFY_CURRENT_ROUND_PART_BACKUP_NODE_SETTINGS);
#endif