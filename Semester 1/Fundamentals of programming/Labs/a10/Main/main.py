from ui.user_interface import UI
from domain.game import *
from domain.board import *

lines = 0
columns = 1
option = 2

tempArray = UI.printMenu()
board = Board(tempArray[lines],tempArray[columns])
game = Game(board)
UI = UI(board,game,tempArray[lines],tempArray[columns],tempArray[option])
UI.start()