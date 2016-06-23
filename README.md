After downloading and unzipping (Make sure to check dependencies!), usage:

```
mkdir build && cd build
cmake ..
make
./NumericalIntegration -current_path=/path/to/code/files(specifically the .py file)
```

In order to play around with parameters, look in the `main.cpp` file,
should be self explanatory.

TO DO:
* Make it easier to change the system size
* Double check and make absolutely sure the calculations are correct



Dependencies:
```
Eigen
gflags
```

On Mac:
```
brew install Eigen
brew install gflags
```

If homebrew is not installed, follow the official [Eigen instructions](http://eigen.tuxfamily.org/index.php?title=Main_Page#Download)
and the official [gflags instructions](https://gflags.github.io/gflags/) to install
the required dependencies.