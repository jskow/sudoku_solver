#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "time.h"
#include "sudoku_solver.h"

#define DEBUG 0

int sudoku_single_thread(void)
{
int rc=0;
struct timespec tstart={0,0}, tend={0,0};
//Begin timer
clock_gettime(CLOCK_MONOTONIC, &tstart);

rc |= row_check_single();
rc |= col_check_single();
rc |= grid_check_single();

clock_gettime(CLOCK_MONOTONIC, &tend);
printf("Time to solve is %f seconds\n", ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
                                ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

//print result
return rc;
} //end main

int row_check_single(void)
{
   sudoku_data_t row1;
   int ret=0;
   row1.row = 0;
   row1.col = 0;
   //not needed
   row1.thread_id = 0;
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
            ret = 1;
          }
       }
     }
     //printf("row %d passed\n",idx);
  }

  return ret;
}

int col_check_single(void)
{
   sudoku_data_t col;
   col.row=0;
   col.col=0;
   col.thread_id=0;
   int i, j, cur_val, idx, ret=0;
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
            ret=1;
          }
       }
     }
     //printf("col %d passed\n",idx);
  }

  return ret;
}


int grid_check_single(void)
{
   sudoku_data_t grid;
   int row[9] = {0, 0, 0, 3, 3, 3, 6, 6, 6};
   int col[9] = {0, 3, 6, 0, 3, 6, 0, 3, 6};
   grid.row=0;
   grid.col=0;
   grid.thread_id=0;
   int i, j, cur_val, idx, idx2, grid_num, ret=0;
   //point to 1st value, scan rest of the array to see if value repeats
   //check entire row
  //if no values repeat, then change status array
   if (DEBUG)
   {
   printf("test grid_check, thread_id %d\n", grid.thread_id);
   }
   for(grid_num=0; grid_num<9; grid_num++)
   {

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
                 printf("grid.row %d, grid.col %d\n", grid.row, grid.col);
                 printf("Subgrid %d failed\n", grid_num);
                 ret=1;
               }
             }
           }
          }
        }
        grid.row = row[grid_num++];
        grid.col = col[grid_num++];
    }
  return ret;
}
