#include "header.hpp"

int binarySearch(int *arr,int low,int high,int x)
{
  while(true){
    if (high >= low) {
      int mid = low + (high - low) / 2;
// If the element is present at the middle
// itself
      if (arr[mid] == x) 
        return mid;
// If element is smaller than mid, then
// it can only be present in left subarray
      if (arr[mid] > x) {
        high = mid - 1;
        continue; 
      }
// Else the element can only be present
// in right subarray
      low = mid + 1;
      continue; 
    }
    return - 1;
  }
}

int process_top(int n,int x,int arr[1024])
{
  
#pragma HLS INTERFACE port=arr ap_memory depth = 1024
  int result = binarySearch(arr,0,n - 1,x);
  return result;
}
