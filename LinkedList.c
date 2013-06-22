/*
*   Name: Andrew Matteson
*   Date: April 17, 2013
*   Course: Operating Systems
*   
*   Homework 1: Writing a basic shell
*/

#include "LinkedList.h"

#define TESTER 0

int size;




/********NODE OPERATIONS*******/
void addOrdered(Node ** head, char * alias, char * cmd){
	Node * temp = (Node *) calloc( 1, sizeof(Node));
	temp -> alias = (char *) calloc (strlen(alias) + 1, sizeof(char));
	temp -> cmd = (char *) calloc (strlen(cmd) + 1, sizeof(char));


	strcpy(temp -> alias, alias);
	strcpy(temp -> cmd, cmd);



	if( *head == NULL){
		*head = temp;
		size = 1;
	} else {
		Node *cur = *head;
		//First check to see if it should go at the beginning of the list
		if( strcmp(temp -> alias, cur -> alias) < 0){
			temp -> next = *head;
			*head = temp;
			size++;
		} else {
			
			//This while loops checks to see if it should go in the inside
			while(cur -> next != NULL){
				if( strcmp(temp -> alias, cur -> next -> alias) < 0){
					temp -> next = cur -> next;
					cur -> next = temp;
					size++;
					return;
				}//end if

				cur = cur -> next;
			}//end while

			//Doesn't fit anywhere else, stick it at the end
			cur -> next = temp;
			size++;
		} //end else 
	}//end else
}//end addOrdered
/**************END NODE OPERATIONS***************/



char * findAlias(Node ** head, char * alias){
	int nodeNum = 0;
	Node * cur = *head;

	while(cur != NULL){
		if(strcmp(cur -> alias, alias) == 0 ){
			#if TESTER
				printf("alias %s found\n", cur->alias);
				printf("it has cmd = %s\n", cur->cmd);
			#endif
			return cur -> cmd;
		}//end if
		nodeNum++;
		cur = cur -> next;
	}//end while
	#if TESTER
	printf("Alias not found\n\n");
	#endif

	return NULL;
}//end findPerson




void printList(Node * head){
	printf("\n\n===Current List=====\n");

	int nodeNum = 0;

	if(head == NULL){
		printf("size = %d\n", size);
		printf("Emptry List\n");
	} else {
		printf("size = %d\n", size);
		Node * cur = head;
		while(cur != NULL){
			printf("alias = %s, cmd = %s\n", cur -> alias, cur -> cmd);
			cur = cur -> next;
			nodeNum++;
		} //end while
		printf("\n\n");
	}//end else
}//end printList











void removeAlias(Node ** head, char * alias){
	Node * prev = *head;

	//Check  to see if the name is the first member
	if(strcmp(prev -> alias, alias) == 0 ){
		*head = prev -> next;
		free(prev -> alias);
		free(prev -> cmd);
		free(prev);
		size--;
		return;
	} else { 

		Node * cur = prev -> next;

		while(cur != NULL){
			if(strcmp(cur -> alias, alias) ==0 ){
				#if TESTING
				printf("Alias deleted!!\n");
				#endif
				//reassign next pointer
				prev -> next = cur -> next;

				//free Memeory
				free(cur -> alias);
				free(cur -> cmd);
				free(cur);
				
				//update size
				size--;
				return;
			}//end if
			cur = cur -> next;
			prev = prev -> next;
		}//end while
	}//end else

	printf("Alias not in the list\n");
}//end deletePerson





void clearList(Node ** head){
	if(*head == NULL){
		printf("Empty List");
	} else{
		Node * cur = *head;
		while(*head != NULL){
			*head = cur -> next;

			/****CLEAN UP*****/
			free(cur -> alias);
			free(cur -> cmd);
			free(cur);
			/*****************/

			cur = *head;
		}//end while
	}//end else
	
	#if TESTER
	printf("List cleared.\n\n");
	#endif

	size = 0;

}//end clearList


void parseAliasCMD(Node ** head, char * string){
	char tempS[1000];
	char * aliasTemp = NULL;
	char ** aliasArgs = NULL;
	char * CMDTemp = NULL;


	#if TESTER
	printf("input string %s<---\n", string);
	#endif

	strcpy(tempS,string);

    aliasTemp = strtok(tempS , "=");
    CMDTemp = strtok(NULL, "=");
    strip(CMDTemp);

    //clear out any " or ' characters
    for(int i = 0; i < strlen(CMDTemp); i++){
    	if( *(CMDTemp + i) == '\'' || *(CMDTemp + i) == '\"'){
    		*(CMDTemp + i) = ' ';
    	}
    }

	#if TESTER
    printf("---->DEBUG:alias temp =%s<-------\n", aliasTemp);
    #endif 


    //We want to remove the alias word
    makeargs(aliasTemp, &aliasArgs);
    //copy back the alias word
    strcpy(aliasTemp, aliasArgs[1]);
    //clean the memory a bit
    clean(2, aliasArgs);

	#if TESTER
    printf("---->DEBUG:alias temp =%s<-------\n", aliasTemp);
    #endif 
   

	#if TESTER
    printf("---->DEBUG:cmd temp =%s<-------\n", CMDTemp);
    #endif 

    addOrdered(head, aliasTemp, CMDTemp);
}





void parseRemoveAlias(Node ** head, char * string){
	char tempS[1000];
	char * aliasTemp = NULL;
	char ** aliasArgs = NULL;


	#if TESTER
	printf("input string %s<---\n", string);
	#endif

	strcpy(tempS,string);

    aliasTemp = strtok(tempS , "=");
    
	#if TESTER
    printf("---->DEBUG:alias temp =%s<-------\n", aliasTemp);
    #endif 


    //We want to remove the alias word
    makeargs(aliasTemp, &aliasArgs);
    //copy back the alias word
    strcpy(aliasTemp, aliasArgs[1]);
    //clean the memory a bit
    clean(2, aliasArgs);

	#if TESTER
    printf("---->DEBUG:alias temp =%s<-------\n", aliasTemp);
    #endif 
   
    removeAlias(head, aliasTemp);
}




void runAlias(Node * head, char * s, char * path){
	char command[1000];
	strcpy(command, findAlias(&head, s));
	
	char ** argv;
	int argc;


	argc = makeargs(command, &argv);

	#if TESTER
		printf("command = %s\n", command);
		printf("ARGC = %d<----\n", argc);
		printargs(argc, argv);
	#endif

	runCommand(argv, argc, path);
	


}


int isAliasCMD(char * string){
	 if(strncmp(string,"alias",5) == 0)
	 	return 1;
	 return 0;
}

int isUnaliasCMD(char * string){
	if(strncmp(string, "unalias", 7) == 0)
		return 1;
	return 0;
}

int runMSSHrc(Node ** head, char ** path){
    pid_t child;
    int result = 0;

    char temp[1000];
    //char ** commands;
    
    //int numCommands = 0;

    FILE * fin = fopen(".msshrc", "r");

    while(!feof(fin)){
        fgets(temp, 1000, fin);
        strip(temp);

        #if TESTER
            printf("strlen(temp) = %d\n", strlen(temp));
        #endif

        if(isAliasCMD(temp)){
            parseAliasCMD(head, temp);
        } else if( strncmp(temp, "history", 6)==0){
        	char * temporary = NULL;
        	temporary = strtok(temp, "=");
        	temporary = strtok(NULL, "=");

        	#if TESTER
        	printf("temp = %s", temporary);
        	#endif

        	result = atoi(temporary);
        }else if(strncmp(temp, "PATH", 4) == 0){
        	char * pathTemp = NULL;
        	pathTemp = strtok(temp,"=");
        	pathTemp = strtok(NULL,"=");
        	
        	*path = (char *) calloc(strlen(pathTemp) + 1, sizeof(char));

        	strcpy(*path, pathTemp);


        }else if(strlen(temp) > 0){
            //numCommands = makeargs(temp, &commands);

            if( (child = fork()) == 0){
                //execvp(commands[0], commands);
                system(temp);

                exit(-1);
            }
            waitpid(child, NULL, 0);
            //clean(numCommands, commands);
        }

        //need to null out temp so it won't repeat the last thing
        temp[0] = '\0';
    }


    fclose(fin);
    return result;

}

