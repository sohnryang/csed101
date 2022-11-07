#include <stdio.h>
#define MAX 6

void bubble_sort(int a[], int size);
void swap(int *x, int *y);
void print_array(int a[], int size);
void input_array(int a[], int size);

int main() {
  int ary[MAX];

  printf("Input integer numbers to be sorted: ");
  input_array(ary, MAX);

  printf(" Unsorted: ");
  print_array(ary, MAX);

  printf("------------------------------------\n");
  bubble_sort(ary, MAX);
  printf("------------------------------------\n");

  printf("   Sorted: ");
  print_array(ary, MAX);

  return 0;
}

void bubble_sort(int a[], int size) {
  for (int i = 0; i < size - 1; i++) {
    printf("[step %2d]: ", i + 1);
    for (int j = size - 1; j > i; j--) {
      if (a[j - 1] > a[j])
        swap(&a[j - 1], &a[j]);
    }
    print_array(a, size);
  }
}

void swap(int *x, int *y) {
  int tmp = *x;
  *x = *y;
  *y = tmp;
}

void print_array(int a[], int size) {
  for (int i = 0; i < size; i++)
    printf("  %2d", a[i]);
  printf("\n");
}

void input_array(int a[], int size) {
  for (int i = 0; i < size; i++)
    scanf("%d", &a[i]);
}
