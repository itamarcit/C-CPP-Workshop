#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX_BUFFER_SIZE 63
#define MIN_BUFFER_SIZE 21
#define COMMA ","
#define ID_ERROR 0
#define AGE_ERROR 1
#define GRADE_ERROR 2
#define VALID_INPUT 3
#define TRUE 1
#define FALSE 0
#define USAGE "USAGE: $ ./manageStudents <best/bubble/quick>\n"
#define ID_ERR_MSG "ERROR: ID must have 10 digits \
and cannot start with a 0.\n"
#define GRADE_ERR_MSG "ERROR: Grade must be an integer \
between 0 and 100.\n"
#define AGE_ERR_MSG "ERROR: Age must be an integer \
between 18 and 120.\n"
#define STU_NUM_ERR "ERROR: Number of students must only \
contain numbers.\n"
#define BEST 1
#define BUBBLE 2
#define QUICK 3
#define INVALID 0
#define ARG_1 "best"
#define ARG_2 "bubble"
#define ARG_3 "quick"
#define ID_LENGTH 10
#define DECIMAL 10
#define GRADE_LOW 0
#define GRADE_HIGH 100
#define AGE_LOW 18
#define AGE_HIGH 120

typedef struct Student {
    unsigned long int id;
    unsigned int grade;
    unsigned int age;
} Student;

/*
 * A function that creates a struct from type student
 */
Student create_student (long int id, int grade, int age)
{
  Student stud;
  stud.id = id;
  stud.grade = grade;
  stud.age = age;
  return stud;
}

/*
 * A function that makes an array of Student
 * structs of given size. (heap array)
 * size - unsigned int to define the number of students
 * in the array
 */
Student *make_array (unsigned int size)
{
  Student *arr = (Student *) malloc (size * sizeof (Student));
  if (arr == NULL)
    {
      return FALSE;
    }
  else
    {
      return arr;
    }
}

/*
 * Checks if a char* consists of numbers only (not
 * including \n) and checks that the number is not
 * 0 if first_check is 1
 * num - char* to check
 * space - if the func should check for \n 1, 0 otherwise
 * first_check - 1 if to also check if num is equal to 0
 */
int is_number_only (char *num, int space, int first_check)
{

  if (space == TRUE)
    {
      if (strlen (num) == 1)
        {
          return FALSE;
        }
      if (first_check == TRUE && strlen (num) == 2) // check if input is 0
        {
          int n = (int) strtol (num, NULL, DECIMAL);
          if (n == 0)
            {
              return FALSE;
            }
        }
      for (int i = 0; i < (int) (strlen (num)) - 1; i++)
        {
          if (isdigit (*(num + i)) == FALSE)
            {
              return FALSE;
            }
        }
    }
  else
    {
      if (strlen (num) == 0)
        {
          return FALSE;
        }
      for (int i = 0; i < (int) (strlen (num)); i++)
        {
          if (isdigit (*(num + i)) == FALSE)
            {
              return FALSE;
            }
        }
    }
  return TRUE;
}

/*
 * Goes through the array and prints out the
 * best student (best grade/age)
 * start - Student pointer to the start of the array
 * end - Student pointer to the end of the array
 */
void best_student (Student *start, Student *end)
{
  float best = -1, check = -1;
  int index = 0, best_index = 0;
  Student *temp;
  temp = start;
  while (temp+index <= end)
    {
      check = ((float) ((temp+index)->grade) / (float) ((temp+index)->age));
      if (check > best)
        {
          best = check;
          best_index = index;
        }
      index++;
    }
  Student *best_student = (start + best_index);
  fprintf (stdout, "best student info is: %lu,%d,%d\n", best_student->id,
           best_student->grade, best_student->age);
}

/*
 * A function that swaps the contents of 2
 * pointers to a Student struct
 */
void swap (Student *a, Student *b)
{
  Student temp_swap;
  temp_swap = *a;
  *a = *b;
  *b = temp_swap;
}

/*
 * Sorts the array from start to end using bubble
 * sort method. Sorts the students by their grade.
 */
void bubble_sort (Student *start, Student *end)
{
  int len = end - start + 1;
  for (int i = 0; i < len - 1; i++)
    {
      for (int j = 0; j < len - 1 - i; j++)
        {
          if ((start + j)->grade > (start + j + 1)->grade)
            {
              swap (start + j, start + j + 1);
            }
        }
    }
  for (int k = 0; k < len; k++)
    {
      fprintf (stdout, "%lu,%d,%d\n", (start + k)->id,
               (start + k)->grade, (start + k)->age);
    }
}

/*
 * A function for quick sort method, partitions
 * by using the first slot as a pivot.
 */
Student *partition (Student *start, Student *end)
{
  Student *pivot = start;
  int start_index = 0, end_index = 0;
  while (start + start_index < end + end_index)
    {
      while ((start + start_index)->age <= pivot->age
             && (start + start_index) <= end)
        {
          start_index++;
        }
      while ((end + end_index)->age > pivot->age && (end + end_index) >= start)
        {
          end_index--;
        }
      if (start + start_index < end + end_index)
        {
          swap (start + start_index, end + end_index);
        }
    }
  if (pivot != end + end_index)
    {
      swap (pivot, end + end_index);
    }
  return end + end_index;
}

/*
 * Recursion helper for the quick sort function
 */
void recursion_helper (Student *start, Student *end)
{
  Student *new_loc;
  if (start < end)
    {
      new_loc = partition (start, end);
      recursion_helper (start, new_loc - 1);
      recursion_helper (new_loc + 1, end);
    }
}

/*
 * Sorts the array from start to end (pointers)
 * by using the quick sort method.
 */
void quick_sort (Student *start, Student *end)
{
  int index = 0;
  recursion_helper (start, end);
  while (start + index <= end)
    {
      fprintf (stdout, "%lu,%d,%d\n", (start + index)->id,
               (start + index)->grade, (start + index)->age);
      index++;
    }
}

/*
 * Another function that handles user input, prints
 * and returns ERROR if there's an error, otherwise
 * returns VALID_INPUT
 */
int handle_input (char (*id), char (*grade), char (*age))
{
  if (*id == '0')
    {
      fprintf (stdout, "%s", ID_ERR_MSG);
      return ID_ERROR;
    }
  else if (strlen (id) != ID_LENGTH)
    {
      fprintf (stdout, "%s", ID_ERR_MSG);
      return ID_ERROR;
    }
  else
    {
      if (is_number_only (id, FALSE, FALSE) == FALSE)
        {
          fprintf (stdout, "%s", ID_ERR_MSG);
          return ID_ERROR;
        }
    }
  if (is_number_only (grade, FALSE, FALSE) == FALSE)
    {
      fprintf (stdout, "%s", GRADE_ERR_MSG);
      return GRADE_ERROR;
    }
  else
    {
      int i_grade = (int) strtol (grade, NULL, DECIMAL);
      if (i_grade < GRADE_LOW || i_grade > GRADE_HIGH)
        {
          fprintf (stdout, "%s", GRADE_ERR_MSG);
          return GRADE_ERROR;
        }
    }
  if (is_number_only (age, TRUE, FALSE) == FALSE)
    {
      fprintf (stdout, "%s", AGE_ERR_MSG);
      return AGE_ERROR;
    }
  else
    {
      int i_age = (int) strtol (age, NULL, DECIMAL);
      if (i_age < AGE_LOW || i_age > AGE_HIGH)
        {
          fprintf (stdout, "%s", AGE_ERR_MSG);
          return AGE_ERROR;
        }
    }
  return VALID_INPUT;
}

/*
 * Checks which input the user gave, returns it.
 * If invalid input, returns INVALID. Also checks
 * that the amount of arguments given by the user
 * equals to 1.(2 with the program name)
 */
int check_args (int argc, char *argv[])
{
  if (argc != 2)
    {
      return INVALID;
    }
  if (strcmp (argv[1], ARG_1) == 0)
    {
      return BEST;
    }
  else if (strcmp (argv[1], ARG_2) == 0)
    {
      return BUBBLE;
    }
  else if (strcmp (argv[1], ARG_3) == 0)
    {
      return QUICK;
    }
  else
    {
      return INVALID;
    }
}

int main (int argc, char *argv[])
{
  int program_arg = check_args (argc, argv);
  if (program_arg == INVALID)
    {
      fprintf (stdout, "%s", USAGE);
      return EXIT_FAILURE;
    }
  int num_students, grade, age, result;
  int counter = 0, check = 0;
  void *test;
  char c_id[MIN_BUFFER_SIZE], c_grade[MIN_BUFFER_SIZE],
      c_age[MIN_BUFFER_SIZE];
  long int id;
  char *ptr;
  char buff[MAX_BUFFER_SIZE];
  while (check == FALSE)
    {
      fprintf (stdout, "Enter number of students. Then enter\n");
      test = fgets (buff, MIN_BUFFER_SIZE, stdin);
      if (test == NULL)
        {
          return EXIT_FAILURE;
        }
      check = is_number_only (buff, TRUE, TRUE);
      if (check == FALSE)
        {
          fprintf (stdout, "%s", STU_NUM_ERR);
        }
    }
  num_students = (int) strtol (buff, NULL, DECIMAL);
  Student *arr_ptr = make_array (num_students);
  if (arr_ptr == FALSE)
    {
      return EXIT_FAILURE;
    }
  while (counter < num_students)
    {
      fprintf (stdout, "Enter student info. Then enter\n");
      test = fgets (buff, MAX_BUFFER_SIZE, stdin);
      if (test == NULL)
        {
          return EXIT_FAILURE;
        }
      ptr = strtok (buff, COMMA);
      if (ptr == NULL)
        {
          fprintf (stdout, "%s", ID_ERR_MSG);
          continue;
        }
      strcpy (c_id, ptr);
      ptr = strtok (NULL, COMMA);
      if (ptr == NULL)
        {
          fprintf (stdout, "%s", GRADE_ERR_MSG);
          continue;
        }
      strcpy (c_grade, ptr);
      ptr = strtok (NULL, COMMA);
      if (ptr == NULL)
        {
          fprintf (stdout, "%s", AGE_ERR_MSG);
          continue;
        }
      strcpy (c_age, ptr);
      result = handle_input (c_id, c_grade, c_age);
      if (result != VALID_INPUT)
        {
          continue;
        }
      id = strtol (c_id, NULL, DECIMAL);
      grade = (int) strtol (c_grade, NULL, DECIMAL);
      age = (int) strtol (c_age, NULL, DECIMAL);
      *(arr_ptr + counter) = create_student (id, grade, age);
      counter++;
    }
  Student *arr_end_ptr = (arr_ptr + num_students - 1);
  if (program_arg == BEST)
    {
      best_student (arr_ptr, arr_end_ptr);
    }
  else if (program_arg == BUBBLE)
    {
      bubble_sort (arr_ptr, arr_end_ptr);
    }
  else if (program_arg == QUICK)
    {
      quick_sort (arr_ptr, arr_end_ptr);
    }
  free (arr_ptr);
  arr_ptr = NULL;
  arr_end_ptr = NULL;
  return EXIT_SUCCESS;
}
