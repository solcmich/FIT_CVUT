from abc import abstractmethod

from Core import Movable


class Solver:
    def __init__(self, game, player1, player2):
        self.game = game
        self.player1 = player1
        self.player2 = player2

    @abstractmethod
    def steps_generator(self) -> tuple:
        raise NotImplemented
