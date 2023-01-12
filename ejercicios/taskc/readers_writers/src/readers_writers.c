#include <omp.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  size_t count = 0;
  char ch = '\0';
  #pragma omp parallel shared(count)
  {
    while (scanf("%c", &ch) == 1) {
      if (toupper(ch) == 'R') {
        printf("Reader got %zu\n", count);
      } else if (toupper(ch) == 'W') {
        #pragma omp critical
        count++;
        printf("Writer increased to %zu\n", count);
      }  // Silently ignoring other chars
    }
  }
  return 0;
}
