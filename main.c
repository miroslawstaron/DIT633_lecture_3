/*
 * main.c
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
 * Main file for lecture #3 -- mostly about Files and sockets
 */
#include <stdio.h>
#include "file_create.h"
#include "read_binary_file.h"
#include "unions.h"
#include "states.h"


// main 
int main(int argc, char* argv[])
{
	// demonstration of creating, reading and writing to a file
	// demo_file_create();

	// demonstration of reading binary files
	// demo_file_binary();

	// demonstration of writing binary files
	// demo_write_binary();

	// demonstration of reading and writing binary files with dynamic strings
	// dynamic_file_main();

	// demonstration of unions
	// unions_main();

	// demonstration of unions with bit-fields
	// bitunions_main();

	// demonstration of simple state machine
	// main_states();

	// demonstration of simple state machine with transition table
	main_transitions();

	return 0;
}
