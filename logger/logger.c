/*

 # logger source file
 #
 # Lang:     C
 # Author:   okhlopkov
 # Version:  0.1

 */

#import <unistd.h>  // for log file finder
#include <string.h> // for memset
#import "logger.h"

/* PRIVATE FUNCTIONS */
// finds propriate file to write logs
uint32_t _find_log_file() {
  uint32_t current_file_number = 0;
  char flag = 1;
  // 10 is the length of the UINT32 max integer: 4294967295
  // 9 is for "logs/" + ".txt"
  while (flag) {
    char *current_file_name = (char *)calloc(10 + 9, sizeof(char));
    sprintf(current_file_name, "logs/%u.txt", current_file_number);

    if (access(current_file_name, F_OK) != -1) {
      // Log file exists, move to another number
      current_file_number += 1;
    } else {
      // File doesn't exists -> exit cycle
      flag = 0;
    }
  }
  return current_file_number;
}

logger_t *logger_init() {
  logger_t *logger = (logger_t *)malloc(sizeof(logger_t));
  if (logger == NULL) {
    perror("Logger memory allocation error.\n");
    return NULL;
  }

  logger->buffer = (buffer_t *)malloc(sizeof(buffer_t));
  if (logger->buffer == NULL) {
    perror("Logger's buffer memory allocation error.\n");
    return NULL;
  }


  logger->buffer->data = (char *)calloc(BUFFERSIZE, sizeof(char));
  if (logger->buffer == NULL) {
    perror("Logger's buffer's data memory allocation error.\n");
    return NULL;
  }

  // set properties:

  logger->buffer->pointer = 0;
  logger->buffer->size    = 0;
  logger->log_file_number = _find_log_file();

  return logger;
}

uint32_t logger_flush(logger_t * logger) {
  if (logger->buffer->size == 0) {
    return 0;
  }
  char *file_name = (char *)calloc(10 + 9, sizeof(char));
  sprintf(file_name, "logs/%u.txt", logger->log_file_number);

  FILE *output = fopen(file_name, "a");
  if (output == 0) {
    printf("Can't open file: %s\n", file_name);
    return 1;
  }

  if (fputs(logger->buffer->data, output) <= 0) {
    printf("Error with writing into %s\n", file_name);
    return 1;
  }

  memset(logger->buffer->data, 0, logger->buffer->size * sizeof(char));
  logger->buffer->size = 0;
  
  return 0;
}

uint32_t logger_deinit(logger_t *logger) {
  logger_flush(logger);
  return 0;
}
