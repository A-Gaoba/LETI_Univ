#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 1000 

void fill_arr(int* ptr, int l){
    for (int i = 0; i < l; i++){
        scanf("%d", ptr + i); 
    }
}



int int_cmp(const void* x, const void* y){

    x = *((int*)x);

    y = *((int*)y);

    if(x == y) return 0;

    if(x < y) return -1;

    if(x > y) return 1;

}



int main(){

  int arr[N];
  fill_arr(arr, N);
  int zero = 0;
    
  qsort(arr, N, sizeof(int), int_cmp);
  

  clock_t start = clock();

  int* ptr = (int*)bsearch(&zero, arr, N, sizeof(int), int_cmp);  

  clock_t end = clock();

  
  if (ptr) puts("exists");
  else puts("doesn't exist");


  printf("%lf\n", (end - start)/(double)CLOCKS_PER_SEC);

  start = clock();


  for (int i = 0; i < N; i++){
      if (arr[i] == 0){
         puts("exists");
         break;}
      if(i == N-1)puts("doesn't exist");}

  

  end = clock();
  printf("%lf\n", (end - start)/(double)CLOCKS_PER_SEC);


  return 0;

}
