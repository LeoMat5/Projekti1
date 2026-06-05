To use the program, run these commands:
gcc reverse.c -o reverse -std=c99 -Wall -pedantic

If you are located in the code folder use one of these:
./reverse
./reverse input.txt
./reverse input.txt output.txt

To test for memory leaks:
gcc reverse.c -o reverse -std=c99 -Wall -Wextra -pedantic -g
valgrind --leak-check=full ./reverse

