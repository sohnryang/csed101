#include <stdio.h>
#include <stdlib.h>

void find_minmax(int *arr, int size, int *max, int *min);
void print_arr(int *arr, int size);
void input_arr(int *arr, int size);

int main() {
  int arr_size, arr_min, arr_max, *arr;
  for (int i = 0; i < 2; i++) {
    if (i == 0)
      printf("Enter array size: ");
    else
      printf("Enter changed array size: ");
    scanf("%d", &arr_size);
    if (i == 0)
      arr = (int *)calloc(arr_size, sizeof(int));
    else
      arr = (int *)realloc(arr, arr_size * sizeof(int));
    input_arr(arr, arr_size);
    print_arr(arr, arr_size);
    find_minmax(arr, arr_size, &arr_max, &arr_min);
    printf("Maximum is %d\n", arr_max);
    printf("Minimum is %d\n", arr_min);
    if (i == 0)
      printf("\n");
  }
  free(arr);
  return 0;
}

void find_minmax(int *arr, int size, int *max, int *min) {
  *max = *min = arr[0];
  for (int i = 0; i < size; i++) {
    *max = arr[i] > *max ? arr[i] : *max;
    *min = arr[i] < *min ? arr[i] : *min;
  }
}

void print_arr(int *arr, int size) {
  printf("Entered numbers are\n");
  for (int i = 0; i < size; i++)
    printf("%d ", arr[i]);
  printf("\n");
}

void input_arr(int *arr, int size) {
  for (int i = 0; i < size; i++)
    scanf("%d", arr + i);
}
