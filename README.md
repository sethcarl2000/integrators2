# integrators2

## How it works

### Functions
There are generic pseudo-random, quasi-random, and grid-based integrators which work for generic functions, in ```MontecarloIntegrate.cpp```, ```SobolIntegrate.cpp``` and ```GridIntegrate.cpp``` respectivley. The function ```compute_sphere_overalp()``` can use any of these methods to compute the overlap of two offset hyperspheres.  

### executables
the ```ndcrescent``` executable can be passed arguments on the command line: 

```bash
$> ./ndcrescent 10 1e7 1.0 0.5 1.0
```

the ```make_plots``` executable creates both ```convergence.png``` and ```methods.png```, by using the command line option 
```
$> ./make_plots convergence
```
or 
```
$> ./make_plots methods
```


Which would compute the overlap between two 10-balls, with radii 1.0 and 0.5, whose centers are offset by 1.0 (using the stone-throwing method, with 10^7 points). 


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


