//
// Created by Aman LaChapelle on 6/22/16.
//

#ifndef NUMERICALINTEGRATION_SOLVER_HPP
#define NUMERICALINTEGRATION_SOLVER_HPP

#include <Eigen/Dense>
#include <complex>
#include <fstream>
#include <unistd.h>
#include <gflags/gflags.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

DEFINE_string(current_path, "", "Specifies where the CODE files are starting from the top directory.  "
        "Required if the boost implementation doesn't work on your system.");

namespace bp = boost::python;

template<typename T>
class Solver {

//Convenience Typedefs
protected:
  typedef Eigen::Matrix<std::complex<T>, Eigen::Dynamic, 1> VectorXt;
  typedef Eigen::Matrix<std::complex<T>, Eigen::Dynamic, Eigen::Dynamic> MatrixXt;
  typedef Eigen::Matrix<T, Eigen::Dynamic, 1> VectorXT;
  typedef Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> MatrixXT;


//Internal Datatypes
protected:
  const std::complex<T> ii;
  const MatrixXt Hamiltonian;
  const VectorXt Psi_0;

//Getters and Setters
public:
  const std::complex<T> &getii() const {
    return ii;
  }

  const MatrixXt &getHamiltonian() const {
    return Hamiltonian;
  }

  const VectorXt &getPsi_0() const {
    return Psi_0;
  }

//Constructors/Destructors
public:
  //Default/expected initialization
  Solver(MatrixXt Ham, VectorXt Psi0, std::complex<T> i): Hamiltonian(Ham), Psi_0(Psi0), ii(i) {
    std::string usage ("See README.md for more info:\n"
                               "./NumericalIntegration </path/to/code/files(specifically the .py file)>");
    google::SetUsageMessage(usage);
  }
  virtual ~Solver() noexcept {;}

  //Copy constructors - Move constructors are omitted because they are unlikely to be used
  Solver(const Solver& other): Hamiltonian(other.Hamiltonian), Psi_0(other.Psi_0), ii(other.ii){;}

  Solver& operator=(const Solver& other){
    Solver tmp(other);
    *this = std::move(tmp);
    return *this;
  }

//Internal method - loop with this so want to keep it internal
protected:
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

//Public interface to the evaluate function
public:
  inline MatrixXt Solve(T step_size, std::size_t num_steps){
    MatrixXt solution (3, 1);

    solution.col(0) = Psi_0;

    for (std::size_t i = 1; i < num_steps; i++){
      _evaluate(step_size, solution);
    }

    return solution;

  }

  inline void logSolution(MatrixXt& solverOutput, std::size_t num_runs){
    char buff[PATH_MAX];

    char* cwd = getcwd(buff, PATH_MAX+1);

    std::string path;

    if (cwd != NULL){
      path = cwd;
    }
    else{
      throw std::runtime_error("Current Working Directory not found!");
    }

    std::ofstream out_file ((path + "output_data.txt").c_str());

    out_file << "Real and then Imaginary parts" << std::endl;
    for (int i = 0; i < 2; i++){
      for (int j = 0; j < num_runs; j++){
        out_file << (solverOutput.real())(i,j) << ", ";
      }
      out_file << std::endl;
    }
    out_file << std::endl;

    for (int i = 0; i < 2; i++){
      for (int j = 0; j < num_runs; j++){
        out_file << (solverOutput.imag())(i,j) << ",";
      }
      out_file << std::endl;
    }

    out_file << std::endl;

    out_file.close();

  }

  inline void plotSolution(MatrixXt& solverOutput, std::size_t num_runs, T step){
    char buff[PATH_MAX];

    char* cwd = getcwd(buff, PATH_MAX+1);

    std::string path;

    if (cwd != NULL){
      path = cwd;
    }
    else{
      throw std::runtime_error("Current Working Directory not found!");
    }

    std::vector<T> r0, r1, i0, i1;
    std::vector<T> linspace, zeroSquared, oneSquared;


    std::ofstream re0 ((path + "r0.tsv").c_str());
    std::ofstream re1 ((path + "r1.tsv").c_str());
    std::ofstream im0 ((path + "i0.tsv").c_str());
    std::ofstream im1 ((path + "i1.tsv").c_str());

    for (int i = 0; i < 2; i++){
      for (int j = 0; j < num_runs; j++){
        if (i == 0){
          re0 << solverOutput.real()(i,j) << "\t";
          r0.push_back(solverOutput.real()(i,j));
          im0 << solverOutput.imag()(i,j) << "\t";
          i0.push_back(solverOutput.imag()(i,j));

          linspace.push_back(j*step);
          zeroSquared.push_back(pow(r0[j], 2) + pow(i0[j], 2));
        }
        else if (i == 1){
          re1 << solverOutput.real()(i,j) << "\t";
          r1.push_back(solverOutput.real()(i,j));
          im1 << solverOutput.imag()(i,j) << "\t";
          i1.push_back(solverOutput.imag()(i,j));

          oneSquared.push_back(pow(r1[j], 2) + pow(i1[j], 2));
        }
      }
    }

    re0.close();
    re1.close();
    im0.close();
    im1.close();

    //Plot the data with matplotlib
    Py_Initialize();

      try{
        bp::object plt = bp::import("matplotlib.pyplot");
      }
      catch (bp::error_already_set){
        PyErr_Print();
        std::cerr <<
                "\n==========================\nResorting to plotting with system() calls...\n==========================\n"
                      << std::endl;
        Py_Finalize();

        if (FLAGS_current_path == ""){
          std::cerr << "Need a file path, see README or run with the --help flag for more." << std::endl;
          return;
        }

        //Only use this in case boost doesn't work...
        system(("python " + FLAGS_current_path + "/plot.py " + path).c_str());
        return;
      }


      bp::class_< std::vector<T> >("PyVec").def(bp::vector_indexing_suite< std::vector<T> >());

      try{
        bp::object main = bp::import("__main__");
        bp::object global (main.attr("__dict__"));

        bp::dict dataset;

        dataset["r0"] = r0;
        dataset["r1"] = r1;
        dataset["i0"] = i0;
        dataset["i1"] = i1;
        dataset["lin"] = linspace;
        dataset["zeroS"] = zeroSquared;
        dataset["oneS"] = oneSquared;

        bp::exec(
            "import matplotlib.pyplot as plt\n"

            "plt.figure()\n"
            "plt.plot(r0, i0)\n"
            "plt.plot(r1, i1)\n"

            "plt.figure()\n"
            "plt.plot(lin, zeroS)\n"
            "plt.plot(lin, oneS)\n"
            "plt.show()\n",
            global, dataset
        );

      }
      catch (bp::error_already_set){
        PyErr_Print();
      }

    Py_Finalize();
  }

};


#endif //NUMERICALINTEGRATION_SOLVER_HPP
