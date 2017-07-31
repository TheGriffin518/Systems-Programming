#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

using std::cout;
using std::endl;
using std::cin;

int remove_directory(const char *);

int main (const int argc, const char * argv [] ) {

  
  char  opt [] = "-r";
  char cmd;

  if (argc == 2)
    {
      cout << "Wish to remove: " 
	   << argv[1]
	   << "? y n ";
      cin >> cmd;
      if(cmd == 'y'){
	if (unlink(argv[1]) != 0){
	  perror(argv[0]);
	}
      }
    } //if argc == 2

  else if (argc == 3) {
    if(strcmp(argv[1], opt) != 0){
      cout << argv[0] 
	   << ": Option not recognized" 
	   << endl;
    }
    else{
      remove_directory(argv[2]);
    }

  } //else if argc == 3
  else {
    cout << argv[0] << ": Not enought arguments" << endl;
  } // else

  return EXIT_SUCCESS;
} //main


int remove_directory(const char * path) {

  DIR *dirp = opendir(path);
  int path_len = strlen(path);
  int r = -1;
  char cmd;

  if(dirp){
    struct dirent *stdirp;
    if((stdirp = readdir(dirp))==NULL) cout << "stdirp is null\n";
    r = 0;

    while(!r && (stdirp = readdir(dirp))) {
	int r2 = -1;
	char *buf;
	size_t len;
	
	if(!strcmp(stdirp->d_name,".") || !strcmp(stdirp->d_name,".."))
	  {
	    continue;
	  }//if
	
	len = path_len + strlen(stdirp->d_name) + 2;
	buf = (char*)malloc(len);
	
	if (buf){
	  struct stat statbuf;
	  
	  snprintf(buf,len,"%s/%s", path, stdirp->d_name);

	  if(!stat(buf, &statbuf)){
	    if (S_IFDIR & (statbuf.st_mode))
	      {
		cout << "Wish to descend into '" 
		     << stdirp->d_name
		     << "'? y n  " << endl;
		cin >> cmd;
		if(cmd == 'y'){
		  r2 = remove_directory(buf);//removes directory
		}
	      }
	    else //is file
	      {
		cout << "Wish to remove '" 
		     << stdirp->d_name
		     << "'? y n  ";
		cin >> cmd;
		if(cmd == 'y'){
		  r2 = unlink(buf);//removes directory
		}
	      }//else is file
	  }//if !stat

	  free(buf);
	}//if (buf)

	r = r2;
    }//while	
      
    closedir(dirp);
  }


  if(!r){
    r = rmdir(path);
  }
  return r;
  
}//remove_directory
