#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "time.h"
#include "sudoku_solver.h"

#define NUM_MULTI_THREADS 27
#define DEBUG 0
#define ERR

static int status[NUM_MULTI_THREADS] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                      1, 1, 1, 1, 1};

//Run a thread for every row, column and subgrid
//This will create a total of 27 threads
int sudoku_solver_27(void)
{
    int ret=0;
    struct timespec tstart={0,0}, tend={0,0};
    //Begin timer
    clock_gettime(CLOCK_MONOTONIC, &tstart);

    //Allocate memory for thread
    pthread_t threads[NUM_MULTI_THREADS];
    int grid_idx=0, i, j;
    //keep track of pointers
    int grid_num=0, row_num=0, col_num=0;

    //Allocate on the heap, stack causes some kind of race condition
    sudoku_data_t *grid[9], *row[9], *col[9];
    //create threads for each row,each column and each 3x3 box
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (i%3 == 0 && j%3 == 0)
            {
                grid[grid_num] = malloc(sizeof(sudoku_data_t));

                grid[grid_num]->row = i;
                grid[grid_num]->col = j;
                grid[grid_num]->thread_id = grid_idx;
                pthread_create(&threads[grid_idx], NULL, (void *)grid_check_27, (void *)grid[grid_num]);
                grid_idx++;
                grid_num++;
            }
            if (j == 0)
            {
                row[row_num] = malloc(sizeof(sudoku_data_t));
                row[row_num]->thread_id=grid_idx;
                row[row_num]->row=i;
                row[row_num]->col=j;

                if (pthread_create(&threads[grid_idx], NULL, (void *)row_check_27, (void *)row[row_num]))
                {
                  printf("error in creating row thread\n");
                }
                row_num++;
                grid_idx++;
            }

            if (i == 0)
            {
                col[col_num] = malloc(sizeof(sudoku_data_t));
                col[col_num]->thread_id=grid_idx;
                col[col_num]->row=i;
                col[col_num]->col=j;

                pthread_create(&threads[grid_idx], NULL, (void *)col_check_27, (void *)col[col_num]);
                col_num++;
                grid_idx++;
            }
        }
    }


//Create a thread to check array
//Use default attributes (NULL)
//Need: column_check, subgrid_check, master_check

//join threads
  for (i=0;i<NUM_MULTI_THREADS;i++)
  {
      pthread_join(threads[i], NULL);
  }

  clock_gettime(CLOCK_MONOTONIC, &tend);
  printf("Time to solve is %f seconds\n", ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
                                  ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

//check if correct or not
  ret = check_status_array_27();

  for (i = 0; i<9; i++)
  {
    free(grid[i]);
    free(row[i]);
    free(col[i]);
  }

  for (i=0;i<NUM_MULTI_THREADS;i++)
  {
    status[i] = 1;
  }


//print result
    return ret;
} //end main

int check_status_array_27(void)
{
    int i;
    for (i = 0;i < NUM_MULTI_THREADS; i++)
    {
        if (status[i] == 0)
        {
            //printf("Error in thread %d.\n", i);
            //printf("Solution is not valid\n");
            return 1;
        } else {
            //printf("No errors found.\n");
        }
    }
    return 0;
    //loop through status array, check if valid solution
}

int row_check_27(void *arg)
{
    sudoku_data_t row1 =  *(sudoku_data_t *)arg;
    int i, j, cur_val;
    //point to 1st value, scan rest of the array to see if value repeats
    //check entire row
    //if no values repeat, then change status array
    if (DEBUG)
    {
        printf("test row_check, thread_id %d\n", row1.thread_id);
    }
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

    return 0;
}

int col_check_27(void *arg)
{
    sudoku_data_t col =  *(sudoku_data_t *)arg;
    int i, j, cur_val;
    //point to 1st value, scan rest of the array to see if value repeats
    //check entire row
    //if no values repeat, then change status array
    if (DEBUG)
    {
        printf("test col_check, thread_id %d\n", col.thread_id);
    }
        //check each value in the row
        for (i=0;i<9;i++)
        {
            cur_val = sudoku[i][col.col];
            if(col.col > 8){
                printf("invalid --- %d\n",col.col);
            }
            //printf("col:%d-value:%d,%d  \n",col.col,cur_val,col.thread_id);
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

    return 0;
}


int grid_check_27(void *arg)
{
    sudoku_data_t grid =  *(sudoku_data_t *)arg;
    //printf("arg pointer is %p, grid_check is %p\n", arg, &grid_check);
    //printf("value @ arg is %d\n", *(int *)arg+4);
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

                        printf("Subgrid at Row: %d, Col %d\n", grid.row, grid.col);
                        //return -1;
                    }
                }
            }
        }
    }

    return 0;
}
