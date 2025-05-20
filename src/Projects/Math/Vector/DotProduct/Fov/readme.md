# Field of View detection

# Formula of Field Of View  calculation: 
```c++
        bool isInFieldOfView = (playerDirection.dot(playerToEnemy) > someCosineThreshold);
```

# Extremely important rule:
1. FOV calculation in short means checking if the angle between two vectors is less than a certain degree.
2. **Two normalized vector's dot product equals the cosine of the angle between them**.
3. So if we can find the angle between two vectors, we can check if the angle is less than a certain degree.
4. Inverse functions like `arccos` can find the angle if you know the sides or ratios.
5. Since in statement 2, `cosine` involved, we should use `arccos` to find angle between two vectors.
6. But `arccos` is expensive, especially in tick so we need another method.
___

# Optimum way to calculate FOV instead:
1. I already mentioned above in 2th statement, **the dot product of two normalized vectors equals the cosine of the angle between them.**
2. so we can check if in threshold with `dot(playerDirection, playerToEnemy) > cos(angle)` (assumed all vectors normalized)
3. Do not forget to halve the angle, because FOV needs to extend equally in both directions 
4. Also all trigonometric functions like `std::cos` wants radians, so we need to convert degrees to radians.

# Example of FOV calculation:
**HeroDirection:** `1,0`

PlayerToEnemy: `0.415 -0.909`

Their dot product: `0.415`

fovDegrees: `90°`

angle between hero to enemy = `arccos(0.415) ≈ 65.5` degrees

FOV `90°` (45° on each side), threshold is `cos(45°) = 0.7071`

Since `0.415 < 0.7071`, the enemy is outside of isInAngle

___

