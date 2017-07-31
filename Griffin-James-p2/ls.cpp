#include <dirent.h>
#include <iostream>
#include <string>
#include <errno.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

using std::cout;
using std::endl;


/*
 *Displays information about the file supplied in a formatted way for easy readability.
 *@param const char *, the name of the file to be stat
 *@return void, displays the formatted info to standard out
 */
void file_stat(const char *);

int main (const int argc, const char * argv []) {

  DIR *dirp;
  struct dirent *dp;

  /*
   *Displays the stat of the file specified or directory
   */
  if(argc == 2){
    if ((dirp = opendir(argv[1])) == NULL) {
      file_stat(argv[1]);
    }
  }
  /*
   *Displays the contents of the pwd and shows relevant information
   */
  else if (argc ==1){
    if ((dirp = opendir(".")) == NULL){
      perror("Couldn't open '.'");
      exit(0);
    }
    /*
     *Reads the contents of the directory and passes them off to file_stat to 
     *display the information in a correctly formatted way
     */
    do {
      if ((dp = readdir(dirp)) != NULL) {
	file_stat(dp->d_name);
      }
    } while (dp != NULL);
    closedir(dirp);
  }
  return EXIT_SUCCESS;

} //main 

void file_stat(const char * file) {
  
  struct stat buf; //the stat struct for the file
  struct passwd *pw; //used to obtain user name
  struct group *gr; //used to obtain the group name
  if(lstat(file, &buf) != 0)
    perror("./ls: ");
  
  pw = getpwuid(buf.st_uid); //transforms the user ID into the user name
  gr = getgrgid(buf.st_gid); //transforms the group ID into the group name
  char * time = ctime(&buf.st_atime); //displays the time in a readable fashion
  cout << (((buf.st_mode & S_IRUSR) != 0) ? "r" : "-")//checks the mode
       << (((buf.st_mode & S_IWUSR) != 0) ? "w" : "-")
       << (((buf.st_mode & S_IXUSR) != 0) ? "x" : "-")
       << (((buf.st_mode & S_IRGRP) != 0) ? "r" : "-")
       << (((buf.st_mode & S_IWGRP) != 0) ? "w" : "-")
       << (((buf.st_mode & S_IXGRP) != 0) ? "x" : "-")
       << (((buf.st_mode & S_IROTH) != 0) ? "r" : "-")
       << (((buf.st_mode & S_IWOTH) != 0) ? "w" : "-")
       << (((buf.st_mode & S_IXOTH) != 0) ? "x" : "-");

  cout << " " << buf.st_nlink; //number of hard links
  cout << " " << pw->pw_name;//the user name
  cout << " " << gr->gr_name;//the group name
  cout << " " << buf.st_size;//the size in megabytes
  cout << " " << time;//readable time
  cout << " " << file << endl; //the file name

}// file_stat


