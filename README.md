# Boggle Solver

A solver for the board game Boggle.

# What is Boggle?

The game of Boggle is normally played on a 4x4 grid. 16 dice, each die printed with six characters from the letters A to Z, are placed in a closed container and the container is shaken. The dice land in the 4x4 grid and the letter on the top face is the letter for that grid square. A three-minute timer is typically started and players try and make as many words from the letters on the grid as they can before time runs out.

# How It Works
It implements Depth First Search and a Prefix Trie data structure to compute a list of all the possible valid words, given a board in the game. Also gives hints on next valid letters.


# How To Use

1. [Download](https://github.com/taiyo-s/Boggle-Solver) the Github Repo

2. For Windows: Simply double click "runwindows.exe"  
For Mac: double click "runmac.command"

3. Wait for "Enter Board Here" and then input in the board from left to right:

i.e. 

Board:
      
	a b c d      
    e f g h
    i j k l
    m n o p

Input:

	abcdefghijklmnop

4. A list of words that are allowed to be made will be shown.
