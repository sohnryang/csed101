#include <stdio.h>

#define SIZE 10

void partial_print(int *arr_start, int size);
int partial_sum(int *arr_start, int *arr_end);

int main() {
  printf("Enter 10 elements of array: ");
  int arr[SIZE];
  for (int i = 0; i < SIZE; i++)
    scanf("%d", arr + i);
  printf("Numbers that you enter are:");
  for (int i = 0; i < SIZE; i++)
    printf(" %d", *(arr + i));
  printf("\n");
  printf("Enter the starting and ending index: ");
  int lo, hi;
  scanf("%d %d", &lo, &hi);
  printf("\n");
  int sum = partial_sum(arr + lo, arr + hi);
  printf("Sum from array[%d] to array[%d] is %d\n", lo, hi, sum);
  partial_print(arr + lo, hi - lo + 1);
  return 0;
}

void partial_print(int *arr_start, int size) {
  printf("Partial array output:");
  for (int i = 0; i < size; i++)
    printf(" %d", *(arr_start + i));
  printf("\n");
}

int partial_sum(int *arr_start, int *arr_end) {
  int sum = 0;
  for (int i = 0; i < arr_end - arr_start + 1; i++)
    sum += *(arr_start + i);
  return sum;
}
