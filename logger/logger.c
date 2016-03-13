/*

 # logger source file
 #
 # Lang:     C
 # Author:   okhlopkov
 # Version:  0.1

 */

#import <unistd.h>  // for log file finder
#include <string.h> // for memset
#include <assert.h>
#include <stdio.h>

#import "logger.h"

/* PRIVATE FUNCTIONS */
// finds propriate file to write logs
uint32_t _find_log_file(char *dst) {
  uint32_t current_file_number = 0;
  char flag = 1;
  // 10 is the length of the UINT32 max integer: 4294967295
  // 9 is for "logs/" + ".txt" 7 is for "logger/"

  char *current_file_name = (char *)calloc(10 + 9 + 7, sizeof(char));
  while (flag) {
    sprintf(current_file_name, "logger/logs/%u.txt", current_file_number);
    if (access(current_file_name, F_OK) != -1) {
      // Log file exists, move to another number
      current_file_number += 1;
    } else {
      // File doesn't exists -> exit cycle
      FILE *output = fopen(current_file_name, "a");
      fclose(output);
      flag = 0;
    }
  }
  // printf("%s", current_file_name);
  memcpy(dst, current_file_name, (10 + 9 + 7) * sizeof(char));
  free(current_file_name);
  return 0;
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
  logger->buffer->size    = 0;
  // 10 is the length of the UINT32 max integer: 4294967295
  // 9 is for "logs/" + ".txt"
  logger->log_file_name = (char *)calloc(10 + 9 + 7, sizeof(char));
  if (logger->log_file_name == NULL) {
    perror("Log file name data memory allocation error.\n");
    return NULL;
  }
  _find_log_file(logger->log_file_name);

  return logger;
}

uint32_t logger_flush(logger_t *logger) {
  assert (logger != NULL);
  if (logger->buffer->size == 0) {
    return 0;
  }
  #ifdef ECHO
    printf("LOGGER:  Flushing into %s\n", logger->log_file_name);
  #endif
  FILE *output = fopen(logger->log_file_name, "a");
  if (output == 0) {
    printf("Can't open file %s\n", logger->log_file_name);
    return 1;
  }

  if (fputs(logger->buffer->data, output) < 0) {
    printf("Error with writing into %s\n", logger->log_file_name);
    return 1;
  }

  memset(logger->buffer->data, 0, logger->buffer->size * sizeof(char));
  logger->buffer->size = 0;

  fclose(output);
  return 0;
}

uint32_t LOG(logger_t *logger, char *msg) {
  assert (logger != NULL);
  assert (msg != NULL);
  #ifdef ECHO
    printf("LOGGER:  %s\n", msg);
  #endif
  uint32_t current_size = logger->buffer->size;
  // i letf last element in buffer for \0
  if (current_size + (uint32_t)strlen(msg) >= BUFFERSIZE) {
    if (logger_flush(logger)) {
      printf("Cant'flush.\n");
      return 1;
    }
  }
  int count = sprintf(&logger->buffer->data[current_size], "%s\n", msg);

  logger->buffer->size += count;
  return 0;
}

uint32_t logger_deinit(logger_t *logger) {
  assert (logger != NULL);
  if (logger_flush(logger)) {
    printf("Can't flush while destruct");
  }
  free(logger->buffer->data);
  free(logger->buffer);
  free(logger->log_file_name);
  free(logger);
  return 0;
}
