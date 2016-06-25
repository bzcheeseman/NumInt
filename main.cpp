#include <iostream>
//#include "Solver_v0.1.hpp"
#include "Solver.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  google::ParseCommandLineFlags(&argc, &argv, false);

  std::complex<double> Delta, J, zero, one;

  Delta = -1.0;
  J = 0.1;
  zero = 0.0;
  one = 1.0;

  Eigen::MatrixXcd Ham (3,3);
  Ham << Delta, J, zero, J, zero, J, zero, J, zero;

  std::vector<double> init = {1.0, 0.0, 0.0};

  //Solver_v0<float> solver (Ham, initial, ii);

  long num_runs = 7000;
  double step_size = 0.001;

  Solver slvr (num_runs, step_size, 3, Ham.data(), 9);

  std::cout << slvr.Solve(init) << std::endl;

//  Eigen::MatrixXcf output = solver.Solve(step_size, num_runs);
//
//  try{
//    solver.logSolution(output, num_runs);
//    //solver.plotSolution(output, num_runs, step_size);
//  }
//  catch (const std::exception& e){
//    std::cout << e.what() << std::endl;
//  }

  return 0;
}