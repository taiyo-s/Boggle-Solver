# Boggle Solver

A solver for the board game Boggle.

# What is Boggle?

The game of Boggle is normally played on a 4x4 grid. 16 dice, each die printed with six characters from the letters A to Z, are placed in a closed container and the container is shaken. The dice land in the 4x4 grid and the letter on the top face is the letter for that grid square. A three-minute timer is typically started and players try and make as many words from the letters on the grid as they can before time runs out. Words must be at least 3 letters in length. Each letter after the first must be a horizontal, vertical, or diagonal neighbor of the one before it. No individual letter cube may be used more than once in a word.

# How It Works
It implements Depth First Search and a Prefix Trie data structure to compute a list of all the possible valid words, given a board in the game. There are 2 different algorithms. One which retrieves the valid words and another algorithm that assists the user by giving hint for the next letter that can be played to make a word.

# How To Use - Retrieving all valid words

1. [Download](https://github.com/taiyo-s/Boggle-Solver) the Github Repo

2. For Windows: Simply double click "getwordswin.exe"  
For Mac: double click "getwordsmac.command"

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

# How To Use - Give hint

1. [Download](https://github.com/taiyo-s/Boggle-Solver) the Github Repo

2. For Windows: Simply double click "gethintwin.exe"  
For Mac: double click "gethintmac.command"

3. Wait for "Enter Board Here" and then input in the board from left to right, followed by a newline character (Press Enter):

i.e. 

Board:
      
	a b c d      
    e f g h
    i j k l
    m n o p

Input:

	abcdefghijklmnop

Example:

    Enter Board Here: abcdefghijklmnop
    Given board:
    A B C D
    E F G H
    I J K L
    M N O P
    Valid words:
    abc
    feb
    fin
    gba
    ink
    jim
    jon
    knife
    min

4. Wait for "Enter Partial Word Here" and then input a partial word, followed by a newline character (Press Enter):

i.e. 

Input:

    abcdefghijklmnop
	j

Example:

    Enter Board Here: abcdefghijklmnop
    Given board:
    A B C D
    E F G H
    I J K L
    M N O P
    Enter Partial Word Here: j
    Hint(s):
    i
    o

5. A list of letters following the given input that can follow on the boggle board will be shown.
  
