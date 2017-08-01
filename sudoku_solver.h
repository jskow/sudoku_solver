//Function declarations
typedef struct {
  int row;
  int col;
  int thread_id;
} sudoku_data_t;

enum {
  RUN_SINGLE_THREAD = 1,
  RUN_27_THREADS,
  RUN_CLEAN,
};

//Global sudoku access
extern int sudoku[9][9];

int check_status_array(void);
//multi-threaded functions
int row_check(void *arg);
int col_check(void *arg);
int grid_check(void *arg);
int sudoku_solver(void);

//single-threaded functions
int row_check_single(void);
int col_check_single(void);
int grid_check_single(void);
int sudoku_single_thread(void);

//27 thread functions
int row_check_27(void *arg);
int col_check_27(void *arg);
int grid_check_27(void *arg);
int check_status_array_27(void);
int sudoku_solver_27(void);

//ui functions
int sudoku_ui(void *arg);

//Test helper functions
void shift_puzzle(int shift_row);
int sudoku_puzzle_print();
int run_sudoku_solver(int in_flag);
