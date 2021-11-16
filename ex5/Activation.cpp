#include "Activation.h"
Activation::Activation (ActivationType act_type) : type (act_type)
{}
ActivationType Activation::get_activation_type () const
{
  return this->type;
}
/**
 * Applies the relu function on the given mat
 * @param mat Given mat
 * @return Ref to the given mat
 */
Matrix &relu (Matrix &mat)
{
  for (int i = 0; i < mat.get_rows () * mat.get_cols (); ++i)
    {
      if (mat[i] < 0)
        {
          mat[i] = 0;
        }
    }
  return mat;
}
/**
 * Applies the softmax function on the given mat
 * @param mat Given mat
 * @return Ref to the given mat
 */
Matrix &softmax (Matrix &mat)
{
  float sum = 0;
  int mat_size = mat.get_rows () * mat.get_cols ();
  for (int i = 0; i < mat_size; ++i)
    {
      sum += (std::exp (mat[i]));
    }
  sum = 1 / sum;
  for (int i = 0; i < mat_size; ++i)
    {
      mat[i] = sum * std::exp (mat[i]);
    }
  return mat;
}
/**
   * Applies activation function on the given matrix
   * @param mat Given matrix
   * @return A *new* matrix that the activation function is applied on
   */
Matrix Activation::operator() (Matrix const &mat) const
{
  Matrix result = Matrix (mat);
  if (this->type == RELU)
    {
      relu (result);
    }
  else if (this->type == SOFTMAX)
    {
      softmax (result);
    }
  else
    {
      std::cout << "Error: Invalid type" << std::endl;
      exit(EXIT_FAILURE);
    }
  return result;
}
