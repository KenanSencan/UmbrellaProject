This project is only for experimenting cmake, benchmarking optimization for different solutions and learning Data Structure And Algorithms. In the future, I'will make algorithm visualizations.  

# Clone the repository with submodules
`git clone --recurse-submodules https://github.com/Selviniahh/CMakeSFMLProject.git`

# Build the project
1. ```cd CMakeSFMLProject && mkdir build && cd build```
2. ```cmake -DCMAKE_BUILD_TYPE=Release -G Ninja  -S .. -B .``` 
**Cmake** will print all the targets designated as **executable.**
3. ```cmake --build . ``` will build every executable. If specific target desired. Choose one from printed executable names during configuration execution.

# Mine 
```less
/usr/bin/cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=/usr/bin/ninja -DCMAKE_C_COMPILER=/usr/bin/clang-19 -DCMAKE_CXX_COMPILER=/usr/bin/clang++-19 -G Ninja -S /home/selviniah/CLionProjects/CMakeSFMLProject -B /home/selviniah/CLionProjects/CMakeSFMLProject/build
```

```less
/usr/bin/cmake --build . --target StringMain -j 14
```