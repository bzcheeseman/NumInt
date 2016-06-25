//
// Created by Aman LaChapelle on 6/24/16.
//

#ifndef NUMERICALINTEGRATION_SOLVER_HPP
#define NUMERICALINTEGRATION_SOLVER_HPP

#include <Eigen/Dense>
#include <gflags/gflags.h>
#include <complex>
#include <fstream>
#include <stdexcept>
#include <iostream>

DECLARE_string(current_path);

class Solver {

//Internal Data
protected:
  Eigen::MatrixXcd* Hamiltonian;

  //Matrix of the number of states (rows) by the number of steps (cols)
  Eigen::MatrixXcd* Solution;

  //Complex i
  std::complex<double> ii;

  //Solver data required - step size, number of steps and number of states
  unsigned long numSteps, numStates, numParameters;
  double stepSize;

//Constructors and Destructor
public:
  Solver(unsigned long num_steps, double step_size, unsigned long num_states,
         std::complex<double>* parameters, unsigned long num_parameters);

  virtual ~Solver() noexcept;

  Eigen::MatrixXcd& Solve(std::vector<double> Psi_0);

};




#endif //NUMERICALINTEGRATION_SOLVER_HPP
