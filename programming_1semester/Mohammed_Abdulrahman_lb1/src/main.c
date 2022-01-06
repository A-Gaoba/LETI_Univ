#include <stdio.h> 
#include <limits.h> 
#include <locale.h> 
 
#define INPUT_SIZE 10000 
#define NOTHING -1 
 
int arr[100]; 
 
int Max(int arr[], int n){ 
 int max = INT_MIN; 
 for (int index = 0; index < n; index++){ 
 if (arr[index] > max) 
 {max = arr[index];}} 
 return max;} 
 
 
 
 
int Min(int arr[], int n){ 
 int min = INT_MAX; 
 for (int index = 0; index < n; index++){ 
 if (arr[index] < min){ 
 min = arr[index];}} 
 return min;} 
 
 
 
int FindIndexFirstMin(int arr[], int n){ 
 int min = Min(arr, n); 
 for (int index = 0; index < n; index++){ 
 if (arr[index] == min){ 
 return index;}} 
 return NOTHING;} 
 
 
 
int SumIndex(int end, int arr[]){ 
 int sum = 0; 
 for (int index = 0; index < end; index++){ 
 sum += arr[index];} 
 return sum;} 
 
 
 
int main(){ 
 char buffer[INPUT_SIZE]; 
 
 fgets(buffer, sizeof(buffer), stdin); 
 char* data = buffer; 
 int n = 0; 
 int t = NOTHING; 
 int number; 
 int x; 
 
 
 if (sscanf(data, "%d%n", &number, &x) == 1){ 
 t = number; 
 data += x; 
 while (sscanf(data, " %d%n", &number, &x) == 1){ 
 arr[n] = number; 
 n++; 
 data += x;}} 
 
 
 
 switch (t) { 
 case 0: { 
 printf("%d\n", Max(arr, n)); 
 break;} 
 case 1: { 
 printf("%d\n", Min(arr, n)); 
 break;} 
 case 2: { 
 printf("%d\n", Max(arr, n) - Min(arr, n)); 
 break;} 
 case 3: { 
 printf("%d\n", SumIndex(FindIndexFirstMin(arr, n), arr)); 
 break;} 
 default: 
 printf("Данные некорректны");} 
 
 
 return 0;}
