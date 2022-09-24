#include <cstdio>

struct tr
{
	char name[15]; // variable name
	int values[20]; // last 20 values
	int first; // points to the index of the first value in values[]
	int last; // points to the index of the last value in values[]
	int size; //store values index!
};

struct tr Traces[5]; // 5 variables
int tracesLength = 0;

void setTraceVariables(char* c)
{
	// enter variable name in c in Traces[]
	FILE* file;
	file = fopen("traceVariables.txt", "r");

	char* data = (char*)malloc(20);

	while (!feof(file))	 //search on traceVariable File whether has or not!
	{
		fscanf(file, "%s", data);
		data = strtok(data, "\n");
		if (!strcmp(data, c))
		{
			strcpy(Traces[tracesLength++].name, c);
			break;
		}

	}

}
void breakpoint(char* c, int n)
{
	// search variable name in c in Traces[] and display its last n values
	// wait for the user to press enter

	for (int i = 0; i < tracesLength; ++i)
	{
		if (!strcmp(c, Traces[i].name)) //when we are going to find right index of c on traceVar. Array
		{
			if (Traces[i].size < n)
			{
				for (int j = 0; j < Traces[i].size; ++j)
					printf("%s:%d\n", Traces[i].name, Traces[i].values[j]);
				getchar(); //for waiting to press enter!!
			}
			else
			{
				for (int j = 0; j < n; ++j)
					printf("%s:%d\n", Traces[i].name, Traces[i].values[j]);
				getchar(); //for waiting to press enter!!
			}
		}
	}

}

void arrayShift(int arr[], int addedNum)  //x-1 shift operation!!
{
	for (int i = 0; i < 19; ++i)
	{
		arr[i] = arr[i + 1];
	}
	arr[19] = addedNum;
}

void save(char* c, int num)
{
	// search variable name c in Traces[] and enter its current value in values[]

	for (int i = 0; i < tracesLength; ++i)
	{
		if (!strcmp(c, Traces[i].name)) //when we are going to find right index of c on traceVar. Array
		{
			if (Traces[i].size >= 20)
			{
				//shift operations
				int last = Traces[i].size - 1;
				arrayShift(Traces[i].values, num);
			}
			else
			{
				Traces[i].values[Traces[i].size++] = num;
				break;
			}
		}
	}
}

