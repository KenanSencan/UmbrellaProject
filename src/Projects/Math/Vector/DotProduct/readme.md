# Most important projects (ordered)
1. MOST IMPORTANT is `Fov
2. `LookAtTrigger`
3. Even though not very related with dot product `MovementProjection`

# Examples of dot product
## Field of View Detection (done)
```cpp
bool isInFieldOfView = (playerDirection.dot(playerToEnemy) > someCosineThreshold);
```
- Detects if objects are within a character's vision cone
- Perfect for stealth games, enemy sight detection
## Front/Behind Detection (done)
```cpp
bool isInFront = (playerDirection.dot(playerToEnemy) > 0);
```
- Determines if an object is in front or behind another
- Useful for `backstab` mechanics, shield blocks, enemy awareness
## Movement Projection (Done)
```cpp
float forwardSpeed = velocity.dot(forwardDirection);
```
- Finds how fast something is moving in a specific direction
- Used for character controllers, slopes, determining input directions
## Light Calculations (somewhat done, more focused on normal vector understanding)
```cpp
float lightIntensity = normal.dot(lightDirection);
```
- Basic diffuse lighting calculation
- Creates proper shading based on surface orientation
## Determining Alignment (currently studying this )
```cpp
float similarity = direction1.normalize().dot(direction2.normalize());
```
- Measures how aligned two directions are (-1 to 1)
- Useful for AI decision making, team formation, targeting systems
## Reflection Calculations (study this separately)
```cpp
reflectionVector = velocity - 2 * normal.dot(velocity) * normal;
```
- Calculates bounce angles for projectiles, balls, etc.
- Essential for physics systems and collision responses
  The dot product is one of the most frequently used vector operations in game development, primarily for directional comparisons and component analysis.