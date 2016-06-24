#include <iostream>
#include "Solver.hpp"

using namespace std;


int main(int argc, char* argv[]) {
  google::ParseCommandLineFlags(&argc, &argv, false);

  std::complex<float> ii (0.0, 1.0);

  std::complex<float> Delta, J, zero, one;

  Delta = -1.0;
  J = 0.1;
  zero = 0.0;
  one = 1.0;

  Eigen::MatrixXcf Ham (2,2); //haven't tested it on more than a 2 level system but it *should* still work
  Ham << Delta, J, J, zero;

  Eigen::VectorXcf initial (2); //haven't tested it on more than a 2 level system but it *should* still work
  initial << one, zero;

  Solver<float> solver (Ham, initial, ii);

  long num_runs = 7000;
  float step_size = 0.001;

  Eigen::MatrixXcf output = solver.Solve(step_size, num_runs);

  try{
    solver.logSolution(output, num_runs);
    solver.plotSolution(output, num_runs, step_size);
  }
  catch (const std::exception& e){
    std::cout << e.what() << std::endl;
  }

  return 0;
}