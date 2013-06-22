/*
*   Name: Andrew Matteson
*   Date: April 17, 2013
*   Course: Operating Systems
*   
*   Homework 1: Writing a basic shell
*/

#ifndef _HISTORYLINKEDLIST_H_
#define _HISTORYLINKEDLIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"


struct HistoryNode{
	char * cmd;
	int number;
	struct HistoryNode * next;
};

typedef struct HistoryNode History;


//add given cmd to linked list
void addHistory(History ** head, char * cmd, int maxSize);

//prints out history list
void printHistoryList(History * head);

//clear history list (unused in program)
void clearHistoryList(History ** head);

//read in history list from the file .mssh_history  (will be created if not found)
void readInHistory(History ** head, int maxSize);

//run given history command number
void runCommandNumber(History * head, int n, char * path);

//returns 1 if the string is a history command, 0 otherwsie
int isPrintHistory(char * string);

//returns 1 if the string is a history number command, 0 otherwise
int isHisNumCommand(char * string);

//parses history command for history number
int parseHistoryNumber(char * string);
/*************/

#endif