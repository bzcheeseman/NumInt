//
// Created by Aman LaChapelle on 6/22/16.
//

#ifndef NUMERICALINTEGRATION_SOLVER_HPP
#define NUMERICALINTEGRATION_SOLVER_HPP

#include <Eigen/Dense>
#include <complex>

template<typename T>
class Solver {

private:

  //convenience typedefs
  typedef Eigen::Matrix<std::complex<T>, Eigen::Dynamic, 1> VectorXt;
  typedef Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> MatrixXt;

private:

  const std::complex<T> ii;
  const MatrixXt Hamiltonian;
  const VectorXt Psi_0;

public:
  Solver(MatrixXt Ham, VectorXt Psi0, std::complex<T> i): Hamiltonian(Ham), Psi_0(Psi0), ii(i) {;}
  virtual ~Solver() {;}

private:
  inline void _evaluate(T step, MatrixXt& Psi_t){
    std::size_t n, j, k;
    n = Psi_t.cols();

    j = Hamiltonian.rows();
    k = Hamiltonian.cols();

    Psi_t.conservativeResize(j, n+1);

    MatrixXt ones = Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic>::Identity(j, k);

    MatrixXt temp = ((step * ii) * Hamiltonian + ones); //leave this here for debugging

    Psi_t.col(n) = temp * Psi_t.col(n-1);
  }

public:
  inline MatrixXt Solve(T step_size, std::size_t num_steps){
    MatrixXt solution (2, 1);

    solution.col(0) = Psi_0;

    for (std::size_t i = 1; i < num_steps; i++){
      _evaluate(step_size, solution);
    }

    return solution;

  }

};


#endif //NUMERICALINTEGRATION_SOLVER_HPP
