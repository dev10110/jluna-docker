# jluna + Docker

After cloning this repo, 
```
docker compose build
docker compose up
```

`docker exec -it <container name> bash` to get into the container

Build:
```
cd /root/doubleIt
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

Run the demo executable
```
cd /root/doubleIt/build
./main
```

the expected output is something like
```
hello from cpp
[JULIA][LOG] initialization successful (1 thread(s)).
hello from julia
[0] in: 0.68496 | C++: 1.36992 |  Julia: 1.36992
[1] in: 0.873393 | C++: 1.74679 |  Julia: 1.74679
[2] in: 0.882608 | C++: 1.76522 |  Julia: 1.76522
[3] in: 0.378197 | C++: 0.756395 |  Julia: 0.756395
[4] in: 0.497081 | C++: 0.994162 |  Julia: 0.994162
[5] in: 0.652911 | C++: 1.30582 |  Julia: 1.30582
[6] in: 0.193949 | C++: 0.387897 |  Julia: 0.387897
[7] in: 0.268078 | C++: 0.536156 |  Julia: 0.536156
[8] in: 0.653548 | C++: 1.3071 |  Julia: 1.3071
[9] in: 0.516391 | C++: 1.03278 |  Julia: 1.03278
EQUALITY TEST PASSED
Time C++  : 1.3274 us/loop
Time Julia: 2.3184 us/loop
```

Note, if the build type is set to `Debug` (default) , C++ code takes 11.9 us/loop, but with  `Release` mode, it takes 1.3 us/loop. 


Here is a plot of the timing comparisons between `julia` and `c++`:
![](overhead.png)


## Creating a Julia Wrapped Library


First we build the library
```
cd /root/libDoubleIt
mkdir build
cd build
cmake ..
make
make install
ldconfig # not sure if this is always necessary
```

Next, we try to use it
```
cd /root/useDoubleIt
mkdir build
cd build
cmake ..
make
```

and if we try to run it
```
./main
``` 
we get the following error:
```
./main: error while loading shared libraries: libjulia.so.1: cannot open shared object file: No such file or directory
```

So we need to set the `LD_LIBRARY_PATH`
```
LD_LIBRARY_PATH=/usr/local/julia/lib:$LD_LIBRARY_PATH ./main
```
which has an output
```
hello from cpp
[JULIA][LOG] initialization successful (1 thread(s)).
INITIALIZING myModule
in: 1 2 3 4 5 
out (before doubling): 0 0 0 0 0 
out (after doubling): 2 4 6 8 10
``` 
demonstrating that a we can wrap a julia library using jluna, export it as a shared c++ library, and import it into another c++ project without any jluna dependencies. This is particulary useful since some projects may not be able to use C++20 features.

I am still trying to figure out how we can avoid the dependency on setting the `LD_LIBRARY_PATH`
