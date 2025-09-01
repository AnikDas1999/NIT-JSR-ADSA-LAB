#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 1000

int numStack[MAX];
int numTop = -1;
char opStack[MAX];
int opTop = -1;

void pushNum(int v) {
    if (numTop >= MAX - 1) { printf("Number stack overflow\n"); exit(1); }
    numStack[++numTop] = v;
}
int popNum() {
    if (numTop < 0) { printf("Missing operand\n"); exit(1); }
    return numStack[numTop--];
}

void pushOp(char c) {
    if (opTop >= MAX - 1) { printf("Operator stack overflow\n"); exit(1); }
    opStack[++opTop] = c;
}
char popOp() {
    if (opTop < 0) { printf("Missing operator\n"); exit(1); }
    return opStack[opTop--];
}
char peekOp() {
    if (opTop < 0) return '\0';
    return opStack[opTop];
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

int applyOp(int a, int b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) { printf("Division by zero\n"); exit(1); }
            return a / b;
    }
    printf("Unknown operator '%c'\n", op);
    exit(1);
}

void applyTopOp() {
    if (opTop < 0) { printf("No operator to apply\n"); exit(1); }
    if (numTop < 1) { printf("Insufficient operands\n"); exit(1); }
    char op = popOp();
    int b = popNum();
    int a = popNum();
    pushNum(applyOp(a, b, op));
}

void processOperator(char op) {
    while (opTop != -1 && peekOp() != '(' && precedence(peekOp()) >= precedence(op)) {
        applyTopOp();
    }
    pushOp(op);
}

int evaluate(const char *s) {
    int i = 0, len = strlen(s);
    while (i < len) {
        char c = s[i];

        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') { i++; continue; }

        if (isdigit((unsigned char)c)) {
            long val = 0;
            while (i < len && isdigit((unsigned char)s[i])) {
                val = val * 10 + (s[i] - '0');
                i++;
            }
            pushNum((int)val);
        }
        else if (c == '(') {
            pushOp(c);
            i++;
        }
        else if (c == ')') {
            while (opTop != -1 && peekOp() != '(') applyTopOp();
            if (peekOp() == '(') popOp(); else { printf("Mismatched parentheses\n"); exit(1); }
            i++;
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            
            if (c == '-') {
                int j = i - 1;
                while (j >= 0 && isspace((unsigned char)s[j])) j--;
                if (j < 0 || s[j] == '(' || s[j] == '+' || s[j] == '-' || s[j] == '*' || s[j] == '/') {
                   
                    i++;
                    while (i < len && isspace((unsigned char)s[i])) i++;
                    if (i < len && isdigit((unsigned char)s[i])) {
                        int val = 0;
                        while (i < len && isdigit((unsigned char)s[i])) {
                            val = val * 10 + (s[i] - '0');
                            i++;
                        }
                        pushNum(-val);
                        continue;
                    } else if (i < len && s[i] == '(') {
                        
                        pushNum(0);
                        processOperator('-');
                        continue;
                    } else {
                        
                        pushNum(0);
                        processOperator('-');
                        continue;
                    }
                }
            }
            processOperator(c);
            i++;
        }
        else {
            printf("Unknown character '%c' in expression\n", c);
            exit(1);
        }
    }

    while (opTop != -1) {
        if (peekOp() == '(' || peekOp() == ')') { printf("Mismatched parentheses\n"); exit(1); }
        applyTopOp();
    }

    if (numTop != 0) {
        
    }
    return popNum();
}

int main(int argc, char *argv[]) {
    char expr[MAX];

    if (argc > 1) {
        
        expr[0] = '\0';
        for (int i = 1; i < argc; ++i) {
            strcat(expr, argv[i]);
            if (i + 1 < argc) strcat(expr, " ");
        }
    } else {
        printf("Enter an arithmetic expression: ");
        if (!fgets(expr, sizeof(expr), stdin)) { printf("Input error\n"); return 1; }
    }

    int result = evaluate(expr);
    printf("%d\n", result);
    return 0;
}

