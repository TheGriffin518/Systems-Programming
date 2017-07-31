#include <string.h>
#include <cstdlib>

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include "command.h"

#define MAXLINE 80

using std::cout;
using std::endl;


/*
 *Clears the parseInfo structure to allow for more commands 
 *and allow the proper parsing of information.
 */

void init_info(parseInfo *pp){
  int i;
  pp -> boolInput=0;
  pp -> boolOutput=0;
  pp -> boolBckgrnd=0;
  pp -> pipeNum=0;

  for (i = 0; i < PIPE_MAX; ++i) {
    pp -> CommArray[i].command = NULL;
    pp -> CommArray[i].VarList[0] = NULL;
    pp -> CommArray[i].VarNum = 0;
  } // for

} // init_info


/*
 *Takes in a command and parses the information into a structure to be 
 *used later on in the 1730sh file
 */
int parse_cmd(char * command, struct commandType * cmd_st) {
  int i = 0;
  int pos = 0;
  char word[MAXLINE];
  
  cmd_st -> VarNum = 0;
  cmd_st -> command = NULL;
  cmd_st -> VarList[0] = NULL;

  while (isspace(command[i])) {
    i++;
  } // while
 
  if (command[i] == '\0') {
    return 1;
  } // if

  while (command[i] != '\0') {
    while ((command[i] != '\0') && (!isspace(command[i])))
      {
	word[pos++] = command[i++];
      }// while ((command[i] != '\0'

    if (cmd_st -> VarNum == MAX_VAR) {
      cout << "Too many arguments." << endl;
      return 0;
    } //if 

    cmd_st -> VarList[cmd_st->VarNum] =(char*) malloc((strlen(word)+1)*sizeof(char));
    strcpy(cmd_st->VarList[cmd_st->VarNum], word);
    cmd_st->VarNum++;
    word[0] = '\0';
    pos = 0;
    while(isspace(command[i])){
      i++;
    } // while(isspace)

  } // while

    cmd_st->command = (char*)malloc((strlen(cmd_st->VarList[0])+1)*sizeof(char));
    strcpy(cmd_st->command, cmd_st->VarList[0]);
    cmd_st->VarList[cmd_st->VarNum] = NULL;
    return 1;
 
} // parse_cmd

/*
 *Parses the input that the user supplies. Takes in a char pointer, which is a 
 *c-string. It then return
 */ 

parseInfo *parse(char *cmdline) {
  parseInfo *Result;
  int i = 0;
  int pos;
  int end = 0;
  char command[MAXLINE];
  int com_pos;
  int iscommproper = 0;

  if(cmdline[i] == '\n' && cmdline[i] == '\0')
    return NULL;

  Result = (parseInfo*)malloc(sizeof(parseInfo));
  if (Result == NULL) {
    return NULL;
  } // if

  init_info(Result);
  com_pos = 0;
  while (cmdline[i] != '\n' && cmdline[i] != '\0') {

    if (cmdline[i] == '&' ) {
      Result -> boolBckgrnd = 1;
      
      if (cmdline[i] != '\n' && cmdline[i+1] != '\0') {
	cout << "Ignore anything beyond &." << endl;
      } // if
      break;
    } //if (cmdlin[i])
    
    else if (cmdline[i] == '<') 
      {
	Result->boolInput = 1;
	while(isspace(cmdline[++i]));
	pos = 0;
	
	while (cmdline[i] != '\0' && !isspace(cmdline[i])) 
	  {
	    if( pos == FILE_MAX )
	      {
		cout << "Error. The input redirection "
		     <<	"file name exceeds the size limit 40" 
		     << endl;
		free_info( Result );
		return NULL;
	      }
	    Result->inFile[pos++] = cmdline[i++];
	  } // while cmdline[i] !=

	Result->inFile[pos]='\0';
	end = 1;

	while( isspace(cmdline[i])) 
	  {
	    if( cmdline[i] == '\n' )
	      break;
	    i++;
	  } // isspace(cmdline)
      

      } // else if cmdline == <


    else if (cmdline[i] == '>') 
      {
	Result->boolOutput=1;
	while (isspace(cmdline[++i]));
	pos=0;
	while( cmdline[i] != '\0' && !isspace(cmdline[i])) 
	  {
	    if( pos==FILE_MAX) 
	      {
		fprintf( stderr, "Error.The output redirection file "
			 "name exceeds the size limit 40\n");
		free_info(Result);

		return NULL;
	      }
	    Result->outFile[pos++] = cmdline[i++];
	  }

	Result->outFile[pos]='\0';
	end = 1;
	while( isspace(cmdline[i])) 
	  {
	    if( cmdline[i] == '\n' )
	      break;
	    i++;
	  }
      } // else if >

    else if (cmdline[i] == '|') 
      {
	command[com_pos]='\0';
	iscommproper = parse_cmd( command, &Result->CommArray[Result->pipeNum]);
	if( ! iscommproper ) 
	  {
	    free_info( Result );
	    return NULL;
	  }

	com_pos = 0;
	end = 0;
	Result->pipeNum++;
	i++;
      } // else if cmdline |

    else 
      {
	if( end == 1 )
	  {
	    fprintf( stderr, "Error.Wrong format of input\n" );
	    free_info( Result );
	    return NULL;
	  }

	if( com_pos == MAXLINE-1 )
	  {
	    fprintf( stderr, "Error. The command length exceeds "
		     "the limit 80\n" );
	    free_info( Result );
	    return NULL;
	  }

	command[com_pos++] = cmdline[i++];

      } // else

  }// while cmdline != \0 && whitespace  
    command[com_pos] = '\0';
    
    iscommproper = parse_cmd(command, &Result->CommArray[Result->pipeNum]);
    if(!iscommproper){
      free_info(Result);
      return NULL;
    }

    return Result;

} // *parse

/*
 *Frees up the memory that was allocated for the command struct
 *
 */
void free_info( parseInfo *info) {
  int i, j;
  struct commandType *comm;

  if(info == NULL) 
    return;
  for(i = 0; i < PIPE_MAX; ++i) {
    comm = &(info->CommArray[i]);
    for(j = 0; j < comm->VarNum; ++j) {
      free(comm->VarList[j]);
    }
    if(comm->command != NULL){
      free(comm->command);
    }
  }
  free(info);
} // free_info
