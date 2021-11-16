#include "Matrix.h"
#include <cmath>

#define INDEX_CONSTANT 0.1
#define DEFAULT_MATRIX_SIZE 1
#define FLOAT_SIZE 4
#define OUT_OF_BOUNDS_ERR "Error: Out of bounds"

/**
 * Default constructor
 */
Matrix::Matrix () : mat_dims ({DEFAULT_MATRIX_SIZE, DEFAULT_MATRIX_SIZE})
{
  mat_ptr = new (std::nothrow) float[DEFAULT_MATRIX_SIZE];
  if (!mat_ptr)
    {
      std::cerr << "Error: Matrix allocation failed. Quitting." << std::endl;
      exit (EXIT_FAILURE);
    }
  mat_ptr[0] = 0;
}

/**
 * Constructor, makes a matrix with (rows, cols) dims
 * @param rows given rows (Natural num > 0)
 * @param cols given cols (Natural num > 0)
 */
Matrix::Matrix (int rows, int cols) : mat_dims ({rows, cols})
{
  if (rows <= 0 || cols <= 0)
    {
      std::cerr << "Error: Invalid matrix rows/cols in matrix constructor"
                << std::endl;
      exit (EXIT_FAILURE);
    }
  mat_ptr = new (std::nothrow) float[rows * cols];
  if (!mat_ptr)
    {
      std::cerr << "Error: Matrix allocation failed. Quitting." << std::endl;
      exit (EXIT_FAILURE);
    }
  for (int i = 0; i < rows * cols; ++i)
    {
      mat_ptr[i] = 0;
    }
}
/**
 * Copy constructor
 * @param other Matrix to copy
 */
Matrix::Matrix (const Matrix &other) : mat_dims{other.get_rows (),
                                                other.get_cols ()}
{
  mat_ptr = new (std::nothrow) float[other.get_size ()];
  if (!mat_ptr)
    {
      std::cerr << "Error: Matrix allocation failed. Quitting." << std::endl;
      exit (EXIT_FAILURE);
    }
  for (int i = 0; i < other.get_size (); ++i)
    {
      this->mat_ptr[i] = other.mat_ptr[i];
    }
}
/**
 * Destructor
 */
Matrix::~Matrix ()
{
  delete[] mat_ptr;
  mat_ptr = nullptr;
}
/**
 * Row getter
 * @return Matrix's rows
 */
int Matrix::get_rows () const
{
  return this->mat_dims.rows;
}
/**
 * Col getter
 * @return Matrix's cols
 */
int Matrix::get_cols () const
{
  return this->mat_dims.cols;
}
/**
 * Gets the size of the matrix
 * @return rows * cols
 */
int Matrix::get_size () const
{
  return this->mat_dims.rows * this->mat_dims.cols;
}
/**
   * Transposes the given matrix, making it's cols the rows and vice versa.
   * The new dims will be the original (cols, rows)
   * @return Reference to the given matrix after it's transposed
   */
Matrix &Matrix::transpose ()
{
  Matrix cpy = Matrix (*this);
  int index = 0;
  for (int i = 0; i < this->mat_dims.cols; ++i)
    {
      for (int j = 0; j < this->mat_dims.rows; ++j)
        {
          this->mat_ptr[index] = cpy.mat_ptr[i + j * cpy.mat_dims.cols];
          ++index;
        }
    }
  this->mat_dims.rows = this->mat_dims.cols;
  this->mat_dims.cols = cpy.mat_dims.rows;
  return *this;

}
/**
   * Changes the given matrix to be with 1 column only, making each float
   * a row.
   * @return Reference to the given matrix after it's changed
   */
Matrix &Matrix::vectorize ()
{
  this->mat_dims.rows = this->get_size ();
  this->mat_dims.cols = 1;
  return *this;
}
/**
   * Prints the given matrix with spaces between each column, and a space
   * between each row
   */
void Matrix::plain_print () const
{
  int index = 0;
  for (int i = 0; i < this->mat_dims.rows; ++i)
    {
      for (int j = 0; j < this->mat_dims.cols; ++j)
        {
          std::cout << this->mat_ptr[index] << " ";
          ++index;
        }
      std::cout << std::endl;
    }
}
/**
   * A function that returns the Frobenius norm of a given matrix.
   * @return The norm of the matrix
   */
float Matrix::norm () const
{
  float result, sum = 0;
  for (int i = 0; i < this->get_size (); ++i)
    {
      sum += std::pow (this->mat_ptr[i], 2);
    }
  result = std::sqrt (sum);
  return result;
}
/**
  * Multiplies two matrices each (row, col) by it's corresponding (row, col)
  * in the other matrix. If the two matrices don't have the same rows, cols
  * then it exits the program
  * @param other Another matrix to multiply with
  * @return A *new* matrix which consists of the multiplication of the two
  * given matrices
  */
Matrix Matrix::dot (const Matrix &other) const
{
  if (this->get_rows () != other.get_rows ()
      || this->get_cols () != other.get_cols ())
    {
      std::cerr << "Error: Invalid matrix dot operation, rows/cols don't match"
                << std::endl;
      exit (EXIT_FAILURE);
    }
  Matrix result = Matrix (other.mat_dims.rows, other.mat_dims.cols);
  int index;
  for (int i = 0; i < other.mat_dims.rows; ++i)
    {
      for (int j = 0; j < other.mat_dims.cols; ++j)
        {
          index = i * other.mat_dims.cols + j;
          result.mat_ptr[index] = other.mat_ptr[index] * this->mat_ptr[index];
        }
    }
  return result;
}
/**
   * Addition operator, adds two matrices and returns a *new* one. Exits the
   * program if the matrix dimensions aren't equal.
   * @param other Matrix to add to this matrix
   * @return A *new* matrix
   */
Matrix Matrix::operator+ (const Matrix &other) const
{
  if (other.mat_dims.rows != this->mat_dims.rows
      || other.mat_dims.cols != this->mat_dims.cols)
    {
      std::cerr << "Error:  Invalid matrix addition" << std::endl;
      exit (EXIT_FAILURE);
    }
  int index;
  Matrix result = Matrix (*this);
  for (int i = 0; i < other.mat_dims.rows; ++i)
    {
      for (int j = 0; j < other.mat_dims.cols; ++j)
        {
          index = i * other.mat_dims.cols + j;
          result.mat_ptr[index] = other.mat_ptr[index] + this->mat_ptr[index];
        }
    }
  return result;
}
/**
   * Fills the matrix elements from a stream of floats which are each the size
   * of 4. Must fill the entire vector or the function will exit the program.
   * @param is - stream to read from
   * @param mat - matrix to fill
   * @return - returns a reference to the given stream
   */
std::istream &read_binary_file (std::istream &is, Matrix const &mat)
{
  unsigned long size = mat.get_rows () * mat.get_cols ();
  is.read ((char *) mat.mat_ptr, size * FLOAT_SIZE);
  if (!is.good ())
    {
      std::cerr << "Error: reading file" << std::endl;
      exit (EXIT_FAILURE);
    }
  return is;
}
/**
   * Equal operator. Makes a new matrix into this and returns it.
   * @param other Matrix to be copied
   * @return Reference to the matrix that's being put into
   */
Matrix &Matrix::operator= (const Matrix &other)
{
  if (this == &other)
    {
      return *this;
    }
  delete[] this->mat_ptr;
  this->mat_dims = {other.get_rows (), other.get_cols ()};
  this->mat_ptr = new float[other.get_size ()];
  for (int i = 0; i < other.get_size (); ++i)
    {
      this->mat_ptr[i] = other.mat_ptr[i];
    }
  return *this;
}
/**
   * Operator +=, adds a matrix to the given matrix and returns a ref.
   * @param other Matrix to be added to this matrix
   * @return Reference to this matrix
   */
Matrix &Matrix::operator+= (const Matrix &other)
{
  *this = *this + other;
  return *this;
}
/**
   * Multiplies two matrices and returns a *new* matrix. Multiplies as taught
   * in Linear Algebra 1 course, so if this's cols aren't equal to other's rows
   * the program will exit.
   * @param other Matrix to be multiplied with
   * @return A *new* matrix
   */
Matrix Matrix::operator* (const Matrix &other) const
{
  if (this->mat_dims.cols != other.mat_dims.rows)
    {
      std::cerr << "Error: Matrix multiplication undefined" << std::endl;
      exit (EXIT_FAILURE);
    }
  Matrix result = Matrix (this->mat_dims.rows, other.mat_dims.cols);
  float sum = 0;
  for (int i = 0; i < this->mat_dims.rows; ++i)
    {
      for (int j = 0; j < other.mat_dims.cols; ++j)
        {
          for (int k = 0; k < other.get_rows (); ++k)
            {
              sum += this->mat_ptr[i * this->mat_dims.cols + k]
                     * other.mat_ptr[j + k * other.mat_dims.cols];
            }
          result[i * other.mat_dims.cols + j] = sum;
          sum = 0;
        }
    }
  return result;
}
/**
   * Returns the (row, col) element of the matrix. If out of bounds, exits the
   * program.
   * @param row - Row requested in this matrix
   * @param col - Col requested in this matrix
   * @return - Float ref to the element requested
   */
float &Matrix::operator() (int row, int col)
{
  if (row > this->mat_dims.rows || col > this->mat_dims.cols)
    {
      std::cerr << OUT_OF_BOUNDS_ERR << std::endl;
      exit (EXIT_FAILURE);
    }
  return this->mat_ptr[row * this->mat_dims.cols + col];
}
const float &Matrix::operator() (int row, int col) const
{
  if (row > this->mat_dims.rows || col > this->mat_dims.cols)
    {
      std::cerr << OUT_OF_BOUNDS_ERR << std::endl;
      exit (EXIT_FAILURE);
    }
  return this->mat_ptr[row * this->mat_dims.cols + col];
}
/**
   * Returns the num's element of the matrix. If out of bounds, then exits the
   * program. (as per section 3.2)
   * Note: this is a non-const func, so the value can be changed
   * @param num - Requested index of the matrix
   * @return - Float ref to the element requested
   */
float &Matrix::operator[] (int num)
{
  if (num > this->get_size ())
    {
      std::cerr << OUT_OF_BOUNDS_ERR << std::endl;
      exit (EXIT_FAILURE);
    }
  float &result = this->mat_ptr[num];
  return result;
}
/**
 * Const [] operator to access the num index of the matrix, by value
 * @param num
 * @return given mat index at num
 */
const float &Matrix::operator[] (int num) const
{
  if (num > this->get_size ())
    {
      std::cerr << OUT_OF_BOUNDS_ERR << std::endl;
      exit (EXIT_FAILURE);
    }
  return this->mat_ptr[num];
}
/**
   * Output stream operator. Will print the matrix as the digit it's supposed
   * to represent. As per section 2.4.
   * @param os - Stream to write into
   * @param mat - Given matrix to print
   * @return - Returns os stream ref
   */
std::ostream &operator<< (std::ostream &os, Matrix const &mat)
{
  for (int i = 0; i < mat.mat_dims.rows; ++i)
    {
      for (int j = 0; j < mat.mat_dims.cols; ++j)
        {
          if ((mat) (i, j) >= INDEX_CONSTANT)
            {
              os << "  ";
            }
          else
            {
              os << "**";
            }
        }
      os << std::endl;
    }
  return os;
}
/**
   * Matrix and scalar (float) multiplication) as taught in Linear Algebra 1.
   * @param mat Matrix to be multiplied
   * @param other Float to be multiplied by
   * @return A *new* matrix
   */
Matrix Matrix::operator* (const float &other) const
{
  Matrix result = Matrix (*this);
  for (int i = 0; i < result.get_size (); ++i)
    {
      result[i] = result.mat_ptr[i] * other;
    }
  return result;
}
/**
   * Matrix and scalar (float) multiplication) as taught in Linear Algebra 1.
   * Same as last function, but the scalar is multiplied from the left side.
   * @param mat Matrix to be multiplied
   * @param other Float to be multiplied by
   * @return A *new* matrix
   */
Matrix operator* (const float &other, const Matrix &mat)
{
  Matrix result = Matrix (mat);
  for (int i = 0; i < mat.get_size (); ++i)
    {
      result[i] = mat.mat_ptr[i] * other;
    }
  return result;
}

