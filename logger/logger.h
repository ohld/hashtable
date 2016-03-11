/*

 # logger header file
 #
 # Lang:     C
 # Author:   okhlopkov
 # Version:  0.1

 */

#include <stdlib.h>

#define BUFFERSIZE 4048

typedef struct {
       char *data;
       uint32_t pointer;
       uint32_t size;
} buffer_t;

typedef struct {
        buffer_t *buffer;
        uint32_t log_file_number;
} logger_t;

/* Constructor & Destructor */
logger_t *logger_init();
uint32_t logger_deinit(logger_t *logger);

/* Logs string */
uint32_t LOG(logger_t *logger, char *string);
