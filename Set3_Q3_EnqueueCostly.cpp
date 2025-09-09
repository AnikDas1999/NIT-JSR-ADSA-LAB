#include <stdio.h>
#include <stdlib.h>
#define MAX 100

/* --- Circular queue (array) --- */
typedef struct {
    int a[MAX];
    int front;   // index of next dequeue
    int rear;    // index of next enqueue
    int count;   // number of elements
} Queue;

void q_init(Queue *q) { q->front = 0; q->rear = 0; q->count = 0; }
int q_empty(Queue *q) { return q->count == 0; }
int q_full(Queue *q) { return q->count == MAX; }

void enqueue(Queue *q, int x) {
    if (q_full(q)) { printf("Queue full\n"); exit(1); }
    q->a[q->rear] = x;
    q->rear = (q->rear + 1) % MAX;
    q->count++;
}

int dequeue(Queue *q) {
    if (q_empty(q)) { printf("Queue empty\n"); exit(1); }
    int v = q->a[q->front];
    q->front = (q->front + 1) % MAX;
    q->count--;
    return v;
}

int q_front(Queue *q) {
    if (q_empty(q)) { printf("Queue empty\n"); exit(1); }
    return q->a[q->front];
}

/* --- Stack using two queues, push costly --- */
typedef struct {
    Queue q1, q2;
} Stack;

void stack_init(Stack *s) { q_init(&s->q1); q_init(&s->q2); }

int stack_empty(Stack *s) { return q_empty(&s->q1); }

/* Push is costly: enqueue to q2, move all from q1 to q2, swap q1/q2 */
void stack_push(Stack *s, int x) {
    if (q_full(&s->q1) && q_full(&s->q2)) { printf("Stack (queues) full\n"); exit(1); }
    enqueue(&s->q2, x);
    while (!q_empty(&s->q1)) enqueue(&s->q2, dequeue(&s->q1));
    // swap q1 and q2 (struct copy)
    Queue tmp = s->q1; s->q1 = s->q2; s->q2 = tmp;
}

/* Pop is simple: dequeue from q1 */
int stack_pop(Stack *s) {
    if (stack_empty(s)) { printf("Stack empty\n"); exit(1); }
    return dequeue(&s->q1);
}

/* Peek top element */
int stack_top(Stack *s) {
    if (stack_empty(s)) { printf("Stack empty\n"); exit(1); }
    return q_front(&s->q1);
}

/* --- Demo --- */
int main(void) {
    Stack st;
    stack_init(&st);

    stack_push(&st, 10);
    stack_push(&st, 20);
    stack_push(&st, 30);

    printf("Top: %d\n", stack_top(&st));   // 30
    printf("Pop: %d\n", stack_pop(&st));   // 30
    printf("Pop: %d\n", stack_pop(&st));   // 20

    stack_push(&st, 40);
    printf("Top: %d\n", stack_top(&st));   // 40
    printf("Pop: %d\n", stack_pop(&st));   // 40
    printf("Pop: %d\n", stack_pop(&st));   // 10

    return 0;
}

