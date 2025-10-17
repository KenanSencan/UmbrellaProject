Look at: /home/selviniah/Documents/obsidian/Techincal Stuffs/Concurrency/Book/Chapter2/naive Parallel accumulate vs sequential & more.md

- Imagine in a game any Enemy's Pathfinding or any algorithm that runs in Update can block the main thread
- Assuming the algorithm makes minimal to no sharing, I can parallelize this operation 
- No matter how complex `HeavyWorkOptimized` can be, main thread won't be blocked so `Player` will run without any freeze
- Each Enemy has their own thread and `HeavyWorkOptimized` will run in it's own thread. 

This is perfect and simple parallelism example 

# However still this project sucks
1. Each NPC has it's own member thread variable. What if I have 1000 NPC. Will I have 1000 threads? 
2. `HeavyWorkOptimized` creates `hardware_threads` amount of threads for each loop. Imagine we are looping for a billions of times, I will have a major thread allocation and deallocation overhead.

Also `htop` couldn't capture allocated threads because they are allocated and deallocated in nanoseconds  