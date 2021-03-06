#include "Matrix.h"
#include <iostream>
#include <cstdlib>

using namespace std;

typedef unsigned int uint;
typedef initializer_list<initializer_list<double>> i_list;

/*
 *Constructor
 *takes in two uints to determine size of matrix, initializes all values to 0
 */
Matrix::Matrix (uint rows, uint cols) {
  matrix = NULL;
  this->rows = rows;
  this-> cols = cols;
  matrix = new double*[rows];
  for(uint i = 0; i < rows; ++i){
    matrix[i] = new double[cols];
    for(uint c = 0; c < cols; ++c){
      matrix[i][c] = 0;
    }
  }
}

/*
 *Constructor using a nested initializer_list
 */
Matrix::Matrix(const i_list & list){
  
  this->rows = list.size();
  this->cols = list.begin()->size();
  matrix = new double*[rows];
  for(uint i = 0; i < rows; ++i){
    matrix[i] = new double[cols];
    for(uint c = 0; c < cols; ++c){
      matrix[i][c] = 0;
    }
  }
  /*
  for(uint r = 0; r < rows; ++r){
    for (uint c = 0; c < cols; ++c){
      matrix[r][c] = *((list.begin()->begin())+(i*cols+j));
    }
  }
  */
  uint r = 0;
  for (auto & l : list){
      uint c = 0;
      for(auto & d : l) {
	matrix[r][c++] = d;
      }
      ++r;
  }
}

/*
 *Copy constructor. Constructs new matrix based on the one passed to it
 */
Matrix::Matrix(const Matrix & m) {
  this ->rows = m.rows;
  this ->cols = m.cols;
  matrix = new double*[m.rows];
  for(uint r = 0; r < m.rows; ++r){
    matrix[r] = new double[m.cols];
    for(uint c = 0; c < m.cols; ++c){
      matrix[r][c] = m.matrix[r][c];
    }
  }
}

/*
 *The deconstructor. Deletes memory of the objects after they have been used
 */
Matrix::~Matrix (){
  for (uint r = 0; r < rows; ++r){
    delete matrix[r];
  }
  delete matrix;
  matrix = NULL;
}

/*
 *Adds a scalar value to the matrix that calls it
 */
Matrix Matrix::add(double s) const {
  for (uint r = 0; r < rows; ++r){
    for(uint c = 0; c < cols; ++c){
      matrix[r][c] += s;
    }
  }
  return *this;
}

 /*
 *Adds a matrix to the matrix that calls it. Checks to see if 
 *addition is possible on the Matrix. 
 */
Matrix Matrix::add(const Matrix & m) const {
  if((rows == m.rows) && (cols == m.cols)){
    for (uint r = 0; r < rows; ++r){
      for (uint c = 0; c < cols; ++c){
	matrix[r][c] = matrix[r][c] + m.matrix[r][c];
      }
    }
    return *this;
  }
  else{
    cout << "The matrices do not have the same dimensions"
	 << " and cannot be added together" << endl;
  }
  return *this;
}

/*
 *Substracts a scalar value from the matrix
 */
Matrix Matrix::subtract(double s) const {
  return add(-s);
}

/*
 *Substracts a matrix from the matrix that called this function. Checks
 *to see if subtraction is possible. 
 */
Matrix Matrix::subtract(const Matrix & m) const {
  if((rows == m.rows) && (cols == m.cols)){
    for (uint r = 0; r < rows; ++r){
      for (uint c = 0; c < cols; ++c){
	matrix[r][c] = matrix[r][c] - m.matrix[r][c];
      }
    }
    return *this;
  }
  else{
    cout << "The matrices do not have the same dimensions"
	 << " and cannot be subtracted" << endl;
  }
  return *this;
}

/*
 *Multiplies the matrix by a scalar value.
 */
Matrix Matrix::multiply(double s) const {
  for (uint r = 0; r < rows; ++r){
    for(uint c = 0; c < cols; ++c){
      matrix[r][c] *= s;
    }
  }
  return *this;
}

/*
 *Multiplies a matrix by the matrix that called this function. Checks
 *to see if multiplication is allowed. 
 */
Matrix Matrix::multiply(const Matrix & m) const {
  if(cols == m.rows) {
    for(uint r = 0; r < rows; ++r){
      for(uint i = 0; i < m.cols; ++i){
	for(uint c = 0; c < cols; ++c){
	  matrix[r][i] += matrix[r][c] * m.matrix[c][i];
	}
      }
    }
  }
  else {
    cout << "Unable to multiply these matrices." << endl;
  }
  return *this;
}

/*
 *Divides the matrix by a scalar value by calling the multiply 
 *function and passing the reciporcal of the scalar value to it
 */
Matrix Matrix::divide(double s) const {
  return multiply(1/s);
}

/*
 *Transposes the matrix that is passed to it and returns the result. 
 */
Matrix Matrix::t() const {
  Matrix tran(cols, rows);
  for(uint r = 0; r < tran.rows; ++r){
    for(uint c = 0; c < tran.cols; ++c){
      tran.matrix[r][c] = matrix[c][r];
    }
  }
  return tran;
}

/*
 *Returns the number of rows for that matrix. 
 */
const uint Matrix::numRows() const {
  return rows;
}

/*
 *Returns the number of columns for that matrix. 
 */
const uint Matrix::numCols() const {
  return cols;
}

/*
 *Gets and sets the value for an element at a particular index.
 */
double & Matrix::at(uint row, uint col){
  return matrix[row][col];
}

/*
 *Gets the value for an element at a particular index. 
 */
const double & Matrix::at (uint row, uint col) const {
  return matrix[row][col];
}

/*
 *Overloads the + operator to allow two matrices to be added together.
 */
Matrix operator+ (const Matrix & a, const Matrix & b) {
  if(a.rows == b.rows && a.cols == b.cols){
    Matrix sum(a.rows, a.cols);
    for(uint r = 0; r < a.rows; ++r){
      for(uint c = 0; c < a.cols; ++c){
	sum.matrix[r][c] = a.matrix[r][c] + b.matrix[r][c];
      }
    }
    return sum;
     }
  else {
    cout << "Unable to add theses matrices." << endl;
    return a;
  }
}

/*
 *Overloads the + operator to allow a scalar to be added to the matrix.
 */
Matrix operator+ (double b, const Matrix & m){
  Matrix sum = m;
  sum.add(b);
  return sum;
}

/*
 *Overloads the + operator to allow a scalar to be on the right side. 
 */
Matrix operator+(const Matrix & m, double b) {
  Matrix sum = m;
  sum.add(b);
  return sum;
}

/*
 *Overloads the - operator to negate the matrix
 */
Matrix operator- (const Matrix & m) {
  Matrix neg(m.rows, m.cols);
  for (uint r = 0; r < m.rows; ++r){
    for(uint c = 0; c < m.cols; ++c){
      neg.matrix[r][c] = -m.matrix[r][c];
    }
  }
  return neg;
}

/*
 *Overloads the - operator to allow the scalar to be on the right side. 
 */
Matrix operator- (const Matrix & m, double b) {
  Matrix diff = m;
  diff.subtract(b);
  return diff;
}

/*
 *Overloads the - operator to allow the scalar to be on the left side. 
 */
Matrix operator- (double b, const Matrix & m) {
  Matrix diff = m;
  diff.subtract(b);
  return diff;
}

/*
 *Overloads the - operator to allow a matrix to be subtracted from 
 *another matrix.
 */
Matrix operator- (const Matrix & a, const Matrix & b){
  if(a.rows == b.rows && a.cols == b.cols){
    Matrix diff(a.rows, a.cols);
    for(uint r = 0; r < a.rows; ++r){
      for(uint c = 0; c < a.cols; ++c){
	diff.matrix[r][c] = a.matrix[r][c] + b.matrix[r][c];
      }
    }
    return diff;
  }
  else {
    cout << "Unable to subtract theses matrices." << endl;
    return a;
  }
}

/*
 *Overloads the * operator to allow the scalar to be on the right side.
 */
Matrix operator* (const Matrix & m, double b) {
  Matrix prod = m;
  prod.multiply(b);
  return prod;
}

/*
 *Overloads the * operator to allow the scalar to be on the left side. 
 */
Matrix operator* (double b, const Matrix & m) {
  Matrix prod = m;
  prod.multiply(b);
  return prod;
}

/*
 *Overloads the * operator to allow matrix multiplication. 
 */
Matrix operator* (const Matrix & a, const Matrix & b) {
  if (a.cols == b.rows) {
    Matrix prod(a.rows, b.cols);
    for(uint r = 0; r < a.rows; ++r){
      for(uint i = 0; i < b.cols; ++i){
	for(uint c = 0; c < a.cols; ++c){
	  prod.matrix[r][i] += a.matrix[r][c] * b.matrix[c][i];
	}
      }
    }
    return prod;
  }
  else {
    cout << "Unable to multiply these matrices." << endl;
    return a;
  }
}

/*
 *Overloads the / operator to allow the scalar to be on the right side. 
 */
Matrix operator/ (const Matrix & m, double b) {
  Matrix div = m;
  div.divide(b);
  return div;
}

/*
 *Overloads the / operator to allow the scalar to be on the left side.
 */
Matrix operator/ (double b, const Matrix & m) {
  Matrix div = m; 
  div.divide(b);
  return div;
}

/*
 *Overloads the = operator to copy the matrix. 
 */
Matrix & Matrix::operator= (const Matrix & m) {
  this->rows = m.rows;
  this->cols = m.cols;
  matrix = new double*[m.rows];
  for(uint r = 0; r < m.rows; ++r){
    matrix[r] = new double[m.cols];
    for(uint c = 0; c < m.cols; ++c){
      matrix[r][c] = m.matrix[r][c];
    }
  }
  return *this;
}

/*
 *Overloads the () operator to access an element at a particular index. 
 */
double & Matrix::operator() (uint r, uint c) {
    return this-> matrix[r][c];
}

/*
 *Overloads the << operator to allow the output of a Matrix object
 */
ostream & operator<< (ostream & os, const Matrix & m) {
 
  for(uint r = 0; r < m.rows; ++r){
    os << "[ ";
    for(uint c = 0; c < m.cols; ++c){
      os << m.matrix[r][c] << ", ";
     }
    os << "]" << "\n";
  }
  return os;
}

