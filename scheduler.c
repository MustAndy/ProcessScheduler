#include "header.h"

// void bubbleSort(int arr[]) {
//     if(arr == null || arr.length == 0)
//         return ;
//     for(int i=0; i<arr.length-1; i++) {
//         for(int j=arr.length-1; j>i; j--) {
//             if(arr[j] &lt; arr[j-1]) {
//                 swap(arr, j-1, j);
//             }
//         }
//     }
// }
// void swap(int arr[], int i, int j) {
// int temp=arr[i];
//     arr[i] = arr[j];
//     arr[j] = temp;
// }
void scheduler()
{
    int a[3] = { 0, 4, 2 }, b[3] = { 4, 2, 3 };
    char c[3][10] = { "aaa", "bbb", "ccc" };

    JOB Joblist[3];
    for (int i = 0; i < 3; i++)
    {
        Joblist[i] = (JOB)malloc(sizeof(struct _Job));
        Joblist[i]->ArrivalTime = a[i];
        Joblist[i]->DurationTime = b[i];
        strcpy(Joblist[i]->JobContent, c[i]);
    }
}