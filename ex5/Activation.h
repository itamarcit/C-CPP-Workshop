//Activation.h
#ifndef ACTIVATION_H
#define ACTIVATION_H
#include "Matrix.h"
#include <cmath>

/**
 * @enum ActivationType
 * @brief Indicator of activation function.
 */
enum ActivationType {
    RELU,
    SOFTMAX
};

class Activation {
 private:
  ActivationType type;
 public:
  Activation (ActivationType act_type); // Constructor
  ActivationType get_activation_type () const; // Getter
  /**
   * Applies activation function on the given matrix
   * @param mat Given matrix
   * @return A *new* matrix that the activation function is applied on
   */
  Matrix operator() (Matrix const &mat) const;

};
#endif //ACTIVATION_H
