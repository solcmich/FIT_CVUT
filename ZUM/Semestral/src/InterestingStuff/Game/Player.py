from abc import abstractmethod
from typing import Tuple, List


class Player:
    def __init__(self, game):
        self.game = game

    @abstractmethod
    def heuristics(self, game) -> List:
        pass

    @abstractmethod
    def evaluation(self, game) -> float:
        pass

    @abstractmethod
    def win(self, game):
        pass

