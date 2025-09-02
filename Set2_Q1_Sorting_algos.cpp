#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Utility function to print array
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

// 1. Insertion Sort
void insertion_sort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
}

// 2. Bubble Sort
void bubble_sort(int arr[], int n) {
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-i-1; j++)
            if (arr[j] > arr[j+1]) {
                int temp = arr[j]; arr[j] = arr[j+1]; arr[j+1] = temp;
            }
}

// 3. Selection Sort
void selection_sort(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        int min_idx = i;
        for (int j = i+1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;
        int temp = arr[i]; arr[i] = arr[min_idx]; arr[min_idx] = temp;
    }
}

// 4. Shell Sort
void shell_sort(int arr[], int n) {
    for (int gap = n/2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i], j;
            for (j = i; j >= gap && arr[j-gap] > temp; j -= gap)
                arr[j] = arr[j-gap];
            arr[j] = temp;
        }
    }
}

// 5. Quick Sort
int partition(int arr[], int low, int high) {
    int pivot = arr[high], i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            int temp = arr[i]; arr[i] = arr[j]; arr[j] = temp;
        }
    }
    int temp = arr[i+1]; arr[i+1] = arr[high]; arr[high] = temp;
    return i+1;
}
void quick_sort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort(arr, low, pi-1);
        quick_sort(arr, pi+1, high);
    }
}

// 6. Merge Sort
void merge(int arr[], int l, int m, int r) {
    int n1 = m-l+1, n2 = r-m;
    int L[n1], R[n2];
    for (int i = 0; i < n1; i++) L[i] = arr[l+i];
    for (int j = 0; j < n2; j++) R[j] = arr[m+1+j];
    int i=0, j=0, k=l;
    while (i < n1 && j < n2) arr[k++] = (L[i]<=R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}
void merge_sort(int arr[], int l, int r) {
    if (l < r) {
        int m = l+(r-l)/2;
        merge_sort(arr, l, m);
        merge_sort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}

// 7. Heap Sort
void heapify(int arr[], int n, int i) {
    int largest = i, l = 2*i+1, r = 2*i+2;
    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest != i) {
        int temp = arr[i]; arr[i] = arr[largest]; arr[largest] = temp;
        heapify(arr, n, largest);
    }
}
void heap_sort(int arr[], int n) {
    for (int i = n/2-1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n-1; i > 0; i--) {
        int temp = arr[0]; arr[0] = arr[i]; arr[i] = temp;
        heapify(arr, i, 0);
    }
}

// 8. Radix Sort (using Counting Sort subroutine)
int getMax(int arr[], int n) {
    int mx = arr[0];
    for (int i = 1; i < n; i++) if (arr[i] > mx) mx = arr[i];
    return mx;
}
void counting_sort_radix(int arr[], int n, int exp) {
    int output[n], count[10] = {0};
    for (int i = 0; i < n; i++) count[(arr[i]/exp)%10]++;
    for (int i = 1; i < 10; i++) count[i] += count[i-1];
    for (int i = n-1; i >= 0; i--) {
        output[count[(arr[i]/exp)%10]-1] = arr[i];
        count[(arr[i]/exp)%10]--;
    }
    for (int i = 0; i < n; i++) arr[i] = output[i];
}
void radix_sort(int arr[], int n) {
    int m = getMax(arr, n);
    for (int exp = 1; m/exp > 0; exp *= 10)
        counting_sort_radix(arr, n, exp);
}

// 9. Counting Sort
void counting_sort(int arr[], int n) {
    int max = arr[0];
    for (int i=1; i<n; i++) if (arr[i]>max) max = arr[i];
    int count[max+1]; memset(count, 0, sizeof(count));
    for (int i=0; i<n; i++) count[arr[i]]++;
    int idx = 0;
    for (int i=0; i<=max; i++)
        while (count[i]--) arr[idx++] = i;
}

// 10. Bucket Sort
void insertion_for_bucket(int arr[], int n) {
    for (int i=1; i<n; i++) {
        int key = arr[i], j=i-1;
        while (j>=0 && arr[j]>key) { arr[j+1]=arr[j]; j--; }
        arr[j+1]=key;
    }
}
void bucket_sort(int arr[], int n) {
    int max = arr[0], min = arr[0];
    for (int i=1;i<n;i++){ if(arr[i]>max) max=arr[i]; if(arr[i]<min) min=arr[i]; }
    int bucketRange = (max-min)/n + 1;
    int buckets[n][n], count[n]; memset(count, 0, sizeof(count));
    for (int i=0;i<n;i++) {
        int idx = (arr[i]-min)/bucketRange;
        buckets[idx][count[idx]++] = arr[i];
    }
    int k=0;
    for (int i=0;i<n;i++) {
        insertion_for_bucket(buckets[i], count[i]);
        for (int j=0;j<count[i];j++) arr[k++] = buckets[i][j];
    }
}

// 11. Radix Exchange Sort (MSD radix sort on binary)
void radix_exchange_sort(int arr[], int left, int right, int bit) {
    if (left >= right || bit < 0) return;
    int i = left, j = right;
    while (i <= j) {
        while (i <= j && ((arr[i]>>bit)&1)==0) i++;
        while (i <= j && ((arr[j]>>bit)&1)==1) j--;
        if (i < j) { int temp=arr[i]; arr[i]=arr[j]; arr[j]=temp; }
    }
    radix_exchange_sort(arr, left, j, bit-1);
    radix_exchange_sort(arr, i, right, bit-1);
}

// 12. Address Calculation Sort (Hash-based bucket sort)
void address_calculation_sort(int arr[], int n) {
    if (n==0) return;
    int min=arr[0], max=arr[0];
    for (int i=1;i<n;i++){ if(arr[i]<min) min=arr[i]; if(arr[i]>max) max=arr[i]; }
    int buckets[n][n], count[n]; memset(count,0,sizeof(count));
    for (int i=0;i<n;i++) {
        int idx = (int)((double)(arr[i]-min)/(max-min+1)*n);
        buckets[idx][count[idx]++] = arr[i];
    }
    int k=0;
    for (int i=0;i<n;i++) {
        insertion_for_bucket(buckets[i], count[i]);
        for (int j=0;j<count[i];j++) arr[k++] = buckets[i][j];
    }
}

// Main
int main() {
    int arr[] = {29,10,14,37,13,2,9,25};
    int n = sizeof(arr)/sizeof(arr[0]);

    int copy[20];

    memcpy(copy, arr, sizeof(arr)); insertion_sort(copy,n); printf("Insertion Sort : "); printArray(copy,n);
    memcpy(copy, arr, sizeof(arr)); bubble_sort(copy,n);   printf("Bubble Sort    : "); printArray(copy,n);
    memcpy(copy, arr, sizeof(arr)); selection_sort(copy,n);printf("Selection Sort : "); printArray(copy,n);
    memcpy(copy, arr, sizeof(arr)); shell_sort(copy,n);    printf("Shell Sort     : "); printArray(copy,n);
    memcpy(copy, arr, sizeof(arr)); quick_sort(copy,0,n-1);printf("Quick Sort     : "); printArray(copy,n);
    memcpy(copy, arr, sizeof(arr)); merge_sort(copy,0,n-1);printf("Merge Sort     : "); printArray(copy,n);
    memcpy(copy, arr, sizeof(arr)); heap_sort(copy,n);     printf("Heap Sort      : "); printArray(copy,n);
    memcpy(copy, arr, sizeof(arr)); radix_sort(copy,n);    printf("Radix Sort     : "); printArray(copy,n);
    memcpy(copy, arr, sizeof(arr)); counting_sort(copy,n); printf("Counting Sort  : "); printArray(copy,n);
    memcpy(copy, arr, sizeof(arr)); bucket_sort(copy,n);   printf("Bucket Sort    : "); printArray(copy,n);
    memcpy(copy, arr, sizeof(arr)); radix_exchange_sort(copy,0,n-1,31); printf("Radix Exchange : "); printArray(copy,n);
    memcpy(copy, arr, sizeof(arr)); address_calculation_sort(copy,n);   printf("Address Calc   : "); printArray(copy,n);

    return 0;
}

