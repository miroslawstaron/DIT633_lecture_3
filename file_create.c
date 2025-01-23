/*
* Creating, reading and writing a file
*/
#include <stdlib.h>		
#include <stdio.h>		// Note! this is the header file which allows us to work with files
#include "file_create.h"

#define MAX 20			// max number of characters to read

void demo_file_create()
{
	FILE* pFileToCreate;	// file which we will create

	// location to my file
	char* strFilename = "myTestFile2.txt";

	// open the file to read
	pFileToCreate = fopen(strFilename, "w");

	// a good practice when working with files is to check
	// that the file pointer is correct, i.e. the file was opened
	// in the way we wanted
	if (pFileToCreate != NULL)
	{
		// if the file was opened (see the flag)
		// we can do something with it
		// here I chose to read a string from the file

		char strToRead[MAX] = "Hello World! #2024\n";
		
		// read one word, %s means that it will read the text
		// until it finds a whitespace of EOF
		fputs(strToRead, pFileToCreate);
		
		// and print the result on the console
		fprintf(pFileToCreate, "%s ______\n", strToRead);

		// read one word, %s means that it will read the text
		// until it finds a whitespace of EOF
		// fscanf(pFileToCreate, "%s", strToRead);

		// and print the result on the console
		// printf("%s \n", strToRead);
	}
	else	// if the file was not opened, e.g. does not exists
	{
		printf("Error opening file %s.", strFilename);
	}

	// and please remember to close the file
	// if we do not close the file, the data may be lost
	fclose(pFileToCreate);
}