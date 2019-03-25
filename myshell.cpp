/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   myshell.cpp
 * Author: james
 *
 * Created on March 22, 2019, 3:38 PM
 */

#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <vector>
#include "string.h"
#include "fcntl.h"
/*added some comment for week 09 */
#include "common.h"

#define SPACE 32
#define NEWLINE 10

#define INPUT_SIZE 100
#define HIST_SIZE 50

using namespace std;

double version;

void displayVersion();


int count(string s, char c) 
{ 
    // Count variable 
    int res = 0; 
    
   
  
    for (int i=0;i<s.size();i++) 
  
        // checking character in string 
        if (s[i] == c) 
            res++; 
  
    return res; 
} 


/*
 *
 * this program will multiple commands
 * it uses the fork process to execute one of these commands and then
 * waits until the first is finished to run n+1
 * 
 *  
 */
int main() {
    
    int processId; 
    int processId2;
    int processId3;
    string userInput;
    string  history;
    int status;
    int numBlank = 0;
    
    //constant string
    const string exitCommand = "exit";
    const string historyCommand = "history";  //n
    string endOfLine ="\n";
    
    version = 1.2;
    displayVersion();   
           
    while(1) {
       
        
        cout << "myshell>" << endl; 
	        
        fflush(stdout);
        
     
        getline(cin,userInput);
        
        char inputArray[sizeof(userInput)+ 1];
	userInput.copy(inputArray, sizeof(userInput) + 1);
	inputArray[sizeof(userInput)] = '\0';
          
               
        history.append(userInput);
        
        if(userInput.compare(exitCommand) == 0) {
	   
            cout << "Exit" << endl; ;
	    fflush(stdout);
            return 0;
        }
        
        if(userInput.compare(historyCommand) == 0) {
            
            cout << history << endl;;
	    fflush(stdout);
            continue;
        }
        
        if(userInput.compare("\n") == 0) {
           
            numBlank++;
        } 
        else
            numBlank = 0;
        
        if(numBlank == 1)
            continue;
        
        
        if(numBlank == 2) {
            
            userInput = "ls\n";
            numBlank = 0;
        }
        
        int counter = 2; 

       
        for(int i = 0; inputArray[i] != NEWLINE; i++) {
            
            if(inputArray[i] == SPACE) { // space separates tokens
             
                counter++;
            }
        }
       
        counter++;
        
        
        if(strchr(inputArray, ';') != NULL) {
        
            char *fCommand[counter];
            *fCommand = (char*)calloc(counter, INPUT_SIZE);
            
            char *sCommand[counter];
            *sCommand = (char*)calloc(counter, INPUT_SIZE);
            
            fCommand[0] = strtok(inputArray, ";");
            
            sCommand[0] = strtok(NULL, ";");
            
           
            char *fArguments[counter];
            *fArguments = (char*)calloc(counter, INPUT_SIZE);
            fArguments[0] = strtok(fCommand[0], " ");
            
            for (int i = 1; i < counter; i++) {
                
                fArguments[i] = strtok(NULL, " ");
            }

           
            
            char *sArguments[counter];
            *sArguments = (char*)calloc(counter, INPUT_SIZE);
            sArguments[0] = strtok(sCommand[0], " ");
            for (int i = 1; i < counter; i++) {
                sArguments[i] = strtok(NULL, " ");
            }

                      
            // null-terminate the arguments
            for(int i = 0; fArguments[i] != NULL; i++) {
                
                for(int j = 0; j < counter; j++) {
                    if(fArguments[i][j] == '\n')
                        fArguments[i][j] = '\0';
                }
            }
            for(int i = 0; sArguments[i] != NULL; i++) {
               
                for(int j = 0; j < counter; j++) {
                    
                    if(sArguments[i][j] == '\n')
                        sArguments[i][j] = '\0';
                }
            }  

            if((processId = fork()) < 0) {
              
                perror("Something went wrong in fork");
               
                return -1;
            } 
            else if(processId == 0) { // child
                // start first process
                execvp(fArguments[0], fArguments);
                
                return -1;
                
            } else { // parent
                
                wait(&status); // wait until child terminates
                
                if((processId2 = fork()) == 0) {
                    
                    // then start second process
                    execvp(sArguments[0], sArguments);
                    return -1;
                } 
                else {
                   
                    wait(&status);
                    continue;
                }
            }
        } 
        else if(strchr(inputArray,'|') != NULL) {
            
           
            char *fCommand[counter];
            *fCommand = (char*)calloc(counter, INPUT_SIZE);
            
            char *sCommand[counter];
            *sCommand = (char*)calloc(counter, INPUT_SIZE);
            
            char *tCommand[counter];
            *sCommand = (char*)calloc(counter, INPUT_SIZE);
            
            fCommand[0] = strtok(inputArray, "|");
            sCommand[0] = strtok(NULL, "|");
            tCommand[0] = strtok(sCommand[0], "|");
            
            int fd[3];
            
                       
            char *fArguments[counter];
            *fArguments = (char*)calloc(counter, INPUT_SIZE);
            fArguments[0] = strtok(fCommand[0], " ");
           
            for (int i = 1; i < counter; i++) {
                fArguments[i] = strtok(NULL, " ");
            }

            
            char *sArguments[counter];
            *sArguments = (char*)calloc(counter, INPUT_SIZE);
            sArguments[0] = strtok(sCommand[0], " ");
            for (int i = 1; i < counter; i++) {
                sArguments[i] = strtok(NULL, " ");
            }
            
            char *tArguments[counter];
            *tArguments = (char*)calloc(counter, INPUT_SIZE);
            tArguments[0] = strtok(sCommand[0], " ");
            for (int i = 1; i < counter; i++) {
                tArguments[i] = strtok(NULL, " ");
            }
      
      
            // null-terminate the arguments
            for(int i = 0; fArguments[i] != NULL; i++) {
               
                for(int j = 0; j < counter; j++) {
                   
                    if(fArguments[i][j] == '\n')
                        fArguments[i][j] = '\0';
                }
            }
                
            for(int i = 0; sArguments[i] != NULL; i++) {
               
                for(int j = 0; j < counter; j++) {
                    
                    if(sArguments[i][j] == '\n')
                        sArguments[i][j] = '\0';
                }
            }
            
            for(int i = 0; tArguments[i] != NULL; i++) {
               
                for(int j = 0; j < counter; j++) {
                    
                    if(tArguments[i][j] == '\n')
                        tArguments[i][j] = '\0';
                }
            }
            
            if((processId = fork()) < 0) {
               
                perror("Something went wrong in fork");
                return -1;
                
            } 
            else if(processId == 0) { // child
            	
                pipe(fd);
            	processId2 = fork();
              	
                if(processId2 < 0) {
                    
            		perror("Something went wrong in fork.");
            		return -1;
            	}
                
                
                if(processId3 < 0) {
                    
            		perror("Something went wrong in fork.");
            		return -1;
            	}
                
                
            	
                if(processId2) { 
                                 
            		wait(&processId);
            		close(fd[1]);	// close write side of pipe
            		close(0);	// close STDIN
            		dup2(fd[0], 0);
            		execvp(sArguments[0], sArguments);
            	}
            	
                if(processId == 0) {
            		close(fd[0]);	// close read side of pipe
            		close(1);	// close STDOUT
            		dup2(fd[1], 1);
            		execvp(fArguments[0], fArguments);
            	}
                
            } 
            else { // parent
               
                wait(&status); // make parent wait until child terminates
                continue;
            }

        } 
        else if(strchr(inputArray,'<') != NULL){   
                       
            char *fCommand[counter];
            *fCommand = (char*)calloc(counter, INPUT_SIZE);
            
            char *sCommand[counter];
            *sCommand = (char*)calloc(counter, INPUT_SIZE);
            
            fCommand[0] = strtok(inputArray, "<");
            sCommand[0] = strtok(NULL, "<");
            
            int fd[2];
            
           
            
            char *fArguments[counter];
            *fArguments = (char*)calloc(counter, INPUT_SIZE);
            fArguments[0] = strtok(fCommand[0], " ");
           
            for (int i = 1; i < counter; i++) {
                fArguments[i] = strtok(NULL, " ");
            }

            
            char *sArguments[counter];
            *sArguments = (char*)calloc(counter, INPUT_SIZE);
            sArguments[0] = strtok(sCommand[0], " ");
            for (int i = 1; i < counter; i++) {
                sArguments[i] = strtok(NULL, " ");
            }
      
            // null-terminate the arguments
            for(int i = 0; fArguments[i] != NULL; i++) {
               
                for(int j = 0; j < counter; j++) {
                   
                    if(fArguments[i][j] == '\n')
                        fArguments[i][j] = '\0';
                }
            }
                
            for(int i = 0; sArguments[i] != NULL; i++) {
               
                for(int j = 0; j < counter; j++) {
                    
                    if(sArguments[i][j] == '\n')
                        sArguments[i][j] = '\0';
                }
            }
            
            if((processId = fork()) < 0) {
               
                perror("Something went wrong in fork");
                return -1;
                
            } 
            else if(processId == 0) { // child
            	
                pipe(fd);
            	processId2 = fork();
            	
                if(processId2 < 0) {
                    
            		perror("Something went wrong in fork.");
            		return -1;
            	}
            	
                if(processId2) { 
                                 
            		wait(&processId);
            		close(fd[1]);	// close write side of pipe
            		close(0);	// close STDIN
            		dup2(fd[0], 0);
            		//execvp(sArguments[0], sArguments);
                        open(sArguments[0],O_RDONLY);
            	}
            	
                if(processId == 0) {
            		close(fd[0]);	// close read side of pipe
            		close(1);	// close STDOUT
            		dup2(fd[1], 1);
            		execvp(fArguments[0], fArguments);
            	}
                
            } 
            else { // parent
               
                wait(&status); // make parent wait until child terminates
                continue;
            }
        }
        else if (strchr(inputArray,'>') != NULL){      //check for output >
            
            int delCount = count(inputArray,'>');
             
            char *fCommand[counter];
            *fCommand = (char*)calloc(counter, INPUT_SIZE);
            
            char *sCommand[counter];
            *sCommand = (char*)calloc(counter, INPUT_SIZE);
            
            fCommand[0] = strtok(inputArray, ">");
            sCommand[0] = strtok(NULL, ">");
            
            int fd[2];
            
           
            
            char *fArguments[counter];
            *fArguments = (char*)calloc(counter, INPUT_SIZE);
            fArguments[0] = strtok(fCommand[0], " ");
           
            for (int i = 1; i < counter; i++) {
                fArguments[i] = strtok(NULL, " ");
            }

            
            char *sArguments[counter];
            *sArguments = (char*)calloc(counter, INPUT_SIZE);
            sArguments[0] = strtok(sCommand[0], " ");
            for (int i = 1; i < counter; i++) {
                sArguments[i] = strtok(NULL, " ");
            }
      
            // null-terminate the arguments
            for(int i = 0; fArguments[i] != NULL; i++) {
               
                for(int j = 0; j < counter; j++) {
                   
                    if(fArguments[i][j] == '\n')
                        fArguments[i][j] = '\0';
                }
            }
                
            for(int i = 0; sArguments[i] != NULL; i++) {
               
                for(int j = 0; j < counter; j++) {
                    
                    if(sArguments[i][j] == '\n')
                        sArguments[i][j] = '\0';
                }
            }
            
            if((processId = fork()) < 0) {
               
                perror("Something went wrong in fork");
                return -1;
                
            } 
            else if(processId == 0) { // child
            	
                pipe(fd);
            	processId2 = fork();
            	
                if(processId2 < 0) {
                    
            		perror("Something went wrong in fork.");
            		return -1;
            	}
            	
                if(processId2) { 
                                 
            		wait(&processId);
            		close(fd[1]);	// close write side of pipe
            		close(0);	// close STDIN
            		dup2(fd[0], 0);
                        if(delCount > 1 )
                            open(sArguments[0],O_APPEND,0644);
                        else
                            open(sArguments[0],O_CREAT|O_TRUNC|O_WRONLY,0644);
            		
            	}
            	
                if(processId == 0) {
            		close(fd[0]);	// close read side of pipe
            		close(1);	// close STDOUT
            		dup2(fd[1], 1);
            		execvp(fArguments[0], fArguments);
            	}
                
            } 
            else { // parent
               
                wait(&status); // make parent wait until child terminates
                continue;
            }
                                                 
            
            
            
        }
        else { 
            
            counter = counter - 2;
            char *tokenInput[counter];
            tokenInput[counter] = NULL;
            
            tokenInput[0] = strtok(inputArray, " ");
            
            for(int i = 1; i < counter; i++) {
               
                tokenInput[i] = strtok(NULL, " ");
            }
            
                      
            // change the command to a concatenation of "which " and the command
            char str_cat[100];
            strcpy(str_cat, "which ");
            strcat(str_cat, tokenInput[0]);
            
            // execute the concatenated command to put the path of the command into
            // the first slot of the tokenizedInput array (the command).
            FILE *pipe;
            pipe = popen(str_cat, "r");
            fgets(str_cat, 100, pipe);
            tokenInput[0] = str_cat;
	    pclose(pipe);
              
            // strip newline character
            for(int i = 0; i < counter; i++) {
               
                for(int j = 0; j < strlen(tokenInput[i]); j++) {
                    
                    if(tokenInput[i][j] == '\n') {
                       
                        tokenInput[i][j] = '\0';
                    }
                }
            }
            
            tokenInput[counter] = NULL; // null terminate
            
            if((processId = fork()) < 0) {
                perror("Error in the forked process");
                return -1;
            } 
            else if (processId == 0) { // child
                
                if(execv(tokenInput[0], tokenInput) == -1)
                    perror("Error in execv");
		    
                return  -1;
            } 
            else { // parent
                
                waitpid(processId, NULL, 0);
                continue;
            }
        }
    }
    
    return 0;
}

