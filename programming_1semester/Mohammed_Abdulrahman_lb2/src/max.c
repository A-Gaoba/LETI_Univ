#include<stdio.h>

#include"max.h"



int Max(int arr[], int n){
    int max = arr[0] ;
    for (int index = 0; index < n; index++){
        if (arr[index] > max){max = arr[index];}}

    return max;}
