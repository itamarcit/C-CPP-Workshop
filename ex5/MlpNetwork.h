//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Matrix.h"
#include "Digit.h"
#include "Dense.h"

#define MLP_SIZE 4

//
const matrix_dims img_dims = {28, 28};
const matrix_dims weights_dims[] = {{128, 784},
                                    {64,  128},
                                    {20,  64},
                                    {10,  20}};
const matrix_dims bias_dims[] = {{128, 1},
                                 {64,  1},
                                 {20,  1},
                                 {10,  1}};

class MlpNetwork {
 private:
  Matrix _weights[MLP_SIZE];
  Matrix _biases[MLP_SIZE];
  /**
   * Denses the given input
   * @param mat given matrix to dense
   * @return matrix after it's densed
   */
  void dense_mat (Matrix &mat) const;
 public:
  MlpNetwork (Matrix weights[], Matrix biases[]); // Constructor for MlpNetwork
  /**
   * Applies the entire network on input
   * @param mat given matrix to apply on
   * @return digit struct
   */
  digit operator() (Matrix const &mat) const;

};
#endif // MLPNETWORK_H
