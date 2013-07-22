#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
  int a, b, c;
  int lines = 0;
  int avg = 0;
  int verbose = 0;
  
  if (argc == 2)
    verbose = 1;

  while (scanf("%d%d%d", &a, &b, &c) != EOF) {
    avg += (a+a+b+c+c+c) / 8;
    lines++;
    if (verbose)
      printf("avg = %d\n", avg);
  }
  printf("%d\n", avg / lines);
}
