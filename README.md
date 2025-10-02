# integrators2

Starter code and examples for MC integration exercise.

See also:

  * https://docs.scipy.org/doc/scipy/reference/generated/scipy.stats.qmc.Sobol.html
  * https://root.cern.ch/doc/master/quasirandom_8C_source.html


# How to compile / run components: 
First, while in the repo's root directory, you must make a new, empty directory called 'build': `mkdir build`. (or whatever name you like): 
Then you can use CMake to compile: 

``` bash
cmake -B build -S .
cmake --build build
```

This will make an executable in the build folder. you can execute it on the command line;
``` bash
./build/integrate
```
But its usually more convenient to make a 'link' to this executable: 
``` bash
ln -s build/integrate integrate
```
Which will add a symbolic link to this executable in the repo's root directory, so to execute you can just run: 
``` bash
./integrate
```


