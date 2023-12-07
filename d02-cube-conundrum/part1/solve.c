#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define BUFFER_SIZE 256
#define INPUT_FILENAME "../input.txt"
#define OUTPUT_FILENAME "./output.txt"

#define GAME_ID_MIN_LENGTH 5
#define RED_CUBES_LIMIT 12
#define GREEN_CUBES_LIMIT 13
#define BLUE_CUBES_LIMIT 14

bool parse_line(const char *);

int main() {
  FILE *file;

  file = fopen(INPUT_FILENAME, "r");
  if (file == NULL) {
    perror("Couldn't open a file");
    return 1;
  }

  char buffer[BUFFER_SIZE],
       cubes_number_array[sizeof(uint8_t) + 1];
  uint64_t result  = 0,
           game_id = 1;

  while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
    if (parse_line(buffer))
      result += game_id;
    game_id++;
  }

  printf("%llu\n", result);

  if (fclose(file) == EOF) {
    perror("Couldn't close a file");
    return 1;
  }

  return 0;
}

bool parse_line(const char *buffer) {
  uint16_t buffer_length = strlen(buffer),
           i             = GAME_ID_MIN_LENGTH;

  // Skip a game identificator.
  while (isdigit(buffer[i])) i++;
  i += 2;

  while (i < buffer_length) {
    uint16_t cubes_number = 0;
    while (isdigit(buffer[i])) {
      cubes_number = cubes_number * 10 + buffer[i] - '0';
      i++;
    }

    // Skip a space.
    i++;

    const char color = buffer[i];
    if (color == 'r' && cubes_number > RED_CUBES_LIMIT)
      return false;
    if (color == 'g' && cubes_number > GREEN_CUBES_LIMIT)
      return false;
    if (color == 'b' && cubes_number > BLUE_CUBES_LIMIT)
      return false;

    // Skip rest of meaningless chars
    // and go right ahead to a next digit or break the loop.
    while (!isdigit(buffer[i]) && i < buffer_length) i++;
  }

  return true;
}
