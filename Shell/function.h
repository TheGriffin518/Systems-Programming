



void pipeFunction(char *cmd);
void execOutput(char *cmd);
void execInput(char *cmd);
void execBoth(char *cmd);
void outRedirect(char **com, char *outfile);
void inRedirect(char **com, char *infile);
void bothRedirect(char **com, char *infile, char *outfile);
void execeHist(void);
void execHelp(void);
void execJobs(void);
void execCD(void);
void parseCmd(char *command, char **argv);
void execPipe(char *cmd1, char *cmd2);
void nope_out(const std::string & sc_name);
void close_pipe(int pipefd [2]);
