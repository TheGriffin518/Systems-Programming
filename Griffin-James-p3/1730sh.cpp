#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "command.h"
#include "function.h"

#define HISTORY_MAX 25
#define MAX_ARGS 25

using std::cout;
using std::endl;
using std::cin;
using std::string;

enum BUILTINS {JOBS, EXIT, HELP, HISTORY, CD, FG, BG, };

char *prompt;
char cwd[128];

static char *history[HISTORY_MAX];
static unsigned historyCounter = 0;


/*
 *Keeps track of the user's history to allow for previous commands to be used
 *for IO redirection. 
 */
void addToHistory(char *command) {
  if (historyCounter < HISTORY_MAX) {
    history[historyCounter++] = strdup(command);
  }
  else {
    free(history[0]);
    unsigned index;
    for(index = 1; index < HISTORY_MAX; index++) {
      history[index-1] = history[index];
    }
    history[HISTORY_MAX - 1] = strdup(command);
  }
}

/*
 *Tests to see whether the user inputted a command that is built in.
 *Runs that command if it is built in. Not all of these have been implemented
 */
int isBuiltInCommand(char *cmd) {
  if(strcmp(cmd, "exit") == 0) return EXIT;
  else if(strcmp(cmd, "jobs") == 0) return JOBS;
  else if(strcmp(cmd, "history") == 0) return HISTORY;
  else if(strcmp(cmd, "help") == 0) return HELP;
  else if(strcmp(cmd, "cd") == 0) return CD;
  else if(strcmp(cmd, "bg") == 0) return BG;
  else if(strcmp(cmd, "fg") == 0) return FG;

  return 0;
}

/*
 *Changes to the directory that the user specifies
 */
void execCD(char *path){

}

/*
 *Displays the history of commands that the user has used. Is executed with
 *history being passed into command line
 */
void execHist(void){
  int num = 0;
  cout << "History" << endl;

  for(unsigned int i = 0; i < sizeof(history); ++i) {
    if (history[i] == NULL)
      i = sizeof(history);
    else{
      cout << num << " " << history[i] << endl;
      num++;
    }
      
  }

} // executeHistoryCommand

/*
 *Displays a list of functions that the user can do
 */
void execHelp(void) {
  cout << "The following are builtin commands:\n"
       << "exit\n"
       << "help\n"
       << "history\n"
       << "cd [directory]\n"
       << "jobs\n"
       << endl;
} // executeHelpCommand

/*
 *Parses the command line that the user inputted. Allows for the tokenizing 
 *the command into pieces that are more easily handled.
 */
void parseCmd(char *command, char **argv) {
  while (*command != '\0') {
    while (*command == ' ' || *command == '\t')
      *command++ = '\0';
    *argv++ = command;
    while(*command != '\0' && *command != ' ' && *command != '\t' && *command != '\n')
      command++;
  }

  *argv = NULL;

} // parseCmd

/*
 *executes the command that the user passed in
 */
void execCommand(char *cmd) {
  char *argv[MAX_ARGS];

  parseCmd(cmd, argv);
  
  execvp(*argv,argv);

  cout << "Unknown Command" << endl;
  exit(0);

} //executeCommand


/*
 *executes the background command. 
 */
void execBack(char *cmd) {
  char *argv[MAX_ARGS];
  char *argTemp[5];

  parseCmd(cmd, argv);

  for(unsigned int i = 0; i < sizeof(argv); ++i) {
    if (strcmp(argv[i], "&") == 0){
      argTemp[i] = NULL;
      execvp(argTemp[i], argTemp);
    }
    else
      argTemp[i] = argv[i];
  }

}

/*
 *executes the output file command. Selects what the input file is for IO
 */
void execOutput(char *cmd) {
  char *argv[MAX_ARGS];
  char *argTemp[5];
  char *outfile;
  
  parseCmd(cmd,argv);

  for(unsigned int i = 0; i < sizeof(argv); ++i) {
    if (strcmp(argv[i], ">") == 0) {
      argTemp[i] = NULL;
      outfile = argv[i+1];
      outRedirect(argTemp, outfile);
    }
    else
      argTemp[i] = argv[i];
  }
} 


/*
 *executes the input file command. Selects what the input file is for IO
 */
void execInput( char *cmd) {
  char *argv[MAX_ARGS];
  char *argTemp[5];
  char *infile;
  
  parseCmd(cmd,argv);

  for(unsigned int i = 0; i < sizeof(argv); ++i) {
    if (strcmp(argv[i], "<") == 0) {
      argTemp[i] = NULL;
      infile = argv[i+1];
      inRedirect(argTemp, infile);
    }
    else
      argTemp[i] = argv[i];
  }
}

/*
 *Used for redirection of  in and out. The input from command is parsed
 */
void execBoth(char *cmd) {
  char *argv[MAX_ARGS];
  char *argTemp1[5];
  char *infile, *outfile;
  
  parseCmd(cmd,argv);

  for(unsigned int i = 0; i < sizeof(argv); ++i) {
    if (strcmp(argv[i], "<") == 0) {
      argTemp1[i] = NULL;
      infile = argv[i+1];
      outfile = argv[i+3];
      bothRedirect(argTemp1, infile, outfile);
    }
    else
      argTemp1[i] = argv[i];
  }

} // execute Both


/*
 *Used for redirection out and in. The input is read from command and is parsed
 */
void bothRedirect(char **com, char *infile, char *outfile) {
  int fd1, fd2;

  fd1 = open(outfile, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  fd2 = open(infile, O_RDWR, S_IRUSR | S_IWUSR);

  if(fd1 < 0) {
    perror("Open error");
    exit(1);
  }

  if(fd2 < 0) {
    perror("No such file");
    exit(1);
  }

  dup2(fd2, STDIN_FILENO);
  dup2(fd1, STDOUT_FILENO);

  close(fd1);

  execvp(com[0], com);
  cout << "Unknown Command" << endl;
} // bothRedirect


/*
 *Redirects the output of the file or command into a file.
 */
void outRedirect( char **com, char *outfile) {
  int fd;
  fd = open(outfile, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

  if(fd < 0) {
    perror("Open error");
    exit(1);
  }

  dup2(fd, STDOUT_FILENO);
  close(fd);
  execvp(com[0], com);
  cout << "Unknown Command" << endl;

} // outRedirect

/*
 *
 */
void inRedirect(char **com, char *infile) {
  int fd;
  com[sizeof(com)-1] = NULL;

  fd = open(infile, O_RDWR, S_IRUSR | S_IWUSR);

  if(fd < 0){
    perror("Open error");
    exit(1);
  }

  dup2(fd, STDIN_FILENO); // Copy fileID to stdout
  execvp(com[0], com);

  cout << "Unknown Command" << endl;
}

/*
 *Parses the commands and determines the pipe directions. 
 */
void pipeFunction(char *cmd) {
  char *argv[MAX_ARGS];

  parseCmd(cmd, argv);
  execPipe(argv[0], argv[2]);
}

/*
 *When things go bad, this nopes out for you
 */
void nope_out(const string & sc_name) {
  perror(sc_name.c_str());
  exit(EXIT_FAILURE);
}

/*
 *Executes the pipe between the commands. I got this from Cotterell's syscalls
 *github
 */
void execPipe(char *cmd1, char *cmd2) {
  int pipefd[2];
  int pid;

  // create pipe
  if (pipe(pipefd) == -1) nope_out("pipe");

  if ((pid = fork()) == -1) {
    nope_out("fork");
  } else if (pid == 0) {
    if (dup2(pipefd[1], STDOUT_FILENO) == -1) nope_out("dup2");
    close_pipe(pipefd);
    execlp(cmd2, cmd2, (char *) 0);
  } // if

  if ((pid = fork()) == -1) {
    nope_out("fork");
  } else if (pid == 0) {
    if (dup2(pipefd[0], STDIN_FILENO) == -1)  nope_out("dup2");
    close_pipe(pipefd);
    execlp(cmd1, cmd1, (char *) 0);
  } // if

  close_pipe(pipefd);
  waitpid(pid, nullptr, 0);

} // executePIPEFunction


/*
 *Closes the pipe after executing the pipe systemc call. 
 */
void close_pipe(int pipefd [2]) {
  if (close(pipefd[0]) == -1) nope_out("close");
  if (close(pipefd[1]) == -1) nope_out("close");
} // close_pipe

/*
 *Displays the prompt, reads user input and then parses it and handles 
 *some fo the builtin functions. 
 */
int main() {

  std::fstream my_stream;
  char buffer[50];

  char * cmdLine;
  parseInfo *info;
  struct commandType *com;
  pid_t childPID;

  getcwd(cwd, sizeof(cwd));
  prompt = strcat(cwd, ": ");

  system("clear");
  while (1) {

    cout << cwd << ":$ ";
    //    my_stream.getline(buffer, sizeof buffer);
    //    cmdLine = buffer;

    cin.getline(buffer,128);
    cmdLine = buffer;
    if(cmdLine == NULL) {
      cout << "Unable to read command" << endl;
      exit(1);
      break;
    }

    addToHistory(cmdLine);

    if(strcmp(cmdLine, "exit") == 0){
      break;
    }

    info = parse(cmdLine);
    if(info == NULL) {
      continue;
    }

    com = &info->CommArray[0];
    if((com == NULL) || (com->command == NULL)){
      free_info(info);
      continue;
    }

    if(isBuiltInCommand(com->command) == EXIT) 
      exit(1);
    else if(isBuiltInCommand(com->command) == HISTORY) 
      execHist();
    else if(isBuiltInCommand(com->command) == HELP) 
      execHelp();
    else if(info->boolOutput == 1 && info->boolInput == 1){
      childPID = fork();
      if (childPID == 0){
	execBoth(cmdLine);
      }
      else{
	wait((int *) 0);
      }
    }
    else if (info->boolBckgrnd == 1) {
      childPID = fork();
      if(childPID == 0){
	pause();
	execBack(cmdLine);
      }
      
    }

    else if (info->boolOutput == 1){
      childPID = fork();
      if(childPID == 0) {
	execOutput(cmdLine);
      }
      else 
	wait((int *)0);
    }
    else if(info->boolInput ==1 ) {
      childPID = fork();
      if (childPID == 0) {
	execInput(cmdLine);
      }
      else 
	wait((int *) 0 );
    } 

    else if (info->pipeNum > 0) {
      pipeFunction(cmdLine);
    }
    else{
      childPID = fork();
      if(childPID == 0) {
	execCommand(cmdLine);
      }
      else
	wait((int *) 0);
    }

    free_info(info);

    cout << "\n";
    
  } // while


} // main
