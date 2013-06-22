/*
*   Name: Andrew Matteson
*   Date: April 17, 2013
*   Course: Operating Systems
*   
*   Homework 1: Writing a basic shell
*/

#ifndef _HELPER_H_
#define _HELPER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "LinkedList.h"
#include "HistoryLinkedList.h"


//Parse pipe command
void parsePipe(int totalPipes,char * s, char *** prePipe, int * prePipeCount, char *** postPipe, int * postPipeCount);

//runs given pipe command
void pipeRun(char * s, int totalPipes, char ** prePipe, char ** postPipe, int prePipeCount, int postPipeCount);

//This runs the command list given
void runCommand(char ** command, int argc, char * path);

//returns the number of pipe symbols in a given string
int countPipes(char * s);

//converts strings into array of words seperated by spaces
int makeargs(char *s, char *** argv);

//converts string into array of words seperated by :
int makePathargs(char *s, char *** argv);

//print out array of words
void printargs(int argc, char **argv);

//clean up memory
void clean(int argc, char ** argv);

//strip a string of carriage return
void strip(char *s);

//get input from the user
void getInput(char * temp);

//shell prompt
void prompt();

//shell error message
void errorMessage();

//appends path to the given command
void AppendPath(char ** result, char * path, char * command);


#endif