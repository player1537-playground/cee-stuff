#include <stdio.h>
#include <string.h>

#define GRADIENTS 4
#define MAXLETTERS 30

typedef struct {
  char string[MAXLETTERS];
  int letters;
} data;

int main(int argc, char** argv) {
  int a, b, c, x = 0, y = 0;
  data gradients[GRADIENTS] = {
    { "@8OCoc:.", 8 },
    { "@8Oo:.", 6 },
    { "MWNHKQDXS65YJtjci=+>!;:~'.", 25 },
    { "MHb6j+:.", 8 }
  };
  if (argc != 3 && argc != 4) {
    printf("Usage: %s [width height [gradient]]\n\n", argv[0]);
    for (a=0; a<GRADIENTS; a++)
      printf("Gradient %d is: %s\n", a, gradients[a].string);
    return 1;
  }  int width = atoi(argv[1]);
  int height = atoi(argv[2]);
  char* letters;
  int factor;
  if (argv[3] == NULL) {
    letters = gradients[0].string;
    factor = 255 / (gradients[0].letters - 1);
  } else if (argv[3][0] == '-') {
    if (strlen(argv[3] + 1) > 1) {
      letters = argv[3] + 1;
      factor = 255 / (strlen(letters) - 1);
    } else {
      printf("Choose a gradient with more than 1 character");
    }
  } else {
    a = atoi(argv[3]);
    if (a >= 0 && a < GRADIENTS) {
      letters = gradients[a].string;
      factor = 255 / (gradients[a].letters -1);
    } else {
      printf("Choose a gradient between 0 and %d\n", GRADIENTS - 1);
      return 1;
    }
  }
  while (scanf("%d%d%d", &a, &b, &c) != EOF) {
    printf("%c", letters[(a + a + b + c + c + c) / 6 / factor]);
    x++;
    if (x == width) {
      y++;
      x = 0;
      printf("\n");
    }
  }
}
