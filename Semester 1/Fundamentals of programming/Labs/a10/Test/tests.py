import unittest

from domain.game import *
from domain.board import *
from domain.square import *


class Tests(unittest.TestCase):
    def testSquare(self):
        sq = Square(2, 3)
        self.assertEqual(sq.getX(), 2)
        self.assertEqual(sq.getY(), 3)
        self.assertEqual(str(sq), '2 3')

    def testBoard(self):
        board1 = Board(3, 3)
        self.assertEqual(board1.makeMove(Square(1, 1), 'x', 1, 3), None)
        self.assertEqual(board1.makeMove(Square(3, 3), 'x', 3, 3), None)
        board2 = Board(1, 3)
        self.assertEqual(str(board2.winningMove(1, 3)), str(Square(1, 2)))
        board3 = Board(2, 2)
        board3.makeMove(Square(1, 1), 'x', 2, 2)
        self.assertEqual(board3.isWon(2, 2), True)
        board4 = Board(3, 3)
        board4.makeMove(Square(2, 1), 'x', 3, 3)
        self.assertNotEqual(board4.emptySq(3, 3), None)

    def testGame(self):
        board = Board(4, 4)
        game = Game(board)
        self.assertEqual(game.movePlayer(Square(1, 1), 4, 4), None)
        self.assertEqual(game.movePlayer(Square(3, 3), 4, 4), None)
        board2 = Board(3, 3)
        self.assertEqual(game.moveRandomEf(3, 3), None)
        board3 = Board(6, 6)
        game3 = Game(board3)
        self.assertEqual(game3.moveComputer(5, 5, 2, Square(1, 1)), None)

    def setUp(self):
        unittest.TestCase.setUp(self)

    """
        This function is called after all test function are executed
        It's like the opposite of setUp, here you dismantle the test scaffolding
    """

    def tearDown(self):
        unittest.TestCase.tearDown(self)
