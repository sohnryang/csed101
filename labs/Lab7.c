#include <stdio.h>

int seqSearch(int a[], int size, int elem);

int main() {
  int arr[10];
  printf("Input 10 integer values: ");
  for (int i = 0; i < 10; i++)
    scanf("%d", &arr[i]);
  printf("Entered number is      :");
  for (int i = 0; i < 10; i++)
    printf(" %d", arr[i]);
  printf("\nInput an integer that you want to find: ");
  int elem;
  scanf("%d", &elem);
  int idx = seqSearch(arr, 10, elem);
  if (idx != -1)
    printf("The index of %d is %d\n", elem, idx);
  else
    printf("Not Found\n");
  return 0;
}

int seqSearch(int a[], int size, int elem) {
  for (int i = 0; i < 10; i++)
    if (elem == a[i])
      return i;
  return -1;
}
