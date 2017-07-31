#include <sys/stat.h>
#include <sys/types.h>
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <string>

using std::cout;
using std::endl;

int main (const int argc, const char * argv []) {

  const char * path; //pathname
  const char p_opt [] = "-p";//option for creating required directories
  const char m_opt [] = "-m";//option to set the mode of directory with octal
  const char * oct_mode;
  const char eod [] = "/";
  std::string tmp;
  std::string pathname;


  //checks to see if an option was supplied
  if(argc == 2){ 
    path = argv[1];
    if(mkdir(path, 0766) != 0) {
      perror(argv[0]);
      exit(0);
    }
  }
  //checks to see if the option supplied was "-p"
  else if (argc == 3){
    if(strcmp(p_opt, argv[1]) != 0){
      cout << "Argument error" << endl;
    }
    else{
      tmp = "";
      pathname = argv[2];
      for (unsigned int z = 0; z < pathname.length(); ++z){
	if(pathname.compare(z,1,eod) == 0){
	  tmp.append(eod);
	  mkdir(tmp.c_str(), 0766);	  
	}
	else {
	  tmp += pathname[z];
	}
      }//for
      mkdir(tmp.c_str(), 0766);
    }//else
  }//else-if argc ==3
  //checks to see if the option supplied was "-m" and if an octal notation was given
  else if (argc == 4) {
    path = argv[3];
    oct_mode = argv[2];
    if(strcmp(m_opt, argv[1]) != 0){
      cout << "Argument error" << endl;
    }
    else {
      char * end;
      long int oct = strtol (oct_mode, &end, 8);
      mkdir(path, oct);
    }//else
  }
  /*
   *For when the user enters $ ./mkdir -p -m [octal] [dir_name]
   */
  else if (argc == 5) {
    if((strcmp(m_opt,argv[2]) != 0) || (strcmp(p_opt, argv[1]) != 0)){
      cout << "Argument order error" << endl;
    }//if
    else {
      //creates the strings to hold the pathname
      tmp = "";
      pathname = argv[4];
      oct_mode = argv[3];
      char * mode;
      long int octal = strtol(oct_mode, &mode, 8); //gets the octal representation
      //transfers the subdirectories to the pathname
      for (unsigned int z = 0; z < pathname.length(); ++z){
	if(pathname.compare(z,1,eod) == 0){
	  tmp.append(eod);
	  mkdir(tmp.c_str(), octal);	  
	}//if
	else {
	  tmp += pathname[z];
	}//else
      }//for
      mkdir(tmp.c_str(), octal);
    }//else
  }//else if argc == 5

} // main
