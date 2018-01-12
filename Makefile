CC     := gcc
MACROS := 
CFLAGS := -I. -Wundef -Wall -Wextra -O3 $(MACROS)


all:
	@$(CC) $(CFLAGS) bn.c ./tests/golden.c      -o ./build/test_golden
	@$(CC) $(CFLAGS) bn.c ./tests/hand_picked.c -o ./build/test_hand_picked
	@$(CC) $(CFLAGS) bn.c ./tests/load_cmp.c    -o ./build/test_load_cmp
	@$(CC) $(CFLAGS) bn.c ./tests/factorial.c   -o ./build/test_factorial
	@$(CC) $(CFLAGS) bn.c ./tests/randomized.c  -o ./build/test_random
	@#$(CC) $(CFLAGS) bn.c ./tests/rsa.c         -o ./build/test_rsa


test:
	@echo
	@echo ================================================================================
	@./build/test_golden
	@echo ================================================================================
	@./build/test_hand_picked
	@echo ================================================================================
	@./build/test_load_cmp
	@echo ================================================================================
	@python ./scripts/fact100.py
	@./build/test_factorial
	@echo ================================================================================
	@python ./scripts/test_old_errors.py
	@echo ================================================================================
	@#./build/test_rsa
	@#echo ================================================================================
	@python ./scripts/test_rand.py 1000
	@echo ================================================================================
	@echo

clean:
	@rm -f ./build/*


