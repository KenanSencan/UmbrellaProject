This project is only for experimenting cmake, benchmarking optimization for different solutions and learning Data Structure And Algorithms. In the future, I'will make algorithm visualizations.  

# Clone the repository with submodules
`git clone --recurse-submodules https://github.com/Selviniahh/CMakeSFMLProject.git`

# Build the project
1. ```cd CMakeSFMLProject && mkdir build && cd build```
2. ```cmake -DCMAKE_BUILD_TYPE=Release -G Ninja -S .. -B .``` 
**Cmake** will print all the targets designated as **executable.**
3. ```cmake --build . --target BenchmarkTestMain ``` Choose other executables based on the printed executable names