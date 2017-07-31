#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <errno.h>

using std::cout;
using std::endl;

int main (const int argc, const char * argv[]){

  //checks for the correct amount of arguments
  if (argc != 3) {
    cout << "Error: Invalid arguments" << endl;
    exit(0);
  }

  //renames the file supplied with the new name and path 
  //that was supplied. If failed, errno is set
  if(rename(argv[1], argv[2]) != 0)
    perror(argv[0]);

  return EXIT_SUCCESS;

}// main


