# Basic interface logic 
- Pure interfaces should NOT have member variables - only virtual destructor and pure virtual methods.

# `IDrawable`
`IDrawable.h` contains basic interface for drawable objects.
it contains `Draw` function which is pure virtual and should be implemented by derived classes.

 The logic is simple. Every Drawable object should implement `Draw` method.
 

# `CircleShape`

`CircleShape.h` is a class that inherited from `IDrawable` interface.
- So it has to implement `Draw` method.


# Main class 
Look here 
```cpp
std::vector<IDrawable*> drawables;
//any IDrawable inherited object is suitable for here
drawables.push_back(new CircleDrawable{100, 100, 50, sf::Color::Red});
drawables.push_back(new RectangleDrawable{300, 200, 100, 50, sf::Color::Blue});
drawables.push_back(new CircleDrawable{500, 300, 30, sf::Color::Green});
```

Different objects can be added to the vector, as long as they implement the `IDrawable` interface.