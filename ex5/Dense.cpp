//
// Created by Itamar on 04/06/2021.
//

#include "Dense.h"

Dense::Dense (const Matrix &w, const Matrix &bias, ActivationType type)
    : _w (w), _bias (bias), _act (Activation (type))
{}
const Matrix &Dense::get_weights () const
{
  return _w;
}
const Matrix &Dense::get_bias () const
{
  return _bias;
}
Activation Dense::get_activation () const
{
  return _act;
}
/**
 * Applies the layer of input and returns output matrix
 * @param input Given matrix to apply the layer on
 * @return Output matrix (*new*)
 */
Matrix Dense::operator() (Matrix const &input) const
{
  Matrix output = input;
  output = (_w * output);
  output = output + _bias;
  output = _act (output);
  return output;
};