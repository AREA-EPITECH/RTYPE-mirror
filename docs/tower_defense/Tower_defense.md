# What is a Tower Defense Game ?

A tower defense game is a type of strategy game where the player has to defend a territory from enemies through waves by placing defensive structures along their path. 
The enemies usually follow a predetermined path, and the player has to strategically place towers to stop them from reaching their destination. 
The player earns resources by defeating enemies, which can be used to build more towers or upgrade existing ones. 
The goal of the game is to prevent the enemies from reaching the end of the path, which usually results in the player losing a life. 

# How to play ?

## Launch game

To launch the game, the player has to run the following command:
```
./r-type-tower_defense <map file>
```
The map file must be a valid JSON file containing the map information and the waves of enemies.
The JSON format to follow is specified in the 'Map.md' file.
An example of a map file can be found in the `maps` folder.


## Controls

The game is only playable with the left click.
With it, you can select an empty tile (grass), where you will be able to construct a tower.
Tile that represent the enemies path, those decorated or with a tower on it can't be selected.
You can select a built tower to view it range.


2 buttons are available on the screen :
- The 'Start round' button, located at the bottom right of the screen, to start the wave if it's green.
- The 'Shop' button, located at the top left of the screen, to open the shop and buy a tower.

### Buy a tower

To buy a tower, you first need to select an empty tile.
Then, you can click on the 'Shop' button to open the shop.
Finally, if you have enough money, you can buy it.

## Win / Lose

You either win when you finish the last round or lose when your health reach 0.

## Rounds

On the bottom left of the screen, information about the game are displayed :
- Your current health
- Your current money
- The current wave number
- The map name

Each round is composed of multiple waves, each waves has its own enemies amount and spawns delays.

When a wave is cleared, you can freely build towers and upgrade them until you decide to start the next wave.

### Enemies

The enemies are represented by a sprite moving from the start to the end of the map.

When they reach the final tile of the path, they deal damage to the player's health.

Currently, the game has 3 types of enemies :
- Basic slime : 10 health, 2 speed, 1 damage, 10 money
- Bat : 5 health, 1 speed, 2 damage, 5 money
- Zombie : 15 health, 3 speed, 5 damage, 15 money

The speed statistic is the time in seconds the enemy takes to move from one tile to another.