This is an unbrella cmake projects that contains many subprojects. Every C++ file ending with `main` suffix will be converted to executable.   
1. `src/DSA` is for data structure and algorithms. `CustomDS` imitating data structures from the standard library.  
2. `src/ImportantConcepts` is to experiment C++ concepts 
3. `src/Projects` are for small projects that are not related to the above two.
 
# Clone the repository with submodules
```
git clone --recurse-submodules https://github.com/Selviniahh/CMakeSFMLProject.git
```

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