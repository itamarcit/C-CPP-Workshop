#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS_IN_SENTENCE_GENERATION 20
#define MAX_WORD_LENGTH 100
#define MAX_SENTENCE_LENGTH 1000
#define TRUE 1
#define FALSE 0
#define FAIL 0
#define DECIMAL 10
#define ALL_FILE -1
#define USAGE_MSG "Usage: tweetsGenerator {num} {num} {path} \
{words}\n"
#define ERROR_MSG "Error: Invalid file path.\n"
#define MEM_ERR_MSG "Allocation failure: Problem allocating \
memory\n"
int allocation_failure = FALSE;
#define VALID_ARG_NUM_1 4
#define VALID_ARG_NUM_2 5

typedef struct WordStruct {
    char *word;
    struct WordProbability *prob_list; // DYNAMIC ARRAY!
    int num_of_occurrences;
} WordStruct;

typedef struct WordProbability {
    struct WordStruct *word_struct_ptr; // SINGLE_WORD_PTR!
    int next_num_occurrences;
    int size;
} WordProbability;

WordProbability *make_word_probability_arr (void)
{

  WordProbability *ptr = (WordProbability *)
      malloc (sizeof (WordProbability));
  if (ptr == NULL)
    {
      printf (MEM_ERR_MSG);
      allocation_failure = TRUE;
      return NULL;
    }
  ptr->size = 0;
  ptr->next_num_occurrences = 0;
  ptr->word_struct_ptr = NULL;
  return ptr;
}

struct WordProbability *add_to_wp_arr (WordProbability *ptr)
{
  WordProbability *temp_ptr = realloc (ptr,
                                       (ptr->size + 1)
                                       * sizeof (WordProbability));
  if (temp_ptr == NULL)
    {
      printf (MEM_ERR_MSG);
      allocation_failure = TRUE;
      return FAIL;
    }
  ptr = temp_ptr;
  temp_ptr = NULL;
  return ptr;
}

/************ LINKED LIST ************/
typedef struct Node {
    WordStruct *data;
    struct Node *next;
} Node;

typedef struct LinkList {
    Node *first;
    Node *last;
    int size;
} LinkList;

/**
 * Add data to new node at the end of the given link list.
 * @param link_list Link list to add data to
 * @param data pointer to dynamically allocated data
 * @return 0 on success, 1 otherwise
 */
int add (LinkList *link_list, WordStruct *data)
{
  Node *new_node = malloc (sizeof (Node));
  if (new_node == NULL)
    {
      return 1;
    }
  *new_node = (Node) {data, NULL};

  if (link_list->first == NULL)
    {
      link_list->first = new_node;
      link_list->last = new_node;
    }
  else
    {
      link_list->last->next = new_node;
      link_list->last = new_node;
    }

  link_list->size++;
  return 0;
}

/*************************************/

/**
 * Checks if given word ends with a '.'
 * @param word
 * @return 1 if it is, 0 otherwise
 */
int is_word_last (char *word)
{
  if (word[strlen (word) - 1] == '.')
    {
      return TRUE;
    }
  return FALSE;
}

/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number
 * @return Random number
 */
int get_random_number (int max_number)
{
  return (rand () % (max_number));
}

/**
 * Choose randomly the next word from the given dictionary, drawn uniformly.
 * The function won't return a word that end's in full stop '.' (Nekuda).
 * @param dictionary Dictionary to choose a word from
 * @return WordStruct of the chosen word
 */
WordStruct *get_first_random_word (LinkList *dictionary)
{
  Node *node = dictionary->first;
  int random;
  random = get_random_number (dictionary->size);
  for (int i = 0; i < random; ++i)
    {
      node = node->next;
    }
  while (is_word_last (node->data->word) == TRUE)
    {
      node = dictionary->first;
      random = get_random_number (dictionary->size - 1);
      for (int i = 0; i < random; ++i)
        {
          node = node->next;
        }
    }
  return (node->data);
}

/**
 * Choose randomly the next word. Depend on it's occurrence frequency
 * in word_struct_ptr->WordProbability.
 * @param word_struct_ptr WordStruct to choose from
 * @return WordStruct of the chosen word
 */
WordStruct *get_next_random_word (WordStruct *word_struct_ptr)
{
  WordStruct *result = word_struct_ptr;
  int random, wp_size, current_occurrences, sum = 0;
  int prev = 1;
  wp_size = word_struct_ptr->prob_list->size;
  random = get_random_number (word_struct_ptr->num_of_occurrences);
  random += 1;
  for (int i = 0; i < wp_size; ++i)
    {
      current_occurrences = (word_struct_ptr->prob_list
                             + i)->next_num_occurrences;
      sum += current_occurrences;
      if (prev <= random && random <= sum)
        {
          result = (word_struct_ptr->prob_list + i)->word_struct_ptr;
          break;
        }
      else
        {
          prev += current_occurrences;
        }
    }
  return result;
}

/**
 * Receive dictionary, generate and print to stdout random sentence out of it.
 * The sentence most have at least 2 words in it.
 * @param dictionary Dictionary to use
 * @return Amount of words in printed sentence
 */
int generate_sentence (LinkList *dictionary)
{
  int words_in_sentence = 1;
  WordStruct *first_word = get_first_random_word (dictionary);
  WordStruct *next_word = first_word;
  printf ("%s", first_word->word);
  while (1 == 1)
    {
      next_word = get_next_random_word (next_word);
      printf (" %s", next_word->word);
      words_in_sentence++;
      if (is_word_last (next_word->word) == TRUE
          || words_in_sentence == MAX_WORDS_IN_SENTENCE_GENERATION)
        {
          break;
        }
    }
  printf ("\n");
  return words_in_sentence;
}

/**
 * Gets 2 WordStructs. If second_word in first_word's prob_list,
 * update the existing probability value.
 * Otherwise, add the second word to the prob_list of the first word.
 * @param first_word
 * @param second_word
 * @return 0 if already in list, 1 otherwise.
 */
int add_word_to_probability_list (WordStruct *first_word, // current now
                                  WordStruct *second_word)
{
  char *word_one, *word_two; // pl=probability list
  int index = 1;
  WordProbability *word_one_pl;
  word_one_pl = first_word->prob_list;
  int pl_size = first_word->prob_list->size;
  word_two = second_word->word;
  while (index <= pl_size)
    {

      if (word_one_pl->word_struct_ptr != NULL)
        {
          word_one = (word_one_pl + index - 1)->word_struct_ptr->word;
        }
      else
        {
          break;
        }
      if (strcmp (word_one, word_two)
          == 0) // if word_one prob list has word_two in it
        {
          (word_one_pl + index - 1)->next_num_occurrences++;
          return 0;
        }
      index++;
    }
  word_one_pl = add_to_wp_arr (word_one_pl);
  first_word->prob_list = word_one_pl;
  (word_one_pl + word_one_pl->size)->word_struct_ptr = second_word;
  (word_one_pl + word_one_pl->size)->next_num_occurrences = 1;
  (word_one_pl + word_one_pl->size)->size = 0;
  word_one_pl->size++;
  return 1;
}

/**
 * Go over the dictionary and check if it contains a Node with a WordStruct
 * containing the word equal to the word parameter.
 * @param dictionary
 * @param word
 * @return index of the word if in the dictionary, -1 otherwise.
 */
int is_in_dict (LinkList *dictionary, char *word)
{
  int i = 0;
  Node *first = dictionary->first;
  while (first != NULL)
    {
      if (strcmp ((first)->data->word, word) == 0) // if equal
        {
          return i;
        }
      first = first->next;
      i++;
    }
  return -1;
}

/**
 * Read word from the given file. Add every unique word to the dictionary.
 * Also, at every iteration, update the prob_list of the previous word with
 * the value of the current word.
 * @param fp File pointer
 * @param words_to_read Number of words to read from file.
 *                      If value is bigger than the file's word count,
 *                      or if words_to_read == -1 than read entire file.
 * @param dictionary Empty dictionary to fill
 */
void fill_dictionary (FILE *fp, int words_to_read, LinkList *dictionary)
{
  Node *current_node = NULL;
  WordStruct *temp_word_struct;
  Node *this_node, *prev_node;
  int total_words_read = 0, words_read = 0, line_index = 0;
  int dict_index, is_last;
  int is_prev_last = FALSE;
  char big_buffer[MAX_SENTENCE_LENGTH];
  const char space[2] = " ";
  char *token;
  while (fgets (big_buffer, MAX_SENTENCE_LENGTH, fp) != NULL)
    {
      big_buffer[strcspn (big_buffer, "\n")] = 0;
      token = strtok (big_buffer, space);
      while (token != NULL
             && (words_to_read == ALL_FILE
                 || words_to_read > total_words_read))
        {
          is_last = is_word_last (token);
          dict_index = is_in_dict (dictionary, token);
          if (dict_index == -1)
            {
              temp_word_struct = (WordStruct *) malloc (sizeof (WordStruct));
              if (temp_word_struct == NULL)
                {
                  printf (MEM_ERR_MSG);
                  allocation_failure = TRUE;
                  return;
                }
              temp_word_struct->word =
                  (char *) malloc (sizeof (char) * MAX_WORD_LENGTH + 1);
              if (temp_word_struct->word == NULL)
                {
                  printf (MEM_ERR_MSG);
                  allocation_failure = TRUE;
                  return;
                }
              strcpy (temp_word_struct->word, token);
              temp_word_struct->num_of_occurrences = 1;
              temp_word_struct->prob_list = NULL;
              if (is_last == FALSE) // comment
                {
                  temp_word_struct->prob_list = make_word_probability_arr ();
                }
              else
                {
                  temp_word_struct->prob_list = NULL;
                }
              if (allocation_failure == TRUE)
                {
                  return;
                }
              if (add (dictionary, temp_word_struct) == 1)
                {
                  allocation_failure = TRUE;
                  return;
                }
              if (total_words_read == 0)
                {
                  this_node = dictionary->first;
                }
              prev_node = this_node;
              this_node = dictionary->last;
            }
          else
            {
              current_node = dictionary->first;
              for (int i = 0; i < dict_index; ++i)
                {
                  current_node = current_node->next;
                }
              current_node->data->num_of_occurrences++;
              prev_node = this_node;
              this_node = current_node;
            }
          if (is_prev_last == FALSE && total_words_read != 0)
            {
              add_word_to_probability_list (prev_node->data, this_node->data);
            }
          words_read++;
          total_words_read++;
          is_prev_last = is_last;
          token = strtok (NULL, space);
        }
      line_index++;
      words_read = 0;
    }
}

/**
 * Free the given dictionary and all of it's content from memory.
 * @param dictionary Dictionary to free
 */
void free_dictionary (LinkList *dictionary)
{
  Node *node_index = dictionary->first;
  Node *new_node;
  while (node_index != NULL)
    {
      free (node_index->data->word);
      free (node_index->data->prob_list);
      free (node_index->data);
      new_node = node_index->next;
      free (node_index);
      node_index = new_node;
    }
  free (dictionary);
}

int arg_check (int argc)
{
  if (argc != VALID_ARG_NUM_1 && argc != VALID_ARG_NUM_2)
    {
      return FALSE;
    }
  return TRUE;
}

/**
 * @param argc
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 *             3) Path to file
 *             4) Optional - Number of words to read
 */
int main (int argc, char *argv[])
{
  if (arg_check (argc) == FALSE)
    {
      printf (USAGE_MSG);
      return EXIT_FAILURE;
    }
  char *path = argv[3];
  int amount_of_tweets = (int) strtol (argv[2], NULL, DECIMAL);
  FILE *txt_file_ptr = fopen (path, "r");
  if (txt_file_ptr == NULL)
    {
      printf ("%s", ERROR_MSG);
      return EXIT_FAILURE;
    }
  int words_to_read = ALL_FILE;
  if (argv[4] != NULL)
    {
      words_to_read = (int) strtol (argv[4], NULL, DECIMAL);
    }
  unsigned int seed = (unsigned int) strtol
      (argv[1], NULL, DECIMAL);
  srand (seed);
  LinkList *word_dictionary = malloc (sizeof (LinkList));
  if (word_dictionary == NULL)
    {
      return EXIT_FAILURE;
    }
  word_dictionary->first = NULL;
  word_dictionary->last = NULL;
  word_dictionary->size = 0;
  fill_dictionary (txt_file_ptr, words_to_read, word_dictionary);
  if (allocation_failure == TRUE)
    {
      return EXIT_FAILURE;
    }
  fclose (txt_file_ptr);
  for (int i = 0; i < amount_of_tweets; ++i)
    {
      printf ("Tweet %d: ", i + 1);
      generate_sentence (word_dictionary);
    }
  free_dictionary (word_dictionary);
  word_dictionary = NULL;
  return 0;
}