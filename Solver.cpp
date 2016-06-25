//
// Created by Aman LaChapelle on 6/24/16.
//

#include "Solver.hpp"

DEFINE_string(current_path, "", "Specifies where the CODE files are starting from the top directory.  "
        "Required if the boost implementation doesn't work on your system.");

Solver::Solver(unsigned long num_steps, double step_size, unsigned long num_states,
               std::complex<double>* parameters, unsigned long num_parameters) {

  ii = std::complex<double> (0.0, 1.0);

  Hamiltonian = new Eigen::MatrixXcd (num_states, num_states);
  Solution = new Eigen::MatrixXcd (num_states, num_steps);

  numSteps = num_steps;
  numStates = num_states;
  stepSize = step_size;
  numParameters = num_parameters;

  if (num_states * num_states != num_parameters){
    throw std::logic_error("Need num_parameters to match num_states^2 to completely fill the Hamiltonian");
  }

  for (auto j = 0; j < num_parameters; j++){
    Hamiltonian[0].data()[j] = parameters[j];
  }

}

Solver::~Solver() noexcept {
  ;
}


Eigen::MatrixXcd &Solver::Solve(std::vector<double> Psi_0) {
  //Setting up the initial variables, will need these to compute the final solution
  Eigen::MatrixXcd ones = Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic>::Identity(numStates, numStates);

  Eigen::VectorXcd Psi0 (numStates);
  for (auto i = 0; i < numStates; i++){
    Psi0[i] = Psi_0[i];
  }

  Solution->col(0) = (stepSize * ii * (*Hamiltonian) + ones) * Psi0;

  for (auto i = 1; i < numSteps; i++){
    Solution->col(i) = (stepSize * ii * (*Hamiltonian) + ones) * Solution->col(i-1);
  }

  return *Solution;

}