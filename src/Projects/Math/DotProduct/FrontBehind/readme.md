# Front/Behind Example using dot product

1. subtract `enemy(mouse)` position with player position to get **player To Enemy Relative location.
2. Calculate dot product between `playerForward` and `playerToEnemyRelative`
3. If dot product is negative, it means Enemy is in **behind** 
4. If dot product is positive it means Enemy is in **front**

# we always need a direction to define "front" or "behind."

# Short formula of Front Behind calculation: 
```c++
        bool isInFront = (playerDirection.dot(playerToEnemy) > 0);
```