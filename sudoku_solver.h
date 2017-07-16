//Function declarations
typedef struct {
  int row;
  int col;
  int thread_id;
} sudoku_data_t;

int check_status_array(void);
int row_check(void *arg);
int col_check(void *arg);
int grid_check(void *arg);

int row_check_single(void);
int col_check_single(void);
int grid_check_single(void);