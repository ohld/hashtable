/*

 # logger header file
 #
 # Lang:     C
 # Author:   okhlopkov
 # Version:  0.1

 */

#include <stdlib.h>

#define BUFFERSIZE 2048
#define MSGSIZE 256

typedef struct {
       char *data;
       uint32_t size;
} buffer_t;

typedef struct {
        char *log_file_name;
        buffer_t *buffer;
} logger_t;

logger_t *logger;
char *msg;


/* Constructor & Destructor */
logger_t *logger_init();
uint32_t logger_deinit(logger_t *logger);

/* Logs string */
uint32_t LOG(logger_t *logger, char *string);
