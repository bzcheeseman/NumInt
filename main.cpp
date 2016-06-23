#include <iostream>
#include <Eigen/Dense>
#include <fstream>
#include <gflags/gflags.h>

#include <stdlib.h>
#include <unistd.h>

#include "Solver.hpp"

using namespace std;

DEFINE_string(current_path, "", "Specifies where the CODE files are starting from the top directory");


int main(int argc, char* argv[]) {
  google::ParseCommandLineFlags(&argc, &argv, false);

  if (FLAGS_current_path == ""){
    cout << "Need a file path, see README or run with the --help flag for more." << endl;
    return -1;
  }

  std::complex<float> ii (0.0, 1.0);

  std::complex<float> Delta, J, zero, one;

  Delta = 1.0;
  J = 0.1;
  zero = 0.0;
  one = 1.0;

  Eigen::MatrixXcf Ham (2,2); //haven't tested it on more than a 2 level system but it *should* still work
  Ham << Delta, J, J, zero;

  Eigen::VectorXcf initial (2); //haven't tested it on more than a 2 level system but it *should* still work
  initial << one, zero;

  Solver<float> solver (Ham, initial, ii);

  long num_runs = 7000;

  Eigen::MatrixXcf output = solver.Solve(0.001, num_runs);




  /////////////Plotting mechanics that I'll put elsewhere later/////////////

  char buff[PATH_MAX];

  char* cwd = getcwd(buff, PATH_MAX+1);

  string path;

  if (cwd != NULL){
    path = cwd;
  }
  else{
    cout << "cwd is null";
    return 0;
  }


  ofstream re0 ((path + "r0.tsv").c_str());
  ofstream re1 ((path + "r1.tsv").c_str());
  ofstream im0 ((path + "i0.tsv").c_str());
  ofstream im1 ((path + "i1.tsv").c_str());

  ofstream out_file ((path + "output_data.txt").c_str());

  out_file << "Real and then Imaginary parts" << std::endl;
  for (int i = 0; i < 2; i++){
    for (int j = 0; j < num_runs; j++){
      out_file << (output.real())(i, j) << ", ";
      if (i == 0){
        re0 << output.real()(i,j) << "\t";
      }
      else if (i == 1){
        re1 << output.real()(i, j) << "\t";
      }
    }
    out_file << std::endl;
  }
  out_file << std::endl;

  for (int i = 0; i < 2; i++){
    for (int j = 0; j < num_runs; j++){
      out_file << (output.imag())(i, j) << ",";
      if (i == 0){
        im0 << output.imag()(i,j) << "\t";
      }
      else if (i == 1){
        im1 << output.imag()(i, j) << "\t";
      }
    }
    out_file << std::endl;
  }

  out_file << std::endl;

  //checking the magnitudes

  Eigen::MatrixXf out = (output.array()).abs();

  out_file << out << std::endl;

  Eigen::MatrixXf o = out.row(0).array().pow(2) + out.row(1).array().pow(2);

  out_file << "\nTotal population\n" << o << std::endl;

  out_file.close();
  re0.close();
  re1.close();
  im0.close();
  im1.close();

  system(("python " + FLAGS_current_path + "/plot.py " + path).c_str()); //yes this is jank, but boost.python isn't working right now

  return 0;
}