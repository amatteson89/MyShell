/*
*   Name: Andrew Matteson
*   Date: April 17, 2013
*   Course: Operating Systems
*   
*   Homework 1: Writing a basic shell
*/

#include "HistoryLinkedList.h"

#define TESTER 0





/********NODE OPERATIONS*******/
void addHistory(History ** head, char * cmd, int maxSize){
	History * temp = (History *) calloc( 1, sizeof(History));
	temp -> cmd = (char *) calloc (strlen(cmd) + 1, sizeof(char));
	strcpy(temp -> cmd, cmd);

	


	if( *head == NULL){
		
		temp->number = 1;
		*head = temp;
		
	} else {
		History *cur = *head;
		
		temp->number = cur->number + 1;
			
		//This while loops checks to see if it should go in the inside
		while(cur -> next != NULL){
			
			
			//remove the first node if the length exceeds maxsize
			if( ( temp->number - (*head)->number) ==  maxSize - 1){
				History * newHead = *head;
				*head = (*head) -> next;
				free(newHead -> cmd);
				free(newHead);
			}
			
			

			temp -> number++;
			cur = cur -> next;
		}
		cur -> next = temp;
		size++; 
	}//end else


}//end addOrdered
/**************END NODE OPERATIONS***************/





void printHistoryList(History * head){
	#if TESTER
	printf("\n\n===History List=====\n");
	#endif

	int nodeNum = 0;

	if(head == NULL){
		
		#if TESTER
		printf("Emptry List\n");
		#endif

	} else {
		History * cur = head;
		while(cur != NULL){
			printf(" %d\t%s\n", cur -> number, cur -> cmd);
			cur = cur -> next;
			nodeNum++;
		} //end while
		
	}//end else
}//end printList



void clearHistoryList(History ** head){
	if(*head == NULL){
		printf("Empty List");
	} else{
		History * cur = *head;
		while(*head != NULL){
			*head = cur -> next;

			/****CLEAN UP*****/
			free(cur -> cmd);
			free(cur);
			/*****************/

			cur = *head;
		}//end while
	}//end else
	
	#if TESTER
	printf("List cleared.\n\n");
	#endif

}//end clearList



void readInHistory(History ** head, int maxSize){
	FILE * fin = fopen(".mssh_history", "r+");

    if(fin == NULL)
        fin = fopen(".mssh_history", "w+");


    while(!feof(fin)){
        char temp[1000];
        fgets(temp, 100, fin);

        if(strlen(temp) == 0){
        	break;
        }

        #if TESTER 
        printf(" %d\t%s", counter, temp);
        #endif

        strip(temp);

        addHistory(head, temp, maxSize);

        temp[0] = '\0';
    }


    fclose(fin);
}


int isPrintHistory(char * string){
	if(strncmp(string, "history", 7) == 0)
		return 1;
	return 0;
}

int isHisNumCommand(char * string){
	if(strncmp(string, "!", 1) == 0)
		return 1;
	return 0;
}

int parseHistoryNumber(char * string){
	char * temp = NULL;

	temp = strtok(string, "!");
	int test = atoi(temp);

	return test;

}



void runCommandNumber(History * head, int n, char * path){
	History * cur = head;
	
	while(cur != NULL && cur->number != n ){
		cur = cur -> next;
	}

	//This will happen if the input number isn't in the range
	if(cur == NULL){
		errorMessage();
		return;
	}
	
	char ** argv;
	int argc;


	argc = makeargs( cur -> cmd, &argv);

	#if TESTER
		printf("command = %s\n", cur -> cmd);
		printf("ARGC = %d<----\n", argc);
		printargs(argc, argv);
	#endif

	runCommand(argv, argc, path);
}