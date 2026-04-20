#include "clogf.h"
#include "stdio.h"

int main() {
  volatile float x = 1.4;
  printf("logf(1.4) = %f\n", logf(x));
  if (logf(x) > 0.5) {
     return 0;
  } else {
     return 1;
  }
}