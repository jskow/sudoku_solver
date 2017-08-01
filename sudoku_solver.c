#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "time.h"
#include "sudoku_solver.h"

#define NUM_THREADS 11
#define DEBUG 0

//initialize to 1
static int status[NUM_THREADS] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

int sudoku_solver(void)
{
int ret = 0;
struct timespec tstart={0,0}, tend={0,0};
//Begin timer
clock_gettime(CLOCK_MONOTONIC, &tstart);

//Allocate memory for thread
pthread_t threads[NUM_THREADS];
int grid_idx=0, i, j;
sudoku_data_t *row, *col;
sudoku_data_t grid[9];
//Create threads to check each row, column and subgrid
for (i=0;i<9;i+=3)
{
  for (j=0;j<9;j+=3)
  {
    if (i==0 && j==0)
    {
      row = malloc(sizeof(sudoku_data_t));
      col = malloc(sizeof(sudoku_data_t));
      row->row = 0;
      row->col = 0;
      col->row = 0;
      col->col = 1;
      //first thread in status array
      row->thread_id=0;
      col->thread_id=1;
      //spawn threads to check rows & columns
      pthread_create(&threads[0], NULL, (void *)row_check, (void *)row);
      pthread_create(&threads[1], NULL, (void *)col_check, (void *)col);
    }
    grid[grid_idx].thread_id=grid_idx+2;
    grid[grid_idx].row=i;
    grid[grid_idx].col=j;

    pthread_create(&threads[grid_idx+2], NULL, (void *)grid_check, (void *)&grid[grid_idx]);
    grid_idx++;
  }
}


//Create a thread to check array
//Use default attributes (NULL)
//Need: column_check, subgrid_check, master_check

//join threads
for (i=0;i<NUM_THREADS;i++)
{
  pthread_join(threads[i], NULL);
}

clock_gettime(CLOCK_MONOTONIC, &tend);
printf("Time to solve is %f seconds\n", ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
                                ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

//check if correct or not
//if there is an error, ret returns 1
ret = check_status_array();
for (i=0;i<NUM_THREADS;i++)
{
  status[i] = 1;
}

//sudoku_ui((void *)row);
free(row);
free(col);


//print result
return ret;
} //end main

int check_status_array(void)
{
  int i;
  for (i = 0;i < NUM_THREADS; i++)
  {
    if (status[i] == 0)
    {
      //printf("Error in thread %d.\n", i);
      //return -1;
      return 1;
    } else {
      //printf("No errors found.\n");
    }
  }
  return 0;
  //loop through status array, check if valid solution
}

int row_check(void *arg)
{
   sudoku_data_t row1 =  *(sudoku_data_t *)arg;
   int i, j, cur_val, idx;
   //point to 1st value, scan rest of the array to see if value repeats
   //check entire row
  //if no values repeat, then change status array
  if (DEBUG)
  {
   printf("test row_check, thread_id %d\n", row1.thread_id);
  }
   //check all the rows
   for(idx=0;idx<9;idx++)
   {
     row1.row=idx;
     //check each value in the row
     for (i=0;i<9;i++)
     {
       cur_val = sudoku[row1.row][i];
       //check it against all other values
       for (j=0;j<9;j++)
       {
          if ((cur_val == sudoku[row1.row][j]) && (j != i))
          {
            printf("Error in row %d\n", row1.row);
            status[row1.thread_id] = 0;
            //return -1;
          }
       }
     }
     //printf("row %d passed\n",idx);
  }

  return 0;
}

int col_check(void *arg)
{
   sudoku_data_t col =  *(sudoku_data_t *)arg;
   int i, j, cur_val, idx;
   //point to 1st value, scan rest of the array to see if value repeats
   //check entire row
  //if no values repeat, then change status array
  if (DEBUG)
  {
   printf("test col_check, thread_id %d\n", col.thread_id);
  }
   //check all the rows
   for(idx=0;idx<9;idx++)
   {
     col.col=idx;
     //check each value in the row
     for (i=0;i<9;i++)
     {
       cur_val = sudoku[i][col.col];
       //check it against all other values
       for (j=0;j<9;j++)
       {
          if ((cur_val == sudoku[j][col.col]) && (j != i))
          {
            printf("Error in col %d\n", col.col);
            status[col.thread_id] = 0;
            //return -1;
          }
       }
     }
     //printf("col %d passed\n",idx);
  }

  return 0;
}


int grid_check(void *arg)
{
   sudoku_data_t grid =  *(sudoku_data_t *)arg;
   int i, j, cur_val, idx, idx2;
   //point to 1st value, scan rest of the array to see if value repeats
   //check entire row
  //if no values repeat, then change status array
   if (DEBUG)
   {
   printf("test grid_check, thread_id %d\n", grid.thread_id);
   }
   //check subgrid
   for (idx=grid.row; idx<grid.row+3; idx++)
   {
     for (idx2=grid.col; idx2<grid.col+3; idx2++)
     {
       cur_val = sudoku[idx][idx2];
       for (i=grid.row; i<grid.row+3; i++)
       {
         for (j=grid.col; j<grid.col+3; j++)
         {
           if ((cur_val == sudoku[i][j]) && ((idx != i) || (idx2 != j)))
           {
             status[grid.thread_id] = 0;
             printf("Subgrid %d failed\n", grid.thread_id-2);
             printf("Subgrid row %d, col %d\n", grid.row, grid.col);
             //return -1;
           }
         }
       }
      }
    }
  return 0;
}
