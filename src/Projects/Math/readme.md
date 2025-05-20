# How math projects are structured
So far every project's structure is: 

```c++
1. #include "../../HelperClass/HelperCollection.h"
2. Global variable declarations
3. Calles GAME_LOOP_START
4. Specific Math functions declarations and definitions. 
    (This part is most important. The functions specifically designed to be easily referencing when needed)
5. Calles MAIN_TEMPLATE_GAME_START
6. All the project/game and drawing related functionalities. (they should be as concise as possible)
7. Calles MAIN_TEMPLATE_GAME_END
```

- I guess the projects inside Math folder are the most important information ever defined in the entire repository.
- There's a `HelperClass` folder. Every math project uses the helper functionalities inside that folder.
___

# Project hierarchy
- I tried to simply the project as much as possible and focus on the math functionalities.
- The projects either focusing on a specific concept like `field of view` or a specific math function like `Dot Product`
- For now the projects are either `Vector` or `Trigonometry`. 