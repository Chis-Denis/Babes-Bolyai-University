from domain.square import Square
from copy import deepcopy


class Board:
    def __init__(self, lines, columns):

        self.__data = []
        for i in range(0, lines):
            self.__data.append([])
            for j in range(0, columns):
                self.__data[i].append(" ")

    def makeMove(self, square, sign, lines, columns):

        '''
        This function checks whether a move can be made or not and makes the move if possible
            input: the move(square),
                   the sign ( meaning X or O ) ,
                    lines and columns of the table
            output : - (raise Error)
        '''

        if 1 <= square.getX() <= lines and 1 <= square.getY() <= columns:

            if self.__data[square.getX() - 1][square.getY() - 1] == " ":

                self.__data[square.getX() - 1][square.getY() - 1] = sign

                '''

                Below we fill the empty squares around our main square

                '''

                if 1 <= square.getX() - 1 <= lines and 1 <= square.getY() - 1 <= columns:
                    self.__data[square.getX() - 2][square.getY() - 2] = '-'

                if 1 <= square.getX() - 1 <= lines and 1 <= square.getY() <= columns:
                    self.__data[square.getX() - 2][square.getY() - 1] = '-'

                if 1 <= square.getX() - 1 <= lines and 1 <= square.getY() + 1 <= columns:
                    self.__data[square.getX() - 2][square.getY()] = '-'

                if 1 <= square.getX() <= lines and 1 <= square.getY() - 1 <= columns:
                    self.__data[square.getX() - 1][square.getY() - 2] = '-'

                if 1 <= square.getX() <= lines and 1 <= square.getY() + 1 <= columns:
                    self.__data[square.getX() - 1][square.getY()] = '-'

                if 1 <= square.getX() + 1 <= lines and 1 <= square.getY() - 1 <= columns:
                    self.__data[square.getX()][square.getY() - 2] = '-'

                if 1 <= square.getX() + 1 <= lines and 1 <= square.getY() <= columns:
                    self.__data[square.getX()][square.getY() - 1] = '-'

                if 1 <= square.getX() + 1 <= lines and 1 <= square.getY() + 1 <= columns:
                    self.__data[square.getX()][square.getY()] = '-'

            else:
                raise ValueError("Your position is invalid")

        else:
            raise ValueError("Out of board")

    def winningMove(self, lines, columns):

        '''
        The function calculates whether or not the computer can make a move or not
        By making a deepcopy of the board, we can test if doing a move will win the came or not
            input : lines,columns
            output : The move to make or false
        '''

        winningMove = Square(-1, -1)
        emptySquaresList = Board.emptySq(self, lines, columns)
        simulation = deepcopy(self.__data)
        for move in emptySquaresList:
            Board.makeMove(self, move, 'o', lines, columns)
            if Board.isWon(self, lines, columns):
                winningMove = move
                break
            self.__data = deepcopy(simulation)

        self.__data = deepcopy(simulation)
        if winningMove.getX() != -1 and winningMove.getY() != -1:
            return winningMove
        else:
            return False

    def isWon(self, lines, columns):

        '''
        The function checks whether or not the game is won
            input : lines, columns
            output : true or false
        '''

        for i in range(0, lines):
            for j in range(0, columns):
                if self.__data[i][j] == " ":
                    return False

        return True

    def emptySq(self, lines, columns):

        '''returns a list of empty squares'''

        return [Square(i + 1, j + 1) for i in range(0, lines) for j in range(0, columns) if self.__data[i][j] == " "]

    def __str__(self):
        printString = ''
        for i in range(0, len(self.__data)):
            t = self.__data[i]
            if i > 0:
                for k in range(0, len(t)):
                    printString += '----'
            printString += '\n'
            for j in range(0, len(t)):
                printString += ' '
                printString += self.__data[i][j]
                if j < len(t) - 1:
                    printString += ' |'
            printString += '\n'

        return printString