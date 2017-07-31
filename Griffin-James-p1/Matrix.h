#ifndef MATRIX_H
#define MATRIX_H

#include <initializer_list>
#include <iostream>

using std::cout;
using std::endl;
using std::initializer_list;
using std::ostream;

typedef unsigned int uint;
typedef initializer_list<initializer_list<double>> i_list;

class Matrix {

 public:
  uint rows, cols;
  double ** matrix;
  
  Matrix(uint rows, uint cols);                 // constructor (all elements initialized to 0)
  Matrix(const i_list & list);                  // constructor (using initializer list)
  Matrix(const Matrix & m);                     // copy constructor
  ~Matrix();                                    // destructor

  Matrix add(double s) const;                   // add scalar to this matrix
  Matrix add(const Matrix & m) const;           // add this matrix and another matrix

  Matrix subtract(double s) const;              // subtract scalar from this matrix
  Matrix subtract(const Matrix & m) const;      // subtract another matrix from this matrix
  
  Matrix multiply(double s) const;              // multiply this matrix by a scalar
  Matrix multiply(const Matrix & m) const;      // multiply this matrix by another matrix
  
  Matrix divide(double s) const;                // divide this matrix by a scalar
  Matrix t() const;                             // transpose of this matrix

  const uint numRows() const;                   // returns the number of rows
  const uint numCols() const;                   // returns the number of cols

  double & at(uint row, uint col);              // get/set element at row,col
  const double & at(uint row, uint col) const;  // get element at row,col (when using a const object)

  Matrix & operator= (const Matrix & m); 
  double & operator() (uint r, uint c);

}; //Matrix

/*
 *Operator overloads to support arithmetic operations with scalars
 */
  Matrix operator- (const Matrix & m);
  Matrix operator- (const Matrix & m, double b);
  Matrix operator- (const Matrix & a, const Matrix & b);
  Matrix operator- (double b, const Matrix & m);
  Matrix operator+ (const Matrix & a, const Matrix & b);
  Matrix operator+ (const Matrix & a, double b);
  Matrix operator+ (double b, const Matrix & m);
  Matrix operator* (const Matrix & m, double b);
  Matrix operator* (const Matrix & a, const Matrix & b);
  Matrix operator* (double b, const Matrix & m);
  Matrix operator/ (const Matrix & m, double b);
  Matrix operator/ (double b, const Matrix & m);
  ostream & operator<< (ostream & os, const Matrix & m);

#endif
