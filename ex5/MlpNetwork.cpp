//
// Created by Itamar on 07/06/2021.
//

#include "MlpNetwork.h"

MlpNetwork::MlpNetwork (Matrix weights[4], Matrix biases[4])
{
  for (int i = 0; i < MLP_SIZE; ++i)
    {
      _weights[i] = weights[i];
      _biases[i] = biases[i];
    }
}
/**
 * Denses the given input (changes input)
 * @param mat given matrix to dense
 */
void MlpNetwork::dense_mat (Matrix &mat) const
{
  Dense first_dense (_weights[0], _biases[0], RELU);
  Dense second_dense (_weights[1], _biases[1], RELU);
  Dense third_dense (_weights[2], _biases[2], RELU);
  Dense final_dense (_weights[3], _biases[3], SOFTMAX);
  mat = first_dense (mat);
  mat = second_dense (mat);
  mat = third_dense (mat);
  mat = final_dense (mat);
}

/**
   * Applies the entire network on input, doesn't change input!
   * @param mat given matrix to apply on
   * @return digit struct
   */
digit MlpNetwork::operator() (Matrix const &mat) const
{
  digit result;
  int index = 0;
  float max;
  Matrix final_mat = mat;
  this->dense_mat (final_mat);
  max = final_mat[0];
  for (int i = 1; i < final_mat.get_cols() * final_mat.get_rows(); ++i)
    {
      if (final_mat[i] > max)
        {
          max = final_mat[i];
          index = i;
        }
    }
  result.value = index;
  result.probability = max;
  return result;
}
