sudoku_solver: sudoku_solver.o sudoku_ui.o
single: sudoku_single_thread.o
		$(CC) sudoku_single_thread.o -o single_thread
multi: sudoku_27_threads.o
		$(CC) sudoku_27_threads.o -o multi_thread
clean:
	rm *.o
	@make clean_sudoku_solver
	@make clean_multi
	@make clean_single

clean_sudoku_solver:
	rm sudoku_solver

clean_multi:
	rm multi

clean_single:
	rm single
