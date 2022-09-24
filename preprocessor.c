
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> 



struct txt
{
	char line[BUFSIZ]; //for line by line file reading!
};
struct txt textbuffer[100];


struct traceVar
{
	char data[15];
};
struct traceVar traceVariableArray[10];


int traceVarArrLength = 0;
int textBufferLength;
int exSpaceNum = 0;   //this is just expandedCountGenerator space number  ->_________save(a,"a");
char tVar[20] = "";   //for current traceVariable!


void fillTextBuffer()
{
	FILE* file;
	int i;


	//Firstly we are going to fill textbuffer with line by line reading operation!
	file = fopen("DenemeC.txt", "r");
	i = 0;
	while (!feof(file))
	{
		fgets(textbuffer[i].line, sizeof(textbuffer[i].line), file);
		printf("%s", textbuffer[i].line);
		++i;
	}
	textBufferLength = i;  //textBufferLength is global and i refers length of textbuffer!
	strcpy(textbuffer[i - 1].line, "}");
	printf("%s", textbuffer[i - 1].line);
	printf("\n\n");
	fclose(file);
	//End of textbuffer filling
}

int hasSetTraceVariables(char line[])
{
	char str[20] = "setTraceVariables";

	if (strstr(line, str) == NULL)   //strstr() checks for whether one string is a sub-string of another string
		return 0;
	else
		return 1;
}

void foundTraceVariable(char line[])
{
	char str[15] = { NULL };
	int j = 0;

	for (int i = 0; i < strlen(line); ++i)
	{
		if (line[i - 1] == '\"' && i > 0)  //if i=0 then i-1->-1 and this is undefined array index!
		{
			int i_tmp = i;
			while (line[i_tmp] != '\"') //if variables have more than one character this code fragment would be necessary!
			{
				str[j] = line[i_tmp++]; //str is a temp trace variable
				++j;
			}
			break;
		}
	}
	strcpy(traceVariableArray[traceVarArrLength++].data, str);
}

int hasConditionOperator(char line[], char condition[])
{
	if (strstr(line, condition) == NULL)   //strstr() checks for whether one string is a sub-string of another string
		return 0;
	else
		return 1;
}


void fillTraceVariableArray()
{
	for (int i = 0; i < textBufferLength; ++i) //scan all line respectively
	{
		if (hasSetTraceVariables(textbuffer[i].line)) //if substring,setTraceVariables,has found!
		{
			//found trace variable operations!!
			foundTraceVariable(textbuffer[i].line);
		}
	}
}

void writeLineToFile(char line[])
{
	//printf("Normal\n");
	FILE* file;
	file = fopen("expandedC.txt", "a");  //In the beginning,expandedC.txt must be EMPTY!!!!! VERY IMPORTANT!!!!
	fprintf(file, "%s", line);
	fclose(file);
}

void addExtraLineToFile() //we should use exSpaceNum->'\t' number and tVar->current traceVariable
{
	//printf("Extra\n");
	FILE* file;
	file = fopen("expandedC.txt", "a");

	for (int i = 0; i < exSpaceNum; ++i) //  \t\t...
		fprintf(file, "%s", "\t");
	fprintf(file, "%s", "save(\"");
	fprintf(file, "%s", tVar);
	fprintf(file, "%s", "\",");
	fprintf(file, "%s", tVar);
	fprintf(file, "%s", ");\n");
	fclose(file);
}

int foundExSpaceNum(char line[], char* var)//this is just return  expandedCountGenerator space number  ->_________save(a,"a");
{
	char* result = strstr(line, var);
	int position = result - line;

	return position;  //find '\t' number!!
}

int checkCondition(char line[]) //a=..   b=...   c=...  conditions checking function! 
{
	char key[20] = ""; //for a=..   b=..
	char keyFirst[20] = "";

	for (int i = 0; i < traceVarArrLength; ++i) //we scan all traceVariableArray here! a,b,c,d consecutively!
	{
		strcpy(key, traceVariableArray[i].data);
		strcpy(keyFirst, traceVariableArray[i].data);
		strcat(key, "=");  //we generate keys like a=...  b=...   count=...

		if (hasConditionOperator(line, key))
		{
			if (hasConditionOperator(line, "for("))  //for(int a=..) condition!
			{
				//for condition code adding '{'
			}
			exSpaceNum = foundExSpaceNum(line, keyFirst);
			strcpy(tVar, keyFirst); //trace variable!
			return 1;
		}
	}
	return 0;
}

void createExpandedCode()
{
	for (int i = 0; i < textBufferLength; ++i) //scan all line respectively
	{
		if (checkCondition(textbuffer[i].line))
		{
			writeLineToFile(textbuffer[i].line);  //normal line
			addExtraLineToFile();				  //extra line
		}
		else
			writeLineToFile(textbuffer[i].line);  //normal line
	}
}


int main()
{
	fillTextBuffer();
	fillTraceVariableArray();
	createExpandedCode();
	return 0;
}

