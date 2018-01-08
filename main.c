/*main.c*/

//
// Balanced Expression Check
//
// Omar Zorob
// UIN: 667503276
// U. of Illinois, Chicago
// CS211, Spring 2017
// Project #02
//

#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

typedef struct Stack
{
	char *Darr; //points to dynamic array holding values for stack
	int Size; //number of elements in stack
	int Top; //indicates where the top of the stack is
}Stack;

void init(Stack *s)
{
	s->Size = 2;//Size of stack is initially set to 2 but is grown if neccessary in push function
	s->Darr = (char *)malloc(sizeof(char) * s->Size); //Dynammically allocated array set to hold as many chars as the size indicates
	s->Top = -1; //Top of the stack is intially set to -1 so that the user knows if the stack is empty or not
}

void push(Stack *s, char val, int debugMode)
{
	s->Top = s->Top + 1; // Adding 1 to the stack so we make top + 1 so the user knows there is another element in the stack
	if (s->Size == s->Top)//if stack is full grow it
	{
		if (debugMode == TRUE)
			printf("Size of Array Before Expansion: %d\n", s->Size);
		s->Size = s->Size + 2;
		if (debugMode == TRUE)
			printf("Size of Array After Expansion: %d\n", s->Size);
		char *temp = (char *)malloc(sizeof(char)  * s->Size);
		int i;
		for (i = 0; i < s->Size - 2; i++)
		{
			temp[i] = s->Darr[i];
		}
		if (debugMode == TRUE)
			printf("Number of values copied to new array: %d\n", i + 1); //account for the 0th element in array
		free(s->Darr);// free space in old dynamic array
		s->Darr = temp;//set dynammic array to new size
	}
	s->Darr[s->Top] = val;//push new val on to stack
}

int isEmpty(Stack *s)
{
	/* Checks if stack
	is empty by checking
	where the top of the stack
	is */

	if (s->Top == -1) {
		return TRUE;
	}
	else
		return FALSE;
}

int top(Stack* s)
{
	return (s->Darr[s->Top]); //Returns the char on top of the stack
}

void pop(Stack *s)
{
	s->Top = s->Top - 1; //Remove the top item from stack
}

void EmptyStack(Stack *s)
{
	free(s->Darr); //Get rid of whatever was in stack
	init(s);//Re initialize stack
	s->Size = 2;//Reset Stack size to 2
}

int main(int argc, char** argv)
{
	int debugMode = FALSE;
	int c;
	for (c = 0; c < argc; c++)
	{
		int z;
		for (z = 0; z < argc; z++)
		{
			if (argv[c][z] == '-' && argv[c][z + 1] == 'd')
				debugMode = TRUE;
		}
	}

	int MAXINPUT = 300; // Longest Possible expression that can be checked
	char expected, missing; // char that was either missing from expression or that was expected
	int errorLoc = -1, missingCount = -1; // location of missing or expected char
	int BALANCED = TRUE;// Expressions are by default balanced

	char userInput[MAXINPUT];//char array to hold userInput

	Stack s;
	init(&s); // Initialize Stack

	printf("Enter an expression to test. (Q or q to quit)\n");
	while (fgets(userInput, MAXINPUT, stdin))
	{
		if (debugMode == TRUE)
			printf("Debugging Information:\n");

		unsigned int countChars = strlen(userInput) - 1;//length of user Input

		if ((userInput[0] == 'q' && countChars == 1) || (userInput[0] == 'Q' && countChars == 1)) { //Exit Case
			break;
		}

		int i;
		for (i = 0; i < countChars + 1; i++)
		{
			/* If one of these opening characters are encountered add to top of stack */
			if (userInput[i] == '<' || userInput[i] == '(' || userInput[i] == '{' || userInput[i] == '[')
			{
				if (debugMode == TRUE)
					printf("Push: %c\n", userInput[i]);
				push(&s, userInput[i], debugMode);
			}

			/* If one of these characters are encountered their corresponding opening bracket either
			needs to be pushed off the top of the stack if it is located on top of the stack or the
			expression is not balanced */
			else if (userInput[i] == '>' || userInput[i] == ')' || userInput[i] == '}' || userInput[i] == ']')
			{
				/* Case 1: Stack is empty and encounters a closing bracket
				which automatically means expression is not balanced */

				if (isEmpty(&s) == TRUE)
				{
					missingCount = i;
					if (userInput[i] == '>') {
						missing = '<';
						BALANCED = FALSE;
						break;
					}
					else if (userInput[i] == ']') {
						missing = '[';
						BALANCED = FALSE;
						break;
					}
					else if (userInput[i] == '}') {
						missing = '{';
						BALANCED = FALSE;
						break;
					}
					else if (userInput[i] == ')') {
						missing = '(';
						BALANCED = FALSE;
						break;
					}
					break;
				}


				/* Case 2: If Top of the stack is the corresponding
				opening bracket pop the opening bracket from the top
				of the stack */

				else if (s.Darr[s.Top] == '{' && userInput[i] == '}') {
					if (debugMode == TRUE)
						printf("Pop : %c\n", '{');
					pop(&s);
				}
				else if (s.Darr[s.Top] == '<' && userInput[i] == '>') {
					if (debugMode == TRUE)
						printf("Pop : %c\n", '<');
					pop(&s);
				}
				else if (s.Darr[s.Top] == '(' && userInput[i] == ')') {
					if (debugMode == TRUE)
						printf("Pop : %c\n", '(');
					pop(&s);
				}
				else if (s.Darr[s.Top] == '[' && userInput[i] == ']') {
					if (debugMode == TRUE)
						printf("Pop : %c\n", '[');
					pop(&s);
				}

				/* Case 3: If top of stack is not the corresponding
				opening bracket the expression is not balanced so error Location
				is set as well as the expected closing bracket */

				else if (s.Darr[s.Top] == '{' && userInput[i] != '}') {
					errorLoc = i;
					expected = '}';
					BALANCED = FALSE;
					break;
				}
				else if (s.Darr[s.Top] == '<' && userInput[i] != '>') {
					errorLoc = i;
					expected = '>';
					BALANCED = FALSE;
					break;
				}
				else if (s.Darr[s.Top] == '(' && userInput[i] != ')') {
					errorLoc = i;
					expected = ')';
					BALANCED = FALSE;
					break;
				}
				else if (s.Darr[s.Top] == '[' && userInput[i] != ']') {
					errorLoc = i;
					expected = ']';
					BALANCED = FALSE;
					break;
				}
			}
		}

		/* No more input is available if the
		stack is not empty at this point the
		expression is not balanced and is missing at
		least one character which is the corresponding
		closing bracket of the top of the stack. */

		if (isEmpty(&s) == FALSE)
		{
			if (missingCount == -1)
				missingCount = countChars;
			if (s.Darr[s.Top] == '<')
				missing = '>';
			else if (s.Darr[s.Top] == '[')
				missing = ']';
			else if (s.Darr[s.Top] == '{')
				missing = '}';
			else if (s.Darr[s.Top] == '(')
				missing = ')';
		}

		/*Expression is Balanced */
		if (isEmpty(&s) == TRUE && BALANCED == TRUE)
			printf("Expression is Balanced\n");

		/* If expression was not balanced there
		is two ways it was not balanced either
		an unexpected character was encountered
		or a character is missing */
		else
		{
			int x;

			//Unexpected character encountered
			if (errorLoc != -1)
			{
				if (debugMode == TRUE)
					printf("%s", userInput);
				for (x = 0; x < errorLoc; x++)
				{
					printf(" ");
				}
				printf("^ expecting %c\n", expected);
			}

			//Character neccessary for balancing was missing
			else
			{
				if (debugMode == TRUE)
					printf("%s", userInput);
				for (x = 0; x < missingCount; x++)
				{
					printf(" ");
				}
				printf("^ missing %c\n", missing);
			}
		}

		EmptyStack(&s); //Empty Stack for next userInput to be checked
		missingCount = -1; //Reset loc of missing character
		errorLoc = -1; //Reset loc of unexpected character
		BALANCED = TRUE;//Reset Balanced to True
		printf("Enter another expression to test. (Q or q to quit)\n");
	}
	return 0;
}
