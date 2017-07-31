#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


using std::cout;
using std::endl;
using std::strcmp;

/*
 *Used to get the octal mode from the user
 */
int oct_mode(std::string&, int);
/*
 *Used to get the symbolic mode from the user. 
 */
int mode(std::string&, int);

  bool plus = false;
  bool minus = false;
  bool equals = false;
 
  bool u = false, g = false, o = false, a = false;
  bool r = false, w = false, x = false, s = false, t = false, X = false;

int main(int argc, char *argv[]){

  char * filename;
  int permission;
  int new_mode;


  if (argc == 4){//used to see if user put in a total of 3 args

    std::string perms = argv[2];//used to get permissions in octal or symbol
    filename = argv[3];//the filename the user passes through
    struct stat buf;

    if(stat (filename, &buf) != 0){
      perror ("No such file");
    }
    else 
      permission = buf.st_mode;


    if (strcmp (argv[1], "-sym") == 0){
      new_mode = mode(perms, permission);
    }
  
    else  if (strcmp (argv[1], "-oct") == 0){
      new_mode = oct_mode(perms, permission);
    }    
  }//if
  else 
    {
      cout << argv[0] << ": Arguments not supplied" << endl;
    } //else 

  if( chmod(filename, new_mode) != 0){
    perror ("ERROR");
  }
  
  return EXIT_SUCCESS;

} //main

int mode(std::string & perms, int perm) {
  int i;
  int t_perm = perm;

  int r_val = 4;
  int w_val = 2;
  int x_val = 1;

  int mask = 0;
  int mod = 0;
  /*
   *Uses a mask to store the value of what the user wants to change the 
   *the permissions and track them. 
   */
  for(unsigned int z = 0; z < perms.length(); ++z){//iterates through the 
    i = perms[z];             //the symbols and determines what the user
    switch (i)                //wants to change the permissions to
      {
      case 'u':
	u = true;
	break;
      case 'g':
	g = true;
	break;
      case 'o':
	o = true;
	break;
      case 'a':
	a = true;
	break;
      case 'r':
	mask = mask | r_val;
	break;
      case 'w':
	mask = mask | w_val;
	break;
      case 'x':
	mask = mask | x_val;
	break;
      case 's':
	s = true;
	break;
      case 't':
	t = true;
	break;
      case 'X':
	X = true;
	break;
      case '=':
	equals = true;
	break;
      case '+':
	plus = true;
	break;
      case '-':
	minus = true;
	break;
      case ',':
	break;
      
      }//switch


  }//while
  /*
   *Insures that the mask appoints the correct value to correct index by
   *left shifting the mask however many positions it needs to be shifted.
   */
  if(plus){
    if(u){
      mod = (mask << 6) | t_perm;
    }
    if (g) {
      mod = ((mask << 3) | t_perm) | mod;
    }
    if (o) {
      mod = (mask | t_perm) | mod;
    }
    if (a) {
      mod = ((mask << 6) | t_perm) | mod;
      mod = ((mask << 3) | t_perm) | mod;      
      mod = (mask | t_perm) | mod;
    }
  }

  else if (minus) {
    if(u){
      mod = ~(mask << 6) & t_perm;
    }
    if (g) {
      mod = (~(mask << 3) & t_perm) | mod;
    }
    if (o) {
      mod = (~mask & t_perm) | mod;
    }
    if (a) {
      mod = (~(mask << 6) & t_perm) | mod;
      mod = (~(mask << 3) & t_perm) | mod;      
      mod = (~mask & t_perm) | mod;
    }
  }
  
  else if (equals) {
    if (u){
      mod = ~(448) & t_perm;
      mod = (mask << 6) | mod;
    }
    if (g){
      mod = ~(56) & t_perm;
      mod = (mask << 3) | mod;
    }
    if (o){
      mod = ~(7) & t_perm;
      mod = mask | mod;
    }
    if (a){
      mod = (mask << 6);
      mod = mod | (mask << 3);
      mod = mod | mask;
    }
  }

  return mod;
} //mode


int oct_mode(std::string& perms, int perm) {
  int index;
  int mask;
  int u_mask = 0;
  int g_mask = 0;
  int o_mask = 0;

  if (perms.length() > 4){
    cout << "Argument error" << endl;
  }
  
  if (perms.length() == 3) 
    index = 1;
  else if (perms.length() == 4)
    index = 0;
  for(unsigned int z = 0; z < perms.length(); ++z){
     mask = 0;
    int c = perms[z];
    switch(c){
      
    case '0':
      break;
    case '1':
      mask = 1;
      break;
    case '2':
      mask = 2;
      break;
    case '3':
      mask = 3;
      break;
    case '4':
      mask = 4;
      break;
    case '5':
      mask = 5;
      break;
    case '6':
      mask = 6;
      break;
    case '7':
      mask = 7;
      break;
    default:
      cout << "Invalid octal" << endl;
      break;
    }//switch

    if (index == 1){
      u_mask = (mask << 6);
    }
    else if(index == 2){
      g_mask = (mask << 3);
    }
    else if (index == 3){
      o_mask = mask;
      mask = (u_mask | g_mask | o_mask);
    }
    index++;
  }//for
  return mask;

} // oct_mode
