# CuBirds 
A C++ version of the card game CuBirds which you can use to find an optimal strategy for CuBirds. 

## Description
This project implements a number of CuBirds players which you can play against each other for several rounds to find the best player

## Installation
Clone the repository: <br/> 
```git clone https://github.com/NoelleBoer/CuBirds.git```<br/> 
Navigate to the project directory:<br/> 
```cd CuBirds```<br/> 
Install dependencies:<br/> 
```make```<br/> 
Run the project:<br/> 
```./CuBirdsGame```<br/> 

## Usage
The implementation is build for a 2 player version of CuBirds. (This could easily be changed to a 2+ player version). To test the different players you can change the playertype in main.cpp.

## Implement 2+ version
To change the number of players change the following functions:<br/> 
main() in main.cpp -> change the size of the array arr<br/> 
game() in game.cpp -> more arguments and more players pushed to the player vector<br/> 
