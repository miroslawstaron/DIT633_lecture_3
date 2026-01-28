/*
* Main file for lecture #3 -- mostly about Files and sockets
*/
#include <stdio.h>
#include "file_create.h"
#include "google_test.h"
#include "file_sockets_client_struct.h"
#include "file_sockets_client.h"
#include "file_sockets_server_struct.h"
#include "file_sockets_server.h"
#include "read_binary_file.h"

// main 
int main(int argc, char* argv[])
{
	// demonstration of creating, reading and writing to a file
	demo_file_create();

	// demonstration of reading binary files
	// demo_file_binary();

	// demonstration of writing binary files
	// demo_write_binary();

	// google_test();

	// main_client();
	
	// main_sock();

	// main_client_struct();
	
	// main_sock_struct();

	// all good, return 0

	// demonstration of simple state machine
	// int result = main_states();

	// demonstration of simple state machine with transition table
	int result = main_transitions();

	return 0;
}
