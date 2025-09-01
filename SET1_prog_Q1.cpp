#include<stdio.h>
#include<ctype.h>
#include<string.h>
#define MAX 100
char  stack[MAX];
int top=-1;
void push(char c)
{
	if(top<MAX-1){
		stack[++top]=c;
	}
}
char pop(){
	if(top>=0){
		return stack[top--];
	}
	return '\0';
}
char peek(){
	if(top>=0){
		return stack[top];
	}
	return '\0';
}
int precedence(char op){
	if(op=='/'|| op=='*')return 2;
	if(op=='+'|| op=='-')return 1;
	return 0;
}
void infixToPostfix(char infix[],char postfix[])
{
	int i,k=0;
	char ch;
	for(i=0;i<strlen(infix);i++)
	{
		ch=infix[i];
		if(isdigit(ch))
		{
			postfix[k++]=ch;
		}
		else if(ch=='(')
		{
			push(ch);
		}
		else if(ch==')')
		{
			while(top!=-1 && peek()!='(')
			{
				postfix[k++]=pop();
			}
			pop();
		}
		else if(ch=='+'|| ch=='-'|| ch=='*'||ch=='/')
		{
			while(top!=-1 && precedence(peek())>=precedence(ch))
			{
				postfix[k++]=pop();	
			}
			push(ch);	
		}
	}
	while(top!=-1)
	{
		postfix[k++]=pop();
	}
	postfix[k]='\0';
}
int main()
{
	char infix[MAX],postfix[MAX];
	printf("Enter infix expression");
	scanf("%s",infix);
	infixToPostfix(infix, postfix);
	printf("postfix expressons: %s\n",postfix);
	return 0;
}

