/*
 * read_binary_file.c
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
#include <string.h>
#define MAX 20			// max number of characters to read

struct sPerson {
	char name[20];
	int age;
};

// this function demonstrates how to write a series of integers to a binary file
void demo_write_binary()
{
	// name of my file that I will write to
	char* binaryFilename = "myBinaryFile.bin";
	
	// pointer to the file and
	// open the file to write
	FILE* pBinaryFile = fopen(binaryFilename, "wb");
	
	// a good practice when working with files is to check
	// that the file pointer is correct, i.e. the file was opened
	if (pBinaryFile != NULL)
	{
		// generate a series of numbers
		for (int i = 0x48; i < 0x97; i++)
		{
			// write the number to the file
			// in a binary format
			fwrite(&i,				// pointer to the data
				   sizeof(i),		// size of one element - in our case the size of one integer
									// we could even write sizeof(i) and that would be the same
				   1,				// number of elements to write
				   pBinaryFile);	// pointer to the file
		}
		
		struct sPerson person;
		
		strcpy(person.name, "John");
		person.age = 25;
		fwrite(&person, sizeof(person), 1, pBinaryFile);
	}
	// if the file was not opened, just provide an error message
	else
	{
		printf("Error opening file %s.", binaryFilename);
	}
	
	fclose(pBinaryFile);
}


void demo_file_binary()
{
	FILE* pFileToCreate;	// file which we will create

	// location to my file
	// let's use the same file as before to check how the reading looks like
	char* strFilename = "myTestFile2.txt";
	
	

	// open the file to read
	// Note! to read binary file we need to change the modifier - rb instead of just r
	pFileToCreate = fopen(strFilename, "rb");
	
	

	// a good practice when working with files is to check
	// that the file pointer is correct, i.e. the file was opened
	// in the way we wanted
	if (pFileToCreate != NULL)
	{
		// if the file was opened (see the flag)
		// we can do something with it
		// here I chose to read a string from the file

		char cValToRead = '0';	// value to read, initialized with 0 just to reserve some space

		fread(&cValToRead,				// where we store the value
			  sizeof(cValToRead),		// the size of the single element
			  1,						// how many elements we should read
			  pFileToCreate);			// where we should read them from (file)

		// and print the result on the console
		printf("%c: 0x%x \n", cValToRead, cValToRead);

		// now, version with the buffer
		char* pstrBuffer; 

		// reserve a place in memory for MAX characters
		pstrBuffer = malloc(MAX * sizeof(char));

		// now, let's make it fail safe wrt memory
		// and check if the memory was actually allocated
		if (pstrBuffer != NULL)
		{
			// read the number of elements to the buffer
			fread(pstrBuffer, 
				  sizeof(char), 
				  MAX-1, 
				  pFileToCreate); 

			// end of string in order to avoid printing random memory content
			pstrBuffer[MAX-1] = '\0';

			// let's print them and see what happens
			printf("%s \n", pstrBuffer);

			// don't forget to free the memory
			free(pstrBuffer);
		}
		else // the memory could not be allocated
		{
			printf("Error - memory could not be allocated!");
		}

		// and please remember to close the file
		// if we do not close the file, the data may be lost
		fclose(pFileToCreate);
	}
	else	// if the file was not opened, e.g. does not exists
	{
		printf("Error opening file %s.", strFilename);
	}	
}