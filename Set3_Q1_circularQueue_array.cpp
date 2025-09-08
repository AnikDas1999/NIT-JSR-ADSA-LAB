#include <stdio.h>
#define SIZE 5  // maximum size of queue

int queue[SIZE];
int front = -1, rear = -1;

// Insert (Enqueue)
void enqueue(int value) {
    if ((rear + 1) % SIZE == front) { // queue full
        printf("Queue is FULL\n");
        return;
    }
    if (front == -1) // first element
        front = 0;
    rear = (rear + 1) % SIZE;  // circular move
    queue[rear] = value;
    printf("%d inserted\n", value);
}

// Delete (Dequeue)
void dequeue() {
    if (front == -1) { // queue empty
        printf("Queue is EMPTY\n");
        return;
    }
    printf("%d removed\n", queue[front]);
    if (front == rear) // only one element
        front = rear = -1;
    else
        front = (front + 1) % SIZE;
}

// Display queue
void display() {
    if (front == -1) {
        printf("Queue is EMPTY\n");
        return;
    }
    printf("Queue: ");
    int i = front;
    while (1) {
        printf("%d ", queue[i]);
        if (i == rear) break;
        i = (i + 1) % SIZE;
    }
    printf("\n");
}

int main() {
    enqueue(10);
    enqueue(20);
    enqueue(30);
    display();
    dequeue();
    display();
    enqueue(40);
    enqueue(50);
    enqueue(60); // will show FULL
    display();
    return 0;
}
