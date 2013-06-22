/*
*   Name: Andrew Matteson
*   Date: April 17, 2013
*   Course: Operating Systems
*   
*   Homework 1: Writing a basic shell
*/

#ifndef _linkedList_H_
#define _linkedList_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"


struct Node{
	char * alias;
	char * cmd;
	struct Node * next;
};

typedef struct Node Node;


extern int size;



// Adds elements in ordered by alphanumeric value
void addOrdered(Node ** head, char * alias, char * cmd); 

//Input the alias name it outputs the command list
char * findAlias(Node ** head, char * alias); 

//print the list
void printList(Node * head); 

//removes alias from linked list
void removeAlias(Node ** head, char * alias); 

//clear list and memory
void clearList(Node ** head);

//parses given string into correct format
void parseAliasCMD(Node ** head, char * string);

//parses given string to remove alias in list
//	if alias not found, it doesn't do anything
void parseRemoveAlias(Node ** head, char * string);

//run specified alias
void runAlias(Node * head, char * s, char * path);

//returns 1 if the given string is alias command, 0 otherwise
int isAliasCMD(char * string);

//returns 1 if the given string is an unalias command
int isUnaliasCMD(char * string);

//reads through msshrc and loads up linked list
int runMSSHrc(Node ** head, char ** path);


#endif