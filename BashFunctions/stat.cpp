#include <iostream>
#include <cstdlib>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>


using std::cout;
using std::endl;


void usage(struct stat &, const char *);

int main (const int argc, const char * argv[]){

  struct stat buf;
  if (argc != 2){
    cout << "Must include file" << endl;
    exit(0);
  }
  
  if(lstat(argv[1], &buf) != 0)
    perror(argv[0]);

  usage(buf, argv[1]);

  return EXIT_SUCCESS;

} //main

/*
 *Prints out information about the file passed to it in an easy to read manner. Supplies more information than 
 *ls -l does. 
 *@param struct stat &buf, the address of the struct stat that was created
 *@param const char * file, the name of the file to be stat() with
 *@return void, displays the formatted information to standard output
 */
void usage(struct stat &buf, const char * file){

  struct passwd *pw; 
  struct group *gr;
  pw = getpwuid(buf.st_uid); //gets the username from user ID 
  gr = getgrgid(buf.st_gid); //gets the group name from group ID

  cout << "File: '" << file << "'" << endl; //prints out the file
  cout << "Size: " << buf.st_size  //prints out the size in MB
       << "        Blocks: " << buf.st_blocks //number of Blocks
       << "        IO Blocks: " << buf.st_blksize //IO Blocksize
       << endl;
  cout << "Device: " << buf.st_dev  //Device ID
       << "         Links: " << buf.st_nlink //number of Links
       << endl;
  cout << "Access: "
       << (((buf.st_mode & S_IRUSR) != 0) ? "r" : "-")//displays the mode of file
       << (((buf.st_mode & S_IWUSR) != 0) ? "w" : "-")
       << (((buf.st_mode & S_IXUSR) != 0) ? "x" : "-")
       << (((buf.st_mode & S_IRGRP) != 0) ? "r" : "-")
       << (((buf.st_mode & S_IWGRP) != 0) ? "w" : "-")
       << (((buf.st_mode & S_IXGRP) != 0) ? "x" : "-")
       << (((buf.st_mode & S_IROTH) != 0) ? "r" : "-")
       << (((buf.st_mode & S_IWOTH) != 0) ? "w" : "-")
       << (((buf.st_mode & S_IXOTH) != 0) ? "x" : "-")
       << "      Uid: " << pw->pw_name //username
       << "      Gid: " << gr->gr_name //group name
       << endl;
  cout << "Access: " << ctime(&buf.st_atime); //access time
  cout << "Modify: " << ctime(&buf.st_mtime); //modify time
  cout << "Change: " << ctime(&buf.st_ctime)  //change time
       << endl;


} //usage

