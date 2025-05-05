# Field of View detection

# Formula of Field Of View  calculation: 
```c++
        bool isInFieldOfView = (playerDirection.dot(playerToEnemy) > someCosineThreshold);
```
# Example of FOV calculation:
```less
HeroDirection: 1,0
PlayerToEnemy: 0.415 -0.909
Their dot product: 0.415
fovDegrees: 90°

angle = arccos(0.415) ≈ 65.5 degrees

FOV (45° on each side), threshold is cos(45°) = 0.7071
Since 0.415 < 0.7071, the enemy is outside of isInAngle
```

