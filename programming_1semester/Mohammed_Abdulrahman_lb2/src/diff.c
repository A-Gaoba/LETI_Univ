#include<stdio.h>
#include "diff.h"
#include "max.h"
#include "min.h"



int diff_between_max_and_min(int*arr,int diff, int n){
    diff = Max(arr, n) - Min(arr, n);
    return diff;

}
