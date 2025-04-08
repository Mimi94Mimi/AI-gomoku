# AI-gomoku
## Introduction
This project defines the algorithm in which an AI gomoku gamer needs to decide where to put their piece at each round. 
The AI would first iterate through every position on the board, and then counts how many of different patterns (e.g. [., O, O, O, X])
occurs at that position. Eventually, it calculate the score of the move at the given position obtained by   
Σ(pattern * weight_of_pattern)

## How to run
main.cpp is the gomoku gaming engine offered by TAs, and this needs to be compiled first by the command:  
`make main`  

If using mac, may require installing the command by:  
`brew install coreutils`  

In order to compile the AI gomoku script into an executable, run:  
`make ai`  

Finally, start the game by running:  
`./main ./ai ./ai`  
and two identical AI gamer will battle with each other.  
Or, the AI gamer can have a challenge with baselines in the /baselines folder which is given by the TA, with the command:  
`./main ./ai <path of baseline>`,  
in which AI gamer plays as black ('O') and baseline plays as white ('X')

## Troubleshooting
If permission denied when accessing baselines, just type:  
`chmod +x baselines/<OS>/*`

## Demo
when executing `chmod +x baselines/mac/baseline1`  

![challenge with baseline1](https://github.com/Mimi94Mimi/AI-gomoku/blob/main/gomoku_demo.gif)
