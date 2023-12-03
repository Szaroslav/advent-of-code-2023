#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE 128
#define DIGIT_WORDS_SIZE 10
#define EMPTY 0
#define INPUT_FILENAME "./input.txt"
#define OUTPUT_FILENAME "./output.txt"

typedef struct digit_word {
  char *name;
  uint8_t value;
} DigitWord;

const DigitWord DIGIT_WORDS[DIGIT_WORDS_SIZE] = {
  { "zero",  '0' },
  { "one",   '1' },
  { "two",   '2' },
  { "three", '3' },
  { "four",  '4' },
  { "five",  '5' },
  { "six",   '6' },
  { "seven", '7' },
  { "eight", '8' },
  { "nine",  '9' },
};

static inline bool is_digit_word(const char *text_offset, const char *digit_word) {
  return memcmp(text_offset, digit_word, strlen(digit_word)) == 0;
}

static inline uint8_t two_digit_string_to_number(const char *digits) {
  return (digits[0] - '0') * 10 + digits[1] - '0';
}

int main() {
  FILE *file;

  file = fopen(INPUT_FILENAME, "r");
  if (file == NULL) {
    perror("Couldn't open a file");
    return 1;
  }

  uint64_t results[2] = { };
  char buffer[BUFFER_SIZE];

  while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
    char digits[2][2] = { };

    size_t buffer_length = strlen(buffer);

    for (size_t i = 0; i < buffer_length; i++) {
      if (isdigit(buffer[i])) {
        if (digits[0][0] == EMPTY)
          digits[0][0] = buffer[i];
        digits[0][1] = buffer[i];

        if (digits[1][0] == EMPTY)
          digits[1][0] = buffer[i];
        digits[1][1] = buffer[i];

        continue;
      }

      for (uint8_t j = 0; j < DIGIT_WORDS_SIZE; j++) {
        DigitWord digit_word = DIGIT_WORDS[j];
        uint8_t digit_word_length = strlen(digit_word.name);
        if (is_digit_word(&buffer[i], digit_word.name)) {
          if (digits[1][0] == EMPTY)
            digits[1][0] = digit_word.value;
          digits[1][1] = digit_word.value;
        }
      }
    }

    results[0] += two_digit_string_to_number(digits[0]);
    results[1] += two_digit_string_to_number(digits[1]);
  }

  if (fclose(file) == EOF) {
    perror("Couldn't close a file");
    return 1;
  }

  printf("Total sum of calibration values:           %lld.\n", results[0]);
  printf("Total sum of calibration values, extended: %lld.\n", results[1]);

  file = fopen(OUTPUT_FILENAME, "w");
  snprintf(buffer, BUFFER_SIZE, "%lld\n%lld\n", results[0], results[1]);
  fwrite(buffer, strlen(buffer), 1, file);
  if (fclose(file) == EOF) {
    perror("Couldn't close a file");
    return 1;
  }

  return 0;
}
