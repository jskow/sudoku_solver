CFLAGS=-Wall -g -lpthread

sudoku_solver: sudoku_solver.o sudoku_ui.o
single: sudoku_single_thread.o
		$(CC) sudoku_single_thread.o -o single_thread
multi: sudoku_27_threads.o
		$(CC) $(CFLAGS) sudoku_27_threads.o -g -o multi_thread
clean:
	rm *.o
	rm -rf *.dSYM
	@make clean_sudoku_solver
	@make clean_multi
	@make clean_single

clean_sudoku_solver:
	rm sudoku_solver

clean_multi:
	rm *.o
	rm multi_thread

clean_single:
	rm single_thread
