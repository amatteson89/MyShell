/*
*   Name: Andrew Matteson
*   Date: April 17, 2013
*   Course: Operating Systems
*   
*   Homework 1: Writing a basic shell
*/

#include "helper.h"

#define TESTER 0

void strip(char *s)
{
  int len;
  len = strlen(s);
  if(s[len - 2] == '\r')
    s[len -2] = '\0';

  else if(s[len - 1] == '\n')
    s[len -1] = '\0';
}// end strip


int makeargs(char *s, char *** argv){

    int wordCount = -1;
    char * temp = NULL;
    char tempS[1000];

    strcpy(tempS,s);

    temp = strtok(s," ");
    wordCount = 0;

  //Get the word count
    while(temp != '\0'){
        temp = strtok(NULL," ");
        wordCount++;
    }

    strcpy(s, tempS);

    *argv = (char**) calloc(wordCount + 1, sizeof(char*));

    temp = strtok(s, " ");
    
    for(int i = 0; i < wordCount; i++){
        *(*argv + i) = (char*) calloc(strlen(temp) + 1, sizeof(char));
        strcpy(*(*argv + i), temp);
        temp = strtok(NULL, " ");
    }


    if(wordCount == 0)
        wordCount = -1;

    return wordCount;

}// end makeArgs


int makePathargs(char *s, char *** argv){
    int wordCount = -1;
    char * temp = NULL;
    char tempS[1000];

    strcpy(tempS,s);

    temp = strtok(s,":");
    wordCount = 0;

  //Get the word count
    while(temp != '\0'){
        temp = strtok(NULL,":");
        wordCount++;
    }

    strcpy(s, tempS);

    *argv = (char**) calloc(wordCount + 1, sizeof(char*));

    temp = strtok(s, ":");
    
    for(int i = 0; i < wordCount; i++){
        *(*argv + i) = (char*) calloc(strlen(temp) + 1, sizeof(char));
        strcpy(*(*argv + i), temp);
        temp = strtok(NULL, ":");
    }


    if(wordCount == 0)
        wordCount = -1;

    return wordCount;
}


void printargs(int argc, char **argv)
{
    for(int x = 0; x < argc; x++)
        printf("%s\n", argv[x]);

}// end printargs


void clean(int argc, char **argv){

    for(int i = 0; i < argc; i++)
        free(*(argv + i));

    free(argv);
}//end clean

void getInput(char *temp){
    char c;
    int i = 0;


    do{
        c = getchar();
        *(temp + i) = c;
        //printf("DEBUG: i = %d, c = %c\n", i, c);
        i++;
    } while(c != '\n');

    //Force the last character to be \0
    //Note that we have i - 1 to override the newline character
    *(temp + i - 1) = '\0';

    //printf("DEBUG: %s\n", temp);
}//end getInput

void prompt(){
    printf("?:");
}

void errorMessage(){
    printf("ERROR! Command not found!\n");
}



void parsePipe(int totalPipes,char * s, char *** prePipe, int * prePipeCount, char *** postPipe, int *postPipeCount){
    char * temp = NULL;
    char tempS[1000];

    //Make a copy because strtok butchers strings
    strcpy(tempS,s);

    temp = strtok(s , "|");

    #if TESTER
    printf("---->DEBUG:temp = %s<-------\n", temp);
    #endif 

    *prePipeCount = makeargs(temp, prePipe);
    
    #if TESTER
    printf("---->DEBUG: prePipeCount = %d<---\n", *prePipeCount);
    #endif

    //replace the s string to original glory
    strcpy(s, tempS);

    temp = strtok(NULL , "|");
    #if TESTER
    printf("---->DEBUG:temp = %s<-------\n", temp);
    #endif

    *postPipeCount = makeargs(temp, postPipe);
    #if TESTER
    printf("---->DEBUG: postPipeCount = %d<---\n", *postPipeCount);
    #endif

}

int countPipes(char * s){

    int result = 0;

    for(int i = 0; i < strlen(s); i++){
        if(s[i] == '|'){
            result++;
        }
    }    

    if(result == 0){
        result = -1;
    }

    return result;
}




void pipeRun(char * s, int totalPipes, char ** prePipe, char ** postPipe, int prePipeCount, int postPipeCount){
    int fd[2];

    pid_t pid1;
    pid_t pid2;


    parsePipe(totalPipes, s, &prePipe, &prePipeCount, &postPipe, &postPipeCount);
      
    #if TESTER
    printf("------->DEBUG: in main prePipeCount = %d<------\n", prePipeCount);
    printf("------->DEBUG: in main postPipeCount = %d<------\n", postPipeCount);
      

    printargs(prePipeCount, prePipe);
    printargs(postPipeCount, postPipe);
    #endif

    if( (pid1 = fork()) == 0){
        //in child 1 
        
        pipe(fd);

        if( (pid2 = fork()) == 0){
          //child 2
          close(fd[0]);
          close(1);
          close(2);
          dup2(fd[1], 1);
          close(fd[1]);

          //run first command
          execvp(prePipe[0],prePipe);
          exit(-1);
        } //end if

        waitpid(pid2, NULL, 0);
        
        close(fd[1]); // stop write end of pipe
        close(0); 
        dup2(fd[0],0);
        close(fd[0]);

        //run 2nd command
        execvp(postPipe[0], postPipe);
        exit(-1);
    }//end if

    waitpid(pid1, NULL,0);

    clean(prePipeCount, prePipe);
    clean(postPipeCount, postPipe);

}


void runCommand(char ** command, int argc, char * path){
   pid_t forkID;
   char * finalPath = NULL;


    if( (forkID = fork()) == 0){
        #if TESTER
        printf("DEBUG::IN CHILD\n");
        #endif
        
    

        /* THIS IS FOR OTHER ARGUMENTS)*/
        char ** pathArgs = NULL;
        int numPathArgs = makePathargs(path, &pathArgs );

        for(int i = 0; i < numPathArgs; i++ ){
            
            AppendPath(&finalPath, *(pathArgs + i), *command);

            #if TESTER
            printf("DEBUG:::: finalPath = %s\n", finalPath);
            #endif
            execv( *(pathArgs + i), command);
            free(finalPath);
        }
            

        execvp(command[0],command);    
        //we want to clean the memory if it doesn't run
        clean(argc, command);
        errorMessage();
        exit(-1);
    
    }

    waitpid(forkID, NULL, 0);
    free(finalPath);
    clean(argc, command);

}


void AppendPath(char ** result, char * path, char * command){
    *result = (char *) malloc( snprintf(NULL, 0, "%s/%s", path, command) + 1);

    sprintf(*result, "%s/%s", path, command);

}
