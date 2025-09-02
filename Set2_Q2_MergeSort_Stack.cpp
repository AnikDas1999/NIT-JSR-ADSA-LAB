#include <stdio.h>
#include <stdlib.h>

// Structure to simulate recursive calls
typedef struct {
    int l, r, stage;
} Frame;

void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];

    free(L);
    free(R);
}

void mergeSort(int arr[], int n) {
    Frame stack[1000]; // Stack to store frames
    int top = -1;

    // Push initial range
    stack[++top] = (Frame){0, n - 1, 0};

    while (top >= 0) {
        Frame *cur = &stack[top];

        if (cur->l >= cur->r) {
            top--; // Single element, pop
            continue;
        }

        int mid = (cur->l + cur->r) / 2;

        if (cur->stage == 0) {
            // First, solve left half
            cur->stage = 1;
            stack[++top] = (Frame){cur->l, mid, 0};
        } else if (cur->stage == 1) {
            // Then, solve right half
            cur->stage = 2;
            stack[++top] = (Frame){mid + 1, cur->r, 0};
        } else {
            // Finally, merge both halves
            merge(arr, cur->l, mid, cur->r);
            top--; // Pop after processing
        }
    }
}

int main() {
    int arr[] = {29, 10, 14, 37, 13, 2, 9, 25};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Original array: ");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");

    mergeSort(arr, n);

    printf("Sorted array:   ");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");

    return 0;
}

