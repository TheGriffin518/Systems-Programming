#ifndef COMMAND_H
#define COMMAND_H

#define MAX_VAR 15
#define PIPE_MAX 15
#define FILE_MAX 45


struct commandType {
  char *command;
  char *VarList[MAX_VAR];
  int VarNum;
};



typedef struct {
  int boolInput;
  int boolOutput;
  int boolBckgrnd;

  struct commandType CommArray[PIPE_MAX];
  int pipeNum;
  char inFile[FILE_MAX];
  char outFile[FILE_MAX];
} parseInfo;

parseInfo *parse(char *);
void free_info(parseInfo *);
void print_info(parseInfo *);
int parse_cmd(char * command, struct commandType *comm);

#endif
