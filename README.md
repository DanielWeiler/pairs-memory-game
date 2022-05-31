# Pairs Memory Game

## Project Description

This project is a memory game where players take turns turning cards to find matching pairs. It is created in C++ with the Qt Creator GUI. 

## Game Instructions and Functionality

At the top of the GUI, there is a header label that guides the players through the required correct inputs and gives status updates about the game. The game setup input fields will be disabled when the inputs are entered but the inputs will still be visible for the players.
 
When setting up the game, the user can choose to have between 2-20 cards and 2-6 players. The user is also asked for a seed value, since the cards will be set randomly in the game board. The default seed value is 0. After entering this information, the user enters the names of the players. The player names are entered one by one.

Once the setup information is entered, the player names will appear on the score board and the cards will appear on the game board. The score board will show the current score of all players throughout the game. There are matching pairs among the cards that the players try to find. The cards are buttons that can be clicked. The cards first appear face down. Once a card is clicked, the card turns over and reveals its letter. On a player's turn, they try to flip two cards over to find a matching pair.

When a player has selected two cards, they must press the 'End turn' button and it will be checked if a pair has been found. If they are pairs, they are removed from the game board and the score of the player is increased. If the cards are not pairs, they will be turned hidden again.

The game will end when all pairs have been found and the game board is empty. The program tells who has won and at what score or what players have tied and at what score.

## Author

Daniel Weiler