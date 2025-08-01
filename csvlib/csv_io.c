
#include "csv_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define CSV_FILE "data.csv"

/*****************************************************************************
 * @fn				- csv_getInt
 *
 * @brief			- This function retrieves an integer value from a CSV file.
 *
 * @param[in]		- const char* key: The key to search for in the CSV file.
 *
 * @return			- The integer value associated with the key, or -1 if not found.
 *
 */
int csv_getInt(const char* key) {

    if (key == NULL) {
        fprintf(stderr, "csv_getInt: NULL key provided.\n");
        return -1;
    }

    FILE* file = fopen(CSV_FILE, "r");
    if (!file) {
        perror("Failed to open CSV file");
        return -1; // or some error value
    }   

    // Buffer to hold each line read from the CSV file
    char line[BUFFER_SIZE];
    
    // Read each line from the CSV file
    while (fgets(line, sizeof(line), file)) {

        char line_copy[BUFFER_SIZE];
        strncpy(line_copy, line, sizeof(line_copy));
        line_copy[sizeof(line_copy) - 1] = '\0'; // Ensure null termination

        char* token_key = strtok(line_copy, ",");
        char* token_value = strtok(NULL, "\n");
        
        if (token_key && token_value && strcmp(token_key, key) == 0) {
            char* endptr;
            errno = 0; // Reset errno before strtol
            long value = strtol(token_value, &endptr, 10);
            if (errno != 0 || endptr == token_value || *endptr != '\0') {
                fprintf(stderr, "Invalid integer value for key '%s': %s\n", key, token_value);
                fclose(file);
                return -1;
            }
            fclose(file);
            return (int)value;
        }
    }
    // Key was not found
    fclose(file);
    fprintf(stderr, "Key '%s' not found in CSV file.\n", key);
    return -1; // or some error value if key not found
}

void csv_setInt(const char* key, int value) {
    // Implementation for setting an integer value in the CSV
}

const char* csv_getString(const char* key) {
    // Implementation for getting a string value from the CSV
}
