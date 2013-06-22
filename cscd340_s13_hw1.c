/*
*   Name: Andrew Matteson
*   Date: April 17, 2013
*   Course: Operating Systems
*   
*   Homework 1: Writing a basic shell
*   
*   Extra Credit: My program handles multiple pipes
*    Example: ?:ls /etc | tail -4 | tac
*                xdg
*                xml
*                xul-ext
*                zsh_command_not_found
*
*
*
*
*   Known issues: 
*        - Can't do !(number) with alias commands
*
*   Notes: 
*       The .msshrc file allows you to have the info in any order
*            (it just parses and looks for keywords)
*       
*/



#include "helper.h"

#define TESTER 0

void MainCommandControl(char * s, char * prev, History * Histhead, Node ** head, int totalPipes, char * path);





int main(){

    /********VARIABLES*********/

  	char s[1000];
    char prev[1000];
    char * path;

	int totalPipes;
    int HistorySize = 0;

    Node * head = NULL;
    History * Histhead = NULL;
    /************************/


    //Do an initial run of .msshrc
    HistorySize = runMSSHrc(&head, &path);

    #if TESTER
    printf("HistorySize = %d\n", HistorySize);
    #endif


    //Read in history
    readInHistory(&Histhead, HistorySize);


    while(1){
        //prompt user
  		prompt();
        //read input
  		getInput(s);

        //Check for quite message
  		if(strcmp(s,"quit") == 0){
            //clear out linked list as to not leak any memory
            clearHistoryList(&Histhead);
            clearList(&head);
            free(path);
        	exit(1);
        }


        //This is for adding to the history list
        if(strcmp(prev,s) != 0 && !isHisNumCommand(s)){
            //add current command to history
            FILE * fout = fopen(".mssh_history", "a");
            fprintf(fout,"%s\n", s);
            addHistory(&Histhead, s, HistorySize);
            //save last command
            strcpy(prev,s);
            fclose(fout);
        }

        //count total pipes
      	totalPipes = countPipes(s);
      	
        #if TESTER
    	   printf("---->DEBUG:totalPipes = %d<-------\n", totalPipes);
    	#endif

        MainCommandControl(s,prev, Histhead, &head, totalPipes, path);
  	}


	return 0;
}






/*
    This was moved down here to make main a bit easier to follow/read
*/
void MainCommandControl(char * s, char * prev, History * Histhead, Node ** head, int totalPipes, char * path){
    int prePipeCount = 0;
    int postPipeCount = 0;
    
    char **prePipe = NULL;
    char **postPipe = NULL;

     if(isHisNumCommand(s)){
            if(strncmp(s,"!!",2) == 0){
                MainCommandControl(prev, prev, Histhead, head, totalPipes, path);
            }
            else
                runCommandNumber(Histhead, parseHistoryNumber(s), path);
            //check for alias    
        }else if(isAliasCMD(s)){
            #if TESTER 
            printf("THIS IS AN ALIAS COMMAND!!\n");
            #endif

            //parse and add alias to linked list
            parseAliasCMD(head, s);
            
        }else if( isPrintHistory(s)){
            #if TESTER
                printf("THIS IS AN printHistory COMMAND\n");
            #endif

            printHistoryList(Histhead);
        }else if (isUnaliasCMD(s)){
            #if TESTER
                printf("THIS IS AN UNALIAS COMMAND\n");
            #endif

            //parse and remove alias from linked list
            parseRemoveAlias(head,s);

        } else if(totalPipes > 0){
            pipeRun(s, totalPipes, prePipe, postPipe, prePipeCount, postPipeCount);
        } else if( totalPipes == -1){
            
            //Check for alias command
            if( findAlias(head,s) != NULL)
                runAlias(*head,s, path);
            else{
            //NOTE: The naming convention on the next two lines is silly
            //    There aren't any pipes in those commands being run in
            prePipeCount = makeargs(s, &prePipe);
            runCommand( prePipe, prePipeCount, path);  
            }              
        

        }

}