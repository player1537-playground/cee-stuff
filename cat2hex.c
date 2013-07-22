#include <stdio.h>

int main(int argc, char** argv) {
  int a;
  unsigned char c;
  char* array = "0123456789ABCDEF";
  while( (a=getc(stdin)) != EOF ) {
    c = (unsigned char)a;
    printf("%c%c", array[c >> 4], array[c & 0x0F]);
  }
}
