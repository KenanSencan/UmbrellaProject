# How math projects are structured
- So far every project's structure is: 

```c++
1. #include "../../HelperClass/HelperCollection.h"
2. Global variable declarations
3. Specific Math functions declarations and definitions. 
    (This part is most important. The functions specifically designed to be easily referencing)
4. Calles MAIN_TEMPLATE_GAME_START
5. All the project/game and drawing related functionalities. (they should be as concise as possible)
6. Calles MAIN_TEMPLATE_GAME_END
```

- I guess the projects inside Math folder are the most important information ever defined in the entire repository.

- There's a `HelperClass` folder. Every math project uses the helper functionalities inside that folder.