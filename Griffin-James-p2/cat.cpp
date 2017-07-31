#include <iostream>
#include <cstdlib>
#include <errno.h>
#include <fcntl.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <unistd.h>

#define BUFFSIZE 4096 //defines number of bytes to read

using std::cout;
using std::endl;

int main(const int argc, const char * argv [] ) {

  int n; 
  int fd;
  char buf[BUFFSIZE]; //will be the buffer

  /*
   *checks to see if user supplied a file
   */
  if(argc != 2){
    cout << "Error:" << argv[0] << endl;
    exit(0);
  }
  /*
   *Sees if the file can be opened and read, if not errno is set
   */
  if ((fd = open(argv[1], O_RDONLY)) == -1)
    perror(argv[1]);

  /*
   *Reads the file to the buffer, and quits when there is no more to read
   */
  while (( n = read(fd, buf, BUFFSIZE)) > 0) {
    if (write(STDOUT_FILENO, buf, n) != n) //writes to standard out
      cout << "write error!" << endl;
  }
    if (n < 0) 
      cout << "read error!" << endl;

  return EXIT_SUCCESS;

}// main
