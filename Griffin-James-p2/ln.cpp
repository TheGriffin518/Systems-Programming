#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <cstdlib>
#include <string.h>

using std::cout;
using std::endl;

int main (const int argc, const char * argv [] ) {

  char opt [] = "-s";

  //checks for the correct amount of arguments
  if (argc < 3) {
    cout << "Error: " << argv[0] << "  Not enough arguments supplied" << endl;
  }  else {
    if(argc == 3) {
      if(link(argv[1], argv[2]) != 0) //attempts to hard link the files together 
	perror(argv[0]);
    }
    else {
      if(strcmp(argv[1], opt) == 0){
	if(symlink (argv[2], argv[3]) != 0)//attempts to symbolically link the files together
	  perror(argv[0]);
      }
    } //argc == 4
  }// if
  
  return EXIT_SUCCESS;

} // main 
