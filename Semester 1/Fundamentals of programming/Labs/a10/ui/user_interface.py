from domain.game import *
from domain.board import *
from domain.square import *


class UI:
    def __init__(self,board, game, lines, columns, option):
        self.board = board
        self.game = game
        self.lines = lines
        self.columns = columns
        self.option = option

    @staticmethod
    def printMenu():
        while True:
            try:
                l = input("Please give the size of the table")
                lines = int(l[0:l.find(',')])
                columns = int(l[l.find(',') + 1:])
                break
            except ValueError:
                print("Invalid size . Ex : 2,3  -  please insert integers")

        if l.find(',') == -1:
            print("Invalid size . Ex : 2,3")
            l = input("Please give the size of the table")

        o = input("Press 1 if you want to start , 2 if you want the computer to start")
        while o != '1' and o != '2':
            print("Please press 1 or 2")
            o = input("Press 1 if you want to start , 2 if you want the computer to start")
        return [lines,columns,o]

    def start(self):
        ok = 0
        cpu = 1
        if self.option == '2':
            print("Cpu moved first")
        contor = 0
        square = Square(0, 0)
        while not self.board.isWon(self.lines, self.columns):

            if self.option == '2':
                if self.lines % 2 == 1 and self.columns % 2 == 1:
                    if cpu != 0:
                        contor = contor + 1
                        self.game.moveComputer(self.lines, self.columns, contor, square)
                else:
                    if cpu != 0:
                        contor = contor + 1
                        self.game.moveRandomEf(self.lines, self.columns)
                print(self.board)

                if not self.board.isWon(self.lines, self.columns):

                    while True:
                        try:
                            l = input("Please give the size position u want to move")
                            x = int(l[0:l.find(',')])
                            y = int(l[l.find(',') + 1:])
                            break
                        except ValueError:
                            print("Invalid dataa")

                    try:

                        square = Square(x, y)
                        self.game.movePlayer(square, self.lines, self.columns)
                        cpu = 1
                    except ValueError as ve:
                        cpu = 0
                        print(ve)
            else:
                print(self.board)
                while True:
                    try:
                        l = input("Please give the position you want to move")
                        x = int(l[0:l.find(',')])
                        y = int(l[l.find(',') + 1:])
                        break
                    except ValueError:
                        print("Invalid dataa")

                try:
                    square = Square(x, y)
                    self.game.movePlayer(square, self.lines, self.columns)
                    cpu = 1
                    ok = 1
                except ValueError as ve:
                    cpu = 0
                    print(ve)
                if cpu != 0 and self.board.isWon(self.lines, self.columns) == False:
                    contor = contor + 1
                    self.game.moveRandomEf(self.lines, self.columns)
                    ok = 0

        print(self.board)

        if ok == 0:
            print("Cpu won")
        else:
            print("You won")