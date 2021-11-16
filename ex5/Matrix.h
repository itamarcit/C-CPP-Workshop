// Matrix.h

#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims {
    int rows, cols;
} matrix_dims;

class Matrix {
 private:
  matrix_dims mat_dims;
  float *mat_ptr;
  int get_size () const; // gets rows * cols of the matrix
 public:
  Matrix (); // Default constructor - sets to (1,1) dims
  Matrix (int rows, int cols); // Constructor to (rows, cols)
  Matrix (Matrix const &other); // Copy constructor
  ~Matrix (); // Destructor
  int get_rows () const; // Getter for rows
  int get_cols () const; // Getter for cols
  /**
   * Transposes the given matrix, making it's cols the rows and vice versa.
   * The new dims will be the original (cols, rows)
   * @return Reference to the given matrix after it's transposed
   */
  Matrix &transpose ();
  /**
   * Changes the given matrix to be with 1 column only, making each float
   * a row.
   * @return Reference to the given matrix after it's changed
   */
  Matrix &vectorize ();
  /**
   * Prints the given matrix with spaces between each column, and a space
   * between each row
   */
  void plain_print () const;
  /**
   * Multiplies two matrices each (row, col) by it's corresponding (row, col)
   * in the other matrix. If the two matrices don't have the same rows, cols
   * then it exits the program
   * @param other Another matrix to multiply with
   * @return A *new* matrix which consists of the multiplication of the two
   * given matrices
   */
  Matrix dot (Matrix const &other) const;
  /**
   * A function that returns the Frobenius norm of a given matrix.
   * @return The norm of the matrix
   */
  float norm () const;

  /**  MATRIX OPERATORS  **/

  /**
   * Addition operator, adds two matrices and returns a *new* one. Exits the
   * program if the matrix dimensions aren't equal.
   * @param other Matrix to add to this matrix
   * @return A *new* matrix
   */
  Matrix operator+ (const Matrix &other) const;
  /**
   * Equal operator. Makes a new matrix into this and returns it.
   * @param other Matrix to be copied
   * @return Reference to the matrix that's being put into
   */
  Matrix &operator= (const Matrix &other);
  /**
   * Multiplies two matrices and returns a *new* matrix. Multiplies as taught
   * in Linear Algebra 1 course, so if this's cols aren't equal to other's rows
   * the program will exit.
   * @param other Matrix to be multiplied with
   * @return A *new* matrix
   */
  Matrix operator* (Matrix const &other) const;
  /**
   * Matrix and scalar (float) multiplication) as taught in Linear Algebra 1.
   * @param mat Matrix to be multiplied
   * @param other Float to be multiplied by
   * @return A *new* matrix
   */
  Matrix operator* (const float &other) const;
  /**
   * Matrix and scalar (float) multiplication) as taught in Linear Algebra 1.
   * Same as last function, but the scalar is multiplied from the left side.
   * @param mat Matrix to be multiplied
   * @param other Float to be multiplied by
   * @return A *new* matrix
   */
  friend Matrix operator* (const float &other, const Matrix &mat);
  /**
   * Output stream operator. Will print the matrix as the digit it's supposed
   * to represent. As per section 2.4.
   * @param os - Stream to write into
   * @param mat - Given matrix to print
   * @return - Returns os stream ref
   */
  friend std::ostream &operator<< (std::ostream &os, Matrix const &mat);
  /**
   * Operator +=, adds a matrix to the given matrix and returns a ref.
   * @param other Matrix to be added to this matrix
   * @return Reference to this matrix
   */
  Matrix &operator+= (const Matrix &other);
  /**
   * Returns the (row, col) element of the matrix. If out of bounds, exits the
   * program.
   * @param row - Row requested in this matrix
   * @param col - Col requested in this matrix
   * @return - Float ref to the element requested
   */
  float &operator() (int row, int col);
  /**
   * Returns the value of (row, col) element in the matrix. If out of bounds,
   * exits the program.
   * @param row Row requested in the matrix
   * @param col Col requested in the matrix
   * @return Float by value to the element requested
   */
  const float &operator() (int row, int col) const;
  /**
   * Returns the num's element of the matrix. If out of bounds, then exits the
   * program. (as per section 3.2)
   * @param num - Requested index of the matrix
   * @return - Float ref to the element requested
   */
  float &operator[] (int num);
  /**
   * Const [] operator to access the num index of the matrix, by value
   * @param num
   * @return given mat index at num
   */
  const float &operator[] (int num) const;
  /**
   * Fills the matrix elements from a stream of floats which are each the size
   * of 4. Must fill the entire vector or the function will exit the program.
   * @param is - stream to read from
   * @param mat - matrix to fill
   * @return - returns a reference to the given stream
   */
  friend std::istream &read_binary_file (std::istream &is, Matrix const &mat);
};

#endif //MATRIX_H
