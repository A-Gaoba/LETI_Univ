#include<stdio.h>
#include"min.h"

int Min(int arr[], int n){
    int min= arr[0];
    for(int index = 0; index < n; index++){
        if (arr[index] <min){
            min = arr[index];}}
    return min;}
