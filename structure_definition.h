#pragma once

/*
* This file contains the definition of the structure
* that we will use when sending information over the network
*/

typedef struct {
	int iAge; 
	char name[20];
	char address[20];
} struct_person;

typedef struct {
	int iAge;
	char name[20];
} struct_person_small;