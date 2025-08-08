I am trying to make a procedural generation algorithm just like Enter the Gungeon.

Here are the rules:
1. Every room will have an entrance and exit enum values 
2. A room's entrance or exit has to be "Right, Left, Top, Down" 
3. If a room's exit is Down, the next room's entrance should be Top 
	1. The same rule applied for others. If a room's exit is Left, the next room's entrance should be Right 
4. We need pre-existent lots of rooms in a pool. They have difference size as a square. For instance one room can be 40x20 while another room can be 100x100 
5. In SFML, you can use a square whatever you want for sizes
6. As a debug text, write each directions colors. For instance if Right is Red, and Top is yellow, write that as debug text top let 

## Collision
Sometimes rooms can collide when trying to generate something 
in this case move the room to the most appropriate position and create a road path using a* pathfinding algorithm to generate a path
A path has to be 16x16 with a distinct color. 

Lastly I am having "double free or corruption (!prev)" error sometimes for no reason also fix that 
Here's Helper Resources