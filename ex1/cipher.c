#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
 *  Written by Itamar Citrin 209701853
 *  Exercise 1 - cipher.c -
 *  A program that encrypts and decrypts text files as well as
 *  checks if two given files are encrypted with any hist.
 */
#define LEN_ALPHABET 26
#define ARG_NUM_MAIN 5 // for encode and decode functions
#define ARG_NUM_CHECK 4 // for check function
#define UPPER_CHAR_A 65
#define UPPER_CHAR_Z 90
#define LOWER_CHAR_A 97
#define LOWER_CHAR_Z 122
#define NOT_IN_ALPHABET 30 // number not in the alphabet dec value

/*!
 * Correct the value to keep the value in the correct ASCII Decimal
 * part. For example, lower case characters will remain lower case,
 * and higher case will remain higher case. Any other characters
 * will remain the same.
 * @param old_val - The original value of the char (int)
 * @param new_val - The new value of the char after adding the hist
 * to it (int)
 * @return - If non-alphabetic, old_val returned, otherwise the
 * corresponding character decimal value of new_val.
 */
int correct(int old_val, int new_val)
{
  int result = new_val;
  if(old_val >= UPPER_CHAR_A &&
     old_val <= UPPER_CHAR_Z) { // Upper char
      while(result > UPPER_CHAR_Z) {
          result = result - LEN_ALPHABET;
        }
      while(result < UPPER_CHAR_A) {
          result = result + LEN_ALPHABET;
        }
      return result;
    }
  else if(old_val >= LOWER_CHAR_A && old_val <= LOWER_CHAR_Z) { // Lower char
      while(result > LOWER_CHAR_Z) {
          result = result - LEN_ALPHABET;
        }
      while(result < LOWER_CHAR_A) {
          result = result + LEN_ALPHABET;
        }
      return result;
    }
  else {
      return old_val;
    }
}
/*!
 * Encrypt function - encrypts a given text file to another
 * text file using the given hist number
 * @param hist - Number to base the encryption on
 * @param incoming_file_path - File to encrypt
 * @param outgoing_file_path - File to save the encryption in
 * @return - 0 if bad file read, 1 if successful
 */
int encode(int hist, char *incoming_file_path,
           char *outgoing_file_path)
{
  FILE *incoming = fopen(incoming_file_path, "r");
  if(incoming == NULL) {
      return 0;
    }
  FILE *outgoing = fopen(outgoing_file_path, "w");
  char ch;
  int old_val, new_val;
  ch = fgetc(incoming);
  old_val = ch;
  new_val = correct(old_val, old_val + hist);
  while (ch != EOF) {
      fputc(new_val, outgoing);
      ch = fgetc(incoming);
      old_val = ch;
      new_val = correct(old_val, old_val + hist);
    }
  fclose(incoming);
  fclose(outgoing);
  return 1;

}

/*!
 * Decrypt function - Decrypts the text in one file into another
 * file using a given integer
 * @param hist - Integer to determine how to decrypt the file
 * @param incoming_file_path - File name to decrypt from
 * @param outgoing_file_path - File name to enter the decryption to
 * @return - 0 if unsuccessful read of the incoming file, 1 if
 * successful
 */
int decode(int hist, const char *incoming_file_path,
           const char *outgoing_file_path)
{
  FILE *incoming = fopen(incoming_file_path, "r");
  if(incoming == NULL)
  {
      return 0;
  }
  FILE *outgoing = fopen(outgoing_file_path, "w");
  char ch;
  int old_val, new_val;
  ch = fgetc(incoming);
  old_val = ch;
  new_val = correct(old_val, old_val - hist);
  while (ch != EOF)
  {
      fputc(new_val, outgoing);
      ch = fgetc(incoming);
      old_val = ch;
      new_val = correct(old_val, old_val - hist);
  }
  fclose(incoming);
  fclose(outgoing);
  return 1;
}

/*!
 * Checks if the given int is a character in ASCII Dec (A-Z or a-z)
 * @param num (int) Given int value to check if it represents a char
 * @return 1 if a char, 0 if not
 */
int is_char(const int num)
{
  if((num >= UPPER_CHAR_A && num <= UPPER_CHAR_Z) ||
     (num >= LOWER_CHAR_A && num <= LOWER_CHAR_Z))
  {
      return 1;
  }
  else
  {
      return 0;
  }
}

/*!
 * Checks if a given int represents an upper case letter in
 * ASCII Decimal value
 * @param num (int) Number to check
 * @return 1 if upper, 0 if not upper
 */
int is_upper(const int num)
{
  if (num >= UPPER_CHAR_A && num <= UPPER_CHAR_Z)
  {
    return 1;
  }
  return 0;
}

int check_files(const char *original_file_path,
                const char *encrypted_file_path) {
  FILE *original = fopen(original_file_path, "r");
  FILE *encrypted = fopen(encrypted_file_path, "r");
  if(original == NULL && encrypted == NULL) {
      return 0;
    }
  else if(original == NULL) {
      fclose(encrypted);
      return 0;
    }
  else if(encrypted == NULL) {
      fclose(original);
      return 0;
    }
  else {
      int k = 0;
      int char1 = 1, char2 = 1; // the chars are saved as ints!
      int check = NOT_IN_ALPHABET;
      while(char1 != EOF && char2 != EOF) {
          char1 = fgetc(original);
          char2 = fgetc(encrypted);
          if(is_char(char1) == 0 && is_char(char2) == 0) {
              if(char1 == char2) {
                continue;
              }
              else {
                fprintf(stdout, "Invalid encrypting\n");
                fclose(original);
                fclose(encrypted);
                exit(EXIT_SUCCESS);
              }
            }
          else if((is_char(char1) == 0 && is_char(char2)) == 1 ||
                  (is_char(char1) == 1 && is_char(char2) == 0))
          {
              fprintf(stdout, "Invalid encrypting\n");
              fclose(original);
              fclose(encrypted);
              exit(EXIT_SUCCESS);
          }
          else
            {
              if (check == NOT_IN_ALPHABET)
              {
                  check = char2-char1;
              }
              if((is_upper(char1) == 1 && is_upper(char2) == 0) ||
              (is_upper(char1) == 0 && is_upper(char2) == 1))
              /* This is fine because this block get only char1,char2
               * which represent alphabetic letters only */
              {
                fprintf(stdout, "Invalid encrypting\n");
                fclose(original);
                fclose(encrypted);
                exit(EXIT_SUCCESS);
              }
              k = char2-char1;
              if(k != check && abs(k) + abs(check) != LEN_ALPHABET)
              {
                  fprintf(stdout, "Invalid encrypting\n");
                  fclose(original);
                  fclose(encrypted);
                  exit(EXIT_SUCCESS);
              }
            }
        }
      if(char1 != EOF || char2 != EOF) {
          fprintf(stdout, "Invalid encrypting\n");
          fclose(original);
          fclose(encrypted);
          exit(EXIT_SUCCESS);
        }
      if(k < 0) {
          k = LEN_ALPHABET+k;
        }
      fprintf(stdout, "Valid encrypting with k = %d\n", k);
      fclose(original);
      fclose(encrypted);
      exit(EXIT_SUCCESS);
    }
}

int main(int argc, char *argv[]) {
  int k;
  int state;
  if (argc == 1) {
    fprintf(stderr, "The given command is invalid\n");
    return EXIT_FAILURE;
  }
  if(strcmp(argv[1], "encode") == 0) {
      if(argc != ARG_NUM_MAIN) {
          fprintf(stderr, "Usage: cipher <encode|decode> <k> <source path "
                          "file> <output path file>\n");
          return EXIT_FAILURE;
        }
      k = atoi(argv[2]);
      state = encode(k, argv[3], argv[4]);
      if(state == 0) {
          fprintf(stderr, "The given file is invalid\n");
          return EXIT_FAILURE;
        }
      else {
          return EXIT_SUCCESS;
        }
    }
  else if(strcmp(argv[1], "decode") == 0) {
      if(argc != ARG_NUM_MAIN) {
          fprintf(stderr, "Usage: cipher <encode|decode> <k> <source path file> "
                          "<output path file>\n");
          return EXIT_FAILURE;
        }
      k = atoi(argv[2]);
      state = decode(k, argv[3], argv[4]);
      if(state == 0) {
          fprintf(stderr, "The given file is invalid\n");
          return EXIT_FAILURE;
        }
      else {
          return EXIT_SUCCESS;
        }
    }
  else if(strcmp(argv[1], "check") == 0) {
      if(argc != ARG_NUM_CHECK) {
          fprintf(stderr, "Usage: cipher <check> <source path file> "
                          "<output path file>\n");
          return EXIT_FAILURE;
        }
      state = check_files(argv[2], argv[3]);
      if(state == 0) {
          fprintf(stderr, "The given file is invalid\n");
          return EXIT_FAILURE;
        }
      else {
          return EXIT_SUCCESS;
        }
    }
  else {
      fprintf(stderr, "The given command is invalid\n");
      return EXIT_FAILURE;
    }

}