#include "stdio.h"
#include "unistd.h"
#include "time.h"
#include "sudoku_solver.h"

//[row][col]
//This is a working sudoku solution
int sudoku[9][9] = {{6, 2, 4, 5, 3, 9, 1, 8, 7},
                    {5, 1, 9, 7, 2, 8, 6, 3, 4},
                    {8, 3, 7, 6, 1, 4, 2, 9, 5},
                    {1, 4, 3, 8, 6, 5, 7, 2, 9},
                    {9, 5, 8, 2, 4, 7, 3, 6, 1},
                    {7, 6, 2, 3, 9, 1, 4, 5, 8},
                    {3, 7, 1, 9, 5, 6, 8, 4, 2},
                    {4, 9, 6, 1, 8, 2, 5, 7, 3},
                    {2, 8, 5, 4, 7, 3, 9, 1, 6}};

int main(int argc, char **argv)
{
  int i, input = 0, in_flag=0, ret = 0, num_of_sol_check = 1;
  struct timespec tstart={0,0}, tend={0,0};

  //allow user to choose which solution for testing purposes
  //"-s" runs single thread
  //"-m" runs 27 thread solution
  //"-c" runs only once with the correct solution
  input = getopt(argc, argv, "smcit");
  switch (input)
  {
    case 's':
      printf("Using single threaded solution.\n");
      in_flag = RUN_SINGLE_THREAD;
      break;
    case 'm':
      printf("Using 27 threaded solution.\n");
      in_flag = RUN_27_THREADS;
      break;
    case 'c':
      printf("Only run once with clean solution using 11 threads.\n");
      in_flag = RUN_CLEAN;
      break;
    case 'i':
      printf("For test invalid case using 11 threads.\n");
      sudoku_puzzle_print();
      int column,row,value;
      printf("Enter the column number from 0-8 : ");
      scanf("%d",&column);
      printf("Enter the row number from 0-8 : ");
      scanf("%d",&row);
      printf("Row : %d , Column : %d ",row,column);
      printf("\n current value at %d,%d in sudoku is %d",row,column,sudoku[row][column]);
      printf("\n Enter the value to make the solution invalid : ");
      do{
        scanf("%d",&value);
      }while(value > 8 || value < 0);
      sudoku[row][column] = value;
      printf("\n new value at %d,%d in sudoku is %d\n",row,column,sudoku[row][column]);
      break;
    case 't':
      printf("shifting operations..to verify invalid solution..using 11 threads");
      num_of_sol_check = 19;
      break;
    default:
      printf("Using 11 theaded solution.\n");
      break;
  }

  //Begin timer
  clock_gettime(CLOCK_MONOTONIC, &tstart);
  //pass sudoku to sudoku solvers
  sudoku_puzzle_print();

  //will run 11 thread approach
  if (in_flag == RUN_CLEAN)
  {
      ret = sudoku_solver();
      if (ret != 0)
      {
        printf("Error found in Sudoku solution\n");
      } else {
        printf("Solution is valid.\n");
      }
  } else {
    for (i = 0; i < num_of_sol_check; )
    {
      ret = run_sudoku_solver(in_flag);
      if (ret != 0)
      {
        printf("Error found in Sudoku solution\n");
      } else {
        printf("Solution is valid.\n");
      }
      if (i >= 9)
      {
        //shift each value in row
         shift_puzzle(0);
       } else {
        //shift each value in column
         shift_puzzle(1);
      }
      i++;
      if (i != num_of_sol_check)
      {
        sudoku_puzzle_print();
      }
    }
  }

  clock_gettime(CLOCK_MONOTONIC, &tend);
  printf("Total application runtime is %f seconds\n", ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
                                  ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
}

//helper function to call respective validator function of the selected approach
int run_sudoku_solver(int in_flag)
{
  int ret = 0;

  if (in_flag == RUN_SINGLE_THREAD)
  {
    //run in single thread mode
    ret = sudoku_single_thread();
  } else if (in_flag == RUN_27_THREADS)  {
    //run in 27 threads mode
    ret = sudoku_solver_27();
  } else {
    //run in 11 threads mode
    ret = sudoku_solver();
  }
  return ret;
}
//TBD: Allow user to choose which row/column to shift
//Right now only shifts row 0 and column 0
void shift_puzzle(int shift_row)
{
  //shift_row == 1, shift each value in column
  //shift_col == 0, shift each value in row
  int i = 0, first_val;
  //Save first value, so it isn't overwritten in shift
  first_val = sudoku[0][0];
  for (i=0;i<9;i++)
  {
    if (shift_row == 1)
    {
      if (i == 8)
      {
        sudoku[i][0] = first_val;
      } else {
        sudoku[i][0] = sudoku[i+1][0];
      }
    } else if (shift_row == 0)
    {
      if (i == 8)
      {
        sudoku[0][i] = first_val;
      } else {
        sudoku[0][i] = sudoku[0][i+1];
      }
    }
  }
}

//Print the current Sudoku puzzle
int sudoku_puzzle_print(void)
{
  for (int i=0;i<9;i++)
  {
    for (int j=0;j<9;j++)
    {
        printf("%d ", sudoku[i][j]);
    }
    printf("\n");
  }

  return 0;
}
