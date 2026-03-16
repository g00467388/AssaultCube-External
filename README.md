# Setup 

1) Clone the repository via git or download the project as a .zip 
`
git clone https://github.com/g00467388/AssaultCube-External
`

2) Open the **AssaultCube-External.sln** in Visual Studio 2022
3) Set build mode from **Debug->Release** 
4) Set architecture from **x64->x86**, This is required due to assault cube being a 32bit game 
5) Build the program by pressing **CTR+SHIFT+B**
6) Have fun!

<!-- Setup instructions -->

# Usage 
Upon launching the program follow the onscreen instructions 

- Type **1** to enable the aimbot 
- Press **2** to enable the auto kill
- Press **3** to enable unlimited ammo 
- Press **CTRL+C** to exit

# About 
An external aimbot written for assaultcube, the program works by externaly modifying memory by opening a handle to the ac_client.exe process and reading and writing memory via ReadProcessMemory and WriteProcessMemory as opposed to using DLL injection and dereferencing memory locations from within the same memory space as the game. 

# Technologies used 
This project is written in C++ 20, The only non standard libraries used in this project is windows.h and psapi.h 

- windows.h: supplies the ReadProcessMemory and WriteProcessMemory functions. These are used for reading and modifying game memory, such as reading enemy player positions, or modifying the players pitch & yaw allowing them to automatically aim at enemy players.
- psapi.h: Allows us to obtain the module base address to ac_client.exe of which all memory offsets specified in offsets.h are offset from. This is required due to the game being dynamically linked rather than statically linked resulting in memory locations changing upon each launch of the game.  
- tlhelp32.h: 

# Features 
- Aimbot 
Targets the nearest enemy using the [euclidean distance formula.](https://en.wikipedia.org/wiki/Euclidean_distance) 
- Auto kill 
Sets the players X,Y,Z coordinates to the enemies X,Y,Z coordinates and aims at them.
- Unlimited ammo 
Sets the players ammo count to 1337 resulting in the player never having to reload. 

# AI Acknowledgement 
Visual studio autocomplete which is powered by copilot was used throughout this project. No code generation was used throughout this project due to AI being incapable of reverse engineering the game which required manual human labour  

## Aimbot
![aimbot example](https://github.com/g00467388/AssaultCube-External/blob/master/media/aimbot.gif?raw=true)


## Auto kill
![auto kill example](https://github.com/g00467388/AssaultCube-External/blob/master/media/killall.gif?raw=true)


