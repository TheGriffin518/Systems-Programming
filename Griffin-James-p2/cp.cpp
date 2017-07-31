#include <cstdlib>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>

#define BUFFSIZE 4096

using std::cout;
using std::endl;

int src_fd;
int trg_fd;

int copy_directory(const char*, const char*);

int main (const int argc, const char * argv[]) {

  char buf [BUFFSIZE];
  int n;
  char ropt [] = "-r";

  //checks for the correct number of arguments supplied
  if((argc != 3) && argc != 4){
    cout << argv[0]
	 << ": Incorrect arguments supplied"
	 << endl;
    exit(0);
  }
  else {
    if(argc == 3){
      if((src_fd = open(argv[1],O_RDONLY)) == -1){ //opens the file the user specified
	perror(argv[1]);
	exit(0);
      }//if src_fd
      else if ((trg_fd = open(argv[2], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH)) == -1){ //opens and creates a file with 
	perror(argv[2]);                                                                                             //with the name the user supplied
	exit(0);
      } //else if trg_fd
    }//if

    else {
      if(strcmp(argv[1], ropt) == 0){

      }
    } //argc == 4 w/ -R
      
  }//else

  /*
   *Reads the file to be copied into a buffer and then writes the buffer to the created file the user specified
   */
  while ((n = read(src_fd, buf, BUFFSIZE)) > 0) {
    if (write(trg_fd, buf, n) != n)
      cout << "write error!" << endl;
  } // while
  if(n < 0) 
    cout << "read error!" << endl;
  if(close(src_fd) != 0) //closes the file to be copied
    perror(argv[1]);
  if(close(trg_fd) != 0) //closes the written file
    perror(argv[2]);

  return EXIT_SUCCESS;
} //main

int copy_directory(const char * srcpath, const char* trgpath) {

  int y;
  DIR *dirp = opendir(srcpath);
  int path_len = strlen(srcpath);
  int r = -1;
  
  if (dirp){
    struct dirent *stdirp;
    r = 0;
    

    while(!r && (stdirp = readdir(dirp))) {
      //      int r2 = -1;
      char *buf;
      size_t len;

      if(!strcmp(stdirp->d_name,".") || !strcmp(stdirp->d_name,".."))
	{
	  continue;
	}

      len = path_len + strlen(stdirp->d_name) + 2;
      buf = (char*)malloc(len);

      if(buf){
	struct stat statbuf;

	snprintf(buf,len,"%s/%s",srcpath, stdirp->d_name);

	if(!stat(buf, &statbuf)) {
	  if(S_IFDIR & (statbuf.st_mode))
	    {

	    }
	  else //isfile
	    {
	      while ((y = read(src_fd, buf, BUFFSIZE)) > 0) {
		if (write(trg_fd, buf, y) != y)
		  cout << "write error!" << endl;
	      } // while

	    }//else
	}//if(!stat(buf,&statbuf))
      }//if(buf)
    }//while(!r && ...)
  }//if(dirp)
  return 0;
}
