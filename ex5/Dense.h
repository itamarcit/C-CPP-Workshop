#ifndef C___PROJECT_DENSE_H
#define C___PROJECT_DENSE_H

#include "Activation.h"

class Dense {
 private:
  Matrix _w;
  Matrix _bias;
  Activation _act;
 public:
  Dense (Matrix const &w, Matrix const &bias, ActivationType type);
  const Matrix &get_weights () const;
  const Matrix &get_bias () const;
  Activation get_activation () const;
  /**
 * Applies the layer of input and returns output matrix
 * @param input Given matrix to apply the layer on
 * @return Output matrix (*new*)
 */
  Matrix operator() (Matrix const &input) const;
};

#endif //C___PROJECT_DENSE_H
