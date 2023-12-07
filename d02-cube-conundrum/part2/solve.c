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

typedef struct cubes_numbers {
  uint16_t red;
  uint16_t green;
  uint16_t blue;
} CubesNumbers;

int max(const int, const int);
CubesNumbers parse_line(const char *);

int main() {
  FILE *file;

  file = fopen(INPUT_FILENAME, "r");
  if (file == NULL) {
    perror("Couldn't open a file");
    return 1;
  }

  char buffer[BUFFER_SIZE],
       cubes_number_array[sizeof(uint8_t) + 1];
  uint64_t result = 0;

  while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
    CubesNumbers min_cubes_numbers = parse_line(buffer);
    uint16_t red   = min_cubes_numbers.red,
             green = min_cubes_numbers.green,
             blue  = min_cubes_numbers.blue;
    result += red * green * blue;
  }

  printf("%llu\n", result);

  if (fclose(file) == EOF) {
    perror("Couldn't close a file");
    return 1;
  }

  return 0;
}

int max(const int a, const int b) {
  return a > b ? a : b;
}

CubesNumbers parse_line(const char *buffer) {
  uint16_t buffer_length = strlen(buffer),
           i             = GAME_ID_MIN_LENGTH;
  CubesNumbers min_cubes_numbers = {
    .red   = 0,
    .green = 0,
    .blue  = 0
  };

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
    if (color == 'r')
      min_cubes_numbers.red   = max(min_cubes_numbers.red,   cubes_number);
    if (color == 'g')
      min_cubes_numbers.green = max(min_cubes_numbers.green, cubes_number);
    if (color == 'b')
      min_cubes_numbers.blue  = max(min_cubes_numbers.blue,  cubes_number);

    // Skip rest of meaningless chars
    // and go right ahead to a next digit or break the loop.
    while (!isdigit(buffer[i]) && i < buffer_length) i++;
  }

  return min_cubes_numbers;
}
