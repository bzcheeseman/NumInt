After downloading and unzipping (Make sure to check dependencies!), usage:

```
mkdir build && cd build
cmake ..
make

//And now run the code!
./NumericalIntegration -current_path=/path/to/code/files(specifically the .py file)
```

In order to play around with parameters, look in the `main.cpp` file,
should be self explanatory.  Change parameters and sizing as desired.

TO DO:
* Make it easier to change the system size
* Double check and make absolutely sure the calculations are correct



Dependencies:
```
cmake
Eigen
gflags
boost
boost.python
```

On Mac:
```
brew install cmake
brew install Eigen
brew install gflags
brew install boost --with-python
brew install boost-python
```

If homebrew is not installed, follow the official [Eigen instructions](http://eigen.tuxfamily.org/index.php?title=Main_Page#Download), the official [cmake instructions](https://cmake.org/install/),
and the official [gflags instructions](https://gflags.github.io/gflags/) to install
the required dependencies.  For Eigen, if it's installed in a place other than /usr/local/include/eigen3 then change the -I flag in the CMakeLists.txt file (line 4)

`set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -I <put your install here>")`
the required dependencies.  For boost and boost.python...just use homebrew.  If you really
don't want to, follow the official [boost instructions](http://www.boost.org/doc/libs/1_61_0/more/getting_started/unix-variants.html)
