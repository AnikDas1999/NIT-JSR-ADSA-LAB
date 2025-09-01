#include <stdio.h>

int main() {
    int rows, cols;
    
    // take size of array
    printf("Enter number of rows: ");
    scanf("%d", &rows);
    printf("Enter number of columns: ");
    scanf("%d", &cols);

    int arr[rows][cols];   // declare 2D array

    // initialize with 0
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            arr[i][j] = 0;
        }
    }

    int r, c, value;
    printf("Enter row no: ");
    scanf("%d", &r);
    printf("Enter column no: ");
    scanf("%d", &c);
    printf("Enter value: ");
    scanf("%d", &value);

    // put value at given position
    if (r >= 0 && r < rows && c >= 0 && c < cols) {
        arr[r][c] = value;
    } else {
        printf("Invalid row or column!\n");
    }

    // print array
    printf("\nFinal 2D Array:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }

    return 0;
}

