//Input varying Sudoku arrays to the sudoku solvers
//Check the speed that it takes to validate the arrays
#define NUM_SOL_CHECK 9


int shift_puzzle(int shift_row);
extern int sudoku_solver(int sudoku_array[9][9]);
//[row][col]
//this will give errors in row 0, col 3, subgrid 1
int sudoku_array[9][9] = {{5, 2, 4, 5, 3, 9, 1, 8, 7},
                    {5, 1, 9, 7, 2, 8, 6, 3, 4},
                    {8, 3, 7, 6, 1, 4, 2, 9, 5},
                    {1, 4, 3, 8, 6, 5, 7, 2, 9},
                    {9, 5, 8, 2, 4, 7, 3, 6, 1},
                    {7, 6, 2, 3, 9, 1, 4, 5, 8},
                    {3, 7, 1, 9, 5, 6, 8, 4, 2},
                    {4, 9, 6, 1, 8, 2, 5, 7, 3},
                    {2, 8, 5, 4, 7, 3, 9, 1, 6}};

int main()
{
  int i;
  //pass sudoku to sudoku solvers
  for (i = 0; i < NUM_SOL_CHECK; i++)
  {
    sudoku_solver(sudoku_array);
    shift_puzzle(1);
  }
}

int shift_puzzle(int shift_row)
{
  //shift_row == 1, shift by row
  //shift_col == 0, shift by column
  int i = 0;
  for (i=0;i<9;i++)
  {
    if (shift_row == 1)
    {
      if (i == 8)
      {
        sudoku[i][0] = sudoku[0][0];
      } else {
        sudoku[i][0] = sudoku[i+1][0];
      }
    }
  }
}
