#include "../include/file.h"
#include "../include/libc.h"
#include "../include/strings.h"

char* replace_str(char* str, char* orig, char* rep) {
  char* ret;
  int i, count = 0;
  size_t newlen = strlen(rep);
  size_t oldlen = strlen(orig);
  for (i = 0; str[i] != '\0'; i++) {
    if (strstr(&str[i], orig) == &str[i]) {
      count++;
      i += oldlen - 1;
    }
  }
  ret = malloc(i + count * (newlen - oldlen));
  if (ret == NULL) {
    return str;
  }
  i = 0;
  while (*str) {
    if (strstr(str, orig) == str) {
      strcpy(&ret[i], rep);
      i += newlen;
      str += oldlen;
    } else {
      ret[i++] = *str++;
    }
  }
  ret[i] = '\0';
  return ret;
}

// Function to split the string 'str' using delimiter 'c'
// Stores the split parts in the array 'arr' and returns the number of parts
// Note: The caller is responsible for freeing the memory allocated for the array and its elements.
int split_string(char* str, char c, char*** arr) {
  int count = 1;       // Initialize the number of tokens to 1
  int token_len = 1;   // Initialize the length of the current token to 1
  int i = 0;           // Initialize the index for array of tokens to 0
  char* p = str;       // Pointer to traverse the input string
  char* t;             // Temporary pointer to store the tokens

  // Count the number of tokens based on delimiter 'c'
  while (*p != '\0') {
    if (*p == c) {
      count++;   // Increment the token count when delimiter is found
    }
    p++;
  }

  // Allocate memory for the array to store the split parts
  // Check the return value of malloc before assigning it to *arr
  void* arr_mem = malloc(sizeof(char*) * count);
  if (arr_mem == NULL)
    return 0;   // Return 0 if memory allocation fails
  *arr = arr_mem;

  p = str;
  while (*p != '\0') {
    if (*p == c) {
        // Allocate memory for each token and store it in the array
        // Check the return value of malloc before assigning it to (*arr)[i]
      void* token_mem = malloc(sizeof(char) * token_len);
      if (token_mem == NULL)
        return 0;   // Return 0 if memory allocation fails
      (*arr)[i] = token_mem;
      token_len = 0;
      i++;
    }
    p++;
    token_len++;
  }

  // Allocate memory for the last token and check the return value before assigning it to (*arr)[i]
  void* last_token_mem = malloc(sizeof(char) * token_len);
  if (last_token_mem == NULL)
    return 0;   // Return 0 if memory allocation fails
  (*arr)[i] = last_token_mem;

  i = 0;
  p = str;
  t = ((*arr)[i]);

  // Split the string based on the delimiter 'c'
  while (*p != '\0') {
    if (*p != c && *p != '\0') {
      *t = *p;
      t++;
    } else {
      *t = '\0';  // Null-terminate each token
      i++;
      t = ((*arr)[i]);   // Move to the next token in the array
    }
    p++;
  }

  return count;   // Return the total number of tokens
}


char* read_string(int f) {
  char* string = malloc(sizeof(char) * 65536);
  int c;
  int length = 0;
  if (!string) {
    return string;
  }
  while ((c = fgetc_pointer(f)) != -1) {
    string[length++] = c;
  }
  string[length++] = '\0';

  return realloc(string, sizeof(char) * length);
}

int substring(char* haystack, char* needle) {
  if (strlen(haystack) >= strlen(needle)) {
    for (int i = strlen(haystack) - strlen(needle); i >= 0; i--) {
      int found = 1;
      for (size_t d = 0; d < strlen(needle); d++) {
        if (haystack[i + d] != needle[d]) {
          found = 0;
          break;
        }
      }
      if (found == 1) {
        return i;
      }
    }
  }
  return -1;
}
