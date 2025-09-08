#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node *front = NULL, *rear = NULL;

// Insert (Enqueue)
void enqueue(int value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = value;
    if (front == NULL) { // first element
        front = rear = newNode;
        rear->next = front;
    } else {
        rear->next = newNode;
        rear = newNode;
        rear->next = front; // circular link
    }
    printf("%d inserted\n", value);
}

// Delete (Dequeue)
void dequeue() {
    if (front == NULL) {
        printf("Queue is EMPTY\n");
        return;
    }
    if (front == rear) { // only one element
        printf("%d removed\n", front->data);
        free(front);
        front = rear = NULL;
    } else {
        struct Node* temp = front;
        printf("%d removed\n", front->data);
        front = front->next;
        rear->next = front;
        free(temp);
    }
}

// Display
void display() {
    if (front == NULL) {
        printf("Queue is EMPTY\n");
        return;
    }
    struct Node* temp = front;
    printf("Queue: ");
    do {
        printf("%d ", temp->data);
        temp = temp->next;
    } while (temp != front);
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
    display();
    return 0;
}
