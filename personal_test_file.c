#include <stdio.h>


int main() {
printf("Problem 1.1 Test Results: \n");

  // will print out numbers between 1-10 inclusive
  for (int i = 1; i <= 10; i++) {
    printf("%d\n", i);
  }

//  1.1 test code here ...

// 1.2 Modify 1.1 to print (after the 10 ints):
//   a) sum of the ints.  b) sum of the ints squared.

printf("Problem 1.2 Test Results: \n");
  int sum_squares = 0;
  int total_sum = 0;
  for (int i = 1; i <= 10; i++) {
    printf("%d\n", i);
    total_sum += i;
    sum_squares += i * i;
  }
  printf("%d %d\n", total_sum, sum_squares);

  return 0;
}