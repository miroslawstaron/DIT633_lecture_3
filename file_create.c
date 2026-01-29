/*
 * file_create.c
 * 
 * Copyright (c) 2026 Miroslaw Staron
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
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