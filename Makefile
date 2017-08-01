CFLAGS=-Wall -g
# -lpthread

all: test

# sudoku_solver: sudoku_solver.o sudoku_ui.o
# 	$(CC) $(CFLAGS) sudoku_solver.o sudoku_ui.o -o sudoku_solver
# single: sudoku_single_thread.o
# 		$(CC) $(CFLAGS) sudoku_single_thread.o -o single_thread
# multi: sudoku_27_threads.o
# 		$(CC) $(CFLAGS) sudoku_27_threads.o -g -o multi_thread
test: sudoku_solver.o sudoku_test.o sudoku_single_thread.o sudoku_27_threads.o
	$(CC) $(CFLAGS) sudoku_solver.o sudoku_test.o sudoku_single_thread.o \
									sudoku_27_threads.o -o sudoku_test

clean:
	rm *.o
	rm -rf *.dSYM
	@make clean_test

# clean_sudoku_solver:
# 	rm sudoku_solver
#
# clean_multi:
# 	rm *.o
# 	rm multi_thread
#
# clean_single:
# 	rm single_thread

clean_test:
	rm sudoku_test
