# Client

## Usage
To start the client, first compile the project with `./build.sh` and then run the client with 
`./r-type_client <host> <port>`. It will start the client and connect it to the server, using the given host and port.

## Models
All the models used by the clients are voxels, except for the backgrounds that are png files.
All the voxels have a metal texture applied on them.
A shader is loaded and applied ont voxels models to receive light and shadows.

## Voxels assets
All voxels files are located in 'client/assets/voxels/'. You can find in it:
- 'player' subfolder containing all the enemy models, in 'spaceship' and 'shot' subfolders.
- 'enemy' subfolder containing all the enemy models, in 'spaceship' and 'shot' subfolders.
- 'title' subfolder containing the title model.
- 'shaders' subfolder containing the shader files. These shaders come from the Raylib examples. 

All models are in '.vox' format, and the shaders are in '.vs' and '.fs' format.

## Background assets
All background files are located in 'client/assets/backgrounds/'. You can find all the png files used as backgrounds.
You can find in it:
- 'game' subfolder containing the game background.
- 'menu' subfolder containing the menu background.
- 'lobby' subfolder containing the lobby background.

## Lights
The lights are created in the client and are applied to the models. These lights come from the Raylib examples.
