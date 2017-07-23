#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "time.h"
#include "sudoku_solver.h"

#define NUM_THREADS 27
#define DEBUG 0
#define ERR


// int check_status_array(void);
// int row_check(void *arg);
// int col_check(void *arg);
// int grid_check(void *arg);

//Create Sudoko array
//This is wrong
//[row][col]
//this will give errors in row 0, col 3, subgrid 1
#ifdef ERR
int sudoku[9][9] = {{6, 2, 4, 3, 3, 9, 1, 8, 7},
                    {5, 1, 9, 7, 2, 8, 6, 3, 4},
                    {8, 3, 7, 6, 1, 4, 2, 9, 5},
                    {1, 4, 3, 8, 6, 5, 7, 2, 9},
                    {9, 5, 8, 2, 4, 7, 3, 6, 1},
                    {7, 6, 2, 3, 9, 1, 4, 5, 8},
                    {3, 7, 1, 9, 5, 6, 8, 4, 2},
                    {4, 9, 6, 1, 8, 2, 5, 7, 3},
                    {2, 8, 5, 4, 7, 3, 9, 1, 6}};
#else
int sudoku[9][9] = {{6, 2, 4, 5, 3, 9, 1, 8, 7},
            {5, 1, 9, 7, 2, 8, 6, 3, 4},
            {8, 3, 7, 6, 1, 4, 2, 9, 5},
            {1, 4, 3, 8, 6, 5, 7, 2, 9},
            {9, 5, 8, 2, 4, 7, 3, 6, 1},
            {7, 6, 2, 3, 9, 1, 4, 5, 8},
            {3, 7, 1, 9, 5, 6, 8, 4, 2},
            {4, 9, 6, 1, 8, 2, 5, 7, 3},
            {2, 8, 5, 4, 7, 3, 9, 1, 6}};
#endif

int status[NUM_THREADS] = {1};

int main()
{
    struct timespec tstart={0,0}, tend={0,0};
//Begin timer
    clock_gettime(CLOCK_MONOTONIC, &tstart);

//Allocate memory for thread
    pthread_t threads[NUM_THREADS];
    int grid_idx=0, i, j;

    sudoku_data_t grid[9],row[9],col[9];

    //create threads for each row,each column and each 3x3 box
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (i%3 == 0 && j%3 == 0)
            {
                grid[grid_idx].thread_id=grid_idx;
                grid[grid_idx].row=i;
                grid[grid_idx].col=j;

                pthread_create(&threads[grid_idx], NULL, (void *)grid_check, (void *)&grid[grid_idx]);
                grid_idx++;
            }
            if (j == 0)
            {
                row[grid_idx].thread_id=grid_idx;
                row[grid_idx].row=i;
                row[grid_idx].col=j;

                pthread_create(&threads[grid_idx], NULL, (void *)row_check, (void *)&row[grid_idx]);
                grid_idx++;
            }

            if (i == 0)
            {
                col[grid_idx].thread_id=grid_idx;
                col[grid_idx].row=i;
                col[grid_idx].col=j;

                pthread_create(&threads[grid_idx], NULL, (void *)col_check, (void *)&col[grid_idx]);
                grid_idx++;
            }
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
    printf("Time to solve is %f\n", ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
                                    ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

//check if correct or not
    check_status_array();

    free(row);
    free(col);

//print result
    return 0;
} //end main

int check_status_array(void)
{
    int i;
    for (i = 0;i < NUM_THREADS; i++)
    {
        if (status[i] == 0)
        {
            //printf("Error in thread %d.\n", i);
            printf("Solution is not valid");
            return -1;
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
                    return -1;
                }
            }
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
                    return -1;
                }
            }
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
                        return -1;
                    }
                }
            }
        }
    }
    return 0;
}
