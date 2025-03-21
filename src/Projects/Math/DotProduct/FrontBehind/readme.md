# Front/Behind Example using dot product

1. subtract `enemy(mouse)` position with player position to get **player To Enemy Relative location.** Line 43
2. Calculate dot product between `playerForward` and `playerToEnemyRelative` Line 46
3. If dot product is negative, it means Enemy is in **behind** 
4. If dot product is positive it means Enemy is in **front**