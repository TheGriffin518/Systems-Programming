#include "Matrix.h"
#include <iostream>
#include <initializer_list>
#include <cstdlib>

using std::cout;
using std::endl;
using std::cin;
using std::initializer_list;
using std::ostream;

typedef initializer_list<initializer_list<double>> ilist;
typedef unsigned int uint;

int main() {


  //creates an array with 0's
  Matrix z(2,2);
  z.at(0,0) = 1;
  z.at(0,1) = 2;
  z.at(1,0) = 3;
  z.at(1,1) = 4;
  
  cout << "Matrix z: " << endl;
  cout << z << "\n\n";

  Matrix b(2,1);
  b.at(0,0) = 3;
  b.at(1,0) = 9;

  cout << "Matrix b: " << endl;
  cout << b << "\n\n";

  Matrix c = z.multiply(b);
  cout << "Matrix c: \n";
  cout << c << "\n\n";

  //construction with initializer_list
  Matrix d = {{1,2},
  	      {3,4}};
  cout << "Matrix d using and initializer list \n";
  cout << d << endl;

  //Scalar on the right side

  Matrix a = {{1,2},
	      {3,4}};

  cout << "Matrix a: \n" << a << "\n\n";
  Matrix c0 = a + 5.2;
  cout << "Matrix a + 5.2 \n" << c0 << endl;
  Matrix c1 = a + a;
  cout << "Matrix a + a \n" << c1 << endl;
  Matrix c2 = a - 3.5;
  cout << "Matrix a - 3.5 \n" << c2 << endl;
  Matrix c3 = a - d;
  cout << "Matrix a -d \n" << c3 << endl;
  Matrix c4 = a * b;
  cout << "Matrix a * b \n" << c4 << endl;
  Matrix c5 = a / 2.0;
  cout << "Matrix a / 2.0 \n" << c5 << endl;

  //Scalar on the left side
  Matrix a1 = 4.0 + a;
  cout << "4.0 + Matrix a \n" << a1 << endl;
  Matrix a2 = 4.0 - a;
  cout << "4.0 - Matrix a \n" << a2 << endl;
  Matrix a3 = 2.0 * a;
  cout << "2.0 * Matrix a \n" << a3 << endl;
  Matrix a4 = 12.0 / a;
  cout << "12.0 / Matrix a \n" << a4 << endl;

  cout << "The negative of Matrix a after being multiplied with b \n";
  cout << -a << endl;

  return EXIT_SUCCESS;

}
