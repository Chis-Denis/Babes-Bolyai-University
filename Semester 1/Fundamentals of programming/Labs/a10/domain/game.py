import random
from domain.square import Square


class Game:

    def __init__(self, board):
        self.__board = board

    def movePlayer(self, square, lines, columns):

        self.__board.makeMove(square, 'x', lines, columns)

    def moveComputer(self, lines, columns, contor, square):

        '''makes a move for the computer if the lines and columns are odd
            input : lines, columns, a contor , square
            output :-
        '''

        if contor == 1:
            self.__board.makeMove(Square(lines // 2 + 1, columns // 2 + 1), 'o', lines, columns)
        else:
            self.__board.makeMove(Square(lines - square.getX() + 1, columns - square.getY() + 1), 'o', lines, columns)

    def moveRandomEf(self, lines, columns):
        '''this procedure is make the computer to try to make the best move to win if possible if not to move random
            input : lines, columns
            output : -
        '''
        randomList = self.__board.emptySq(lines, columns)
        if not self.__board.winningMove(lines, columns):
            randomSquare = int(random.random() * len(randomList))
            self.__board.makeMove(randomList[randomSquare], 'o', lines, columns)
        else:
            print(self.__board.winningMove(lines, columns))
            self.__board.makeMove(self.__board.winningMove(lines, columns), 'o', lines, columns)