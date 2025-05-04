# Front/Behind Example using dot product

1. subtract `enemy(mouse)` position with player position to get **player To Enemy Relative location.
2. Calculate dot product between `playerForward` and `playerToEnemyRelative` Line 46
3. If dot product is negative, it means Enemy is in **behind** 
4. If dot product is positive it means Enemy is in **front**

# we always need some concept of direction to define "front" or "behind."

```c++
        // Vector from player to enemy
        sf::Vector2f playerToEnemyRelative = enemyPos - playerPos;

        // Compute the dot product
        float dot = (HeroDirection.x * playerToEnemyRelative.x) + (HeroDirection.y * playerToEnemyRelative.y);

        // Determine if the enemy is in front or behind
        // Positive dot => front, Negative dot => behind
        std::string message = (dot >= 0.f) ? "ENEMY IS IN FRONT" : "ENEMY IS BEHIND";
```