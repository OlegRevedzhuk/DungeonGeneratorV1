# DungeonGeneratorV1

This personal project is my attempt to take an abstract idea, "Make a dungeon layout randomizer", and find a way
to implement it. I originally thought that it there would be a simple solution, but I soon realized that in order
to connecting all the rooms I generate is not so easy. 

The challenge I ran into is that I needed a way to store all of the generated rooms in one place, and I needed to 
create extra functionality to define where the connections between rooms would be. When I first started to think
about ways to solve this problem, I had not yet been introduced to object oriented programming, but after I studied
more I was able to come back to this problem an make headway.

Armed with a better understanding of how to organize my code, I started to seperate all the functions and objects I
made up to that point into different classes. I ended up with one class which held all the different parts of the 
dungeon, and several classes dedicated to specialized tasks.

The Room class was created to generate a single square room which then could have traps, treasures, and monsters in it.
The DungeonMap class was created to hold the arrangement of each room relative to the others.
The RNGGen class was used as a utility class, mostly for taking care of any randomization throughout the other classes.
The Dungeon class was used to hold all of the above classes and delegate information and the flow of the application.

I am proud of my final solution. No doubt there is more that can be done, but the solution I got does exactly what I
immagined it would do when I first decided to create it.
