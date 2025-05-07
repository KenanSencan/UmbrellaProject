# What's Projected vector
- We need to use this **Projection calculations** to determine whether the player is moving forward or backward.
- (no rotation made) When I press D it will go to yellow (direction) line just like red (velocity) line.
- When I press `A + S` it will go to opposite direction of yellow line. Whereas red line (velocity) is just going where the player should go based on the input.

### Calculation of Projected Vector

### What's the use of `projected Speed`
- If projected speed is 0, it means **we are not moving**
- IF it's > 0, we are **moving forward** 
- If it's < 0 we are **moving backward**.
- Try to make rotation and see based on key press, how it's considered as moving forward/backward.
___

## Projection velocity calculation: 
```c++
//If > 0, we are moving forward.
//< 0 we are moving backward. 
//==0 we are not moving
ProjectedSpeed = Math::Dot(Velocity, Direction); 

//Direction is not input direction. It's the Yellow Line. 
//In short if ProjectedSpeed is positive, it will be same as Yellow Line (Direction).
//If it's negative, it will be opposite of yellow line.
ProjectedVector = Direction * ProjectedSpeed;
```

### What's the use of `LateralSpeed`
- We know when we move **forward** and **backward**.
- We need to learn if we are moving **Up or Down**
- If we are moving **Up** with `W`, the blue line needs to show **Upwards**. Same for going down with `S` key.

Here's the calculation of **lateral direction and speed**:
- (no rotation made) when moving Upwards with `W` the **Forward Direction** (green line) and it's **speed** will be `0`
- But `LateralSpeed` will be displayed.

Here's the calculation of **lateral direction and speed**:
- We can create a perpendicular vector by swapping x/y and negating one

```c++
const sf::Vector2f lateralDirection(-Direction.y, Direction.x);
LateralSpeed = Math::Dot(Velocity, lateralDirection);
```
___

The overlay text will correctly show the direction of the player based on the key presses:
- D: Forward
- A: Backward
-  A + W: Backward + Left
- A + S: Backward + Right
- S + D: Forward Right
- S + E: Forward Left



# Other non projection vector calculation related stuffs:
___
# How slowing down works

- `drag` = 0.9
- `PlayerVelocity *= drag;` Every time this executes, the velocity will decrease %10

___

# How two key pressing works

- Pressing just W: inputDirection = `(0,-1)`, length = 1
- Pressing just A: inputDirection = `(-1,0)`, length = 1
- Pressing W+A simultaneously: inputDirection = `(-1,-1)`, length = `√2 ≈ 1.414`
- W+A: (-1,-1) → normalized to approximately (-0.707,-0.707) → length 1
- Since length always equals 1 for just pressing `W` or `W + E`, the player will always move at the same speed regardless of the direction.

___

# Velocity calculation:

Character's velocity calculated as:

```c++
// NOTE: Normalize input direction and apply movement speed
if (Math::Length(inputDirection) > 0)
{
    // In case it's going diagonal with W E keys, it'll be: 1.41421 we need to normalize it.
    inputDirection = Math::Normalize(inputDirection);

    // Apply movement speed
    PlayerVelocity += inputDirection * moveSpeed;
}

// NOTE: Apply drag to slow down
PlayerVelocity *= drag;
```
And position of the player is updated with:

```c++

// NOTE: Update position based on velocity
PlayerPosition += PlayerVelocity;
player.setPosition(PlayerPosition);
```

# Red line:

Red line represents Hero Position to Velocity. is calculated as:

```c++
velocityLine[0].position = position;
velocityLine[1].position = position + (velocity * 15.f); // Scale for visibility
```

# Green line

This is the projected velocity. Calculated as

```c++
ProjectedSpeed = Math::Dot(Velocity, Direction);
ProjectedVector = Direction * ProjectedSpeed;
```


