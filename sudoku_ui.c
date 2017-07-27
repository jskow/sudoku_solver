//Allow users to change row/col values on demand
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

static int check_valid_num(int input);

int sudoku_ui(void *arg)
{
  // int argc, in;
  // char **argv;
  char *input, *token;
  //Allow 8 characters to input
  input = malloc(15);
  //token = malloc(8);
  printf("Enter q to quit this application, h for help\n");
  //Always listen for input from users

  fgets(input, 15, stdin);
  //Format is: r # c # v #
  //If "q", quit application
  printf("input value is %s\n", input);
  //get all values from line
  int read_trys = 0;
  while ((strcmp(token, "\r\0")) || (read_trys > 10))
  {
    token = strtok(input, " ");
    switch (token[0])
    {
      case 'r':
        token = strtok(NULL, " ");
        if (check_valid_num(atoi(token))) {
          printf("Invalid value entered\n");
          return -1;
        }
        printf("Row: ");
        printf("%s\n", token);
        break;
      case 'c':
        token = strtok(NULL, " ");
        if (check_valid_num(atoi(token))) {
          printf("Invalid value entered\n");
          return -1;
        }
        printf("Column: ");
        printf("%s\n", token);
        break;
      case 'v':
        token = strtok(NULL, " ");
        if (check_valid_num(atoi(token))) {
          printf("Invalid value entered\n");
          return -1;
        }
        printf("Value: ");
        printf("%s\n", token);
        break;
      case 'h':
        printf("Valid syntax is \"r <row_value> c <column_value>\"\n");
        break;
      case 'q':
        printf("exit application.\n");
        return 0;
      default:
        printf("found nothing\n");
    }
    read_trys++;
  }

  free(input);

  return 0;
}

static int check_valid_num(int input)
{
  printf("Input is %d\n", input);
  if ((input < 1) || (input > 9))
  {
    //invalid number for sudoku puzzle
    return -1;
  } else {
    return 0;
  }
}
