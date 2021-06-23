from typing import List

from Game import Game
from InterestingStuff.Game.PacmanPlayer import PacmanPlayer
from Pacman.Pacman import Pacman


class CantTurnPacman(PacmanPlayer):

    def __init__(self, game: Game, pacman):
        super().__init__(game, pacman)

    def heuristics(self, game: Game) -> List:
        s = game.rules.get_walkable_neighbouring(self.pacman[0].x, self.pacman[0].y)
        p = self.pacman[0].last_pos
        to_consider = []
        for i in s:
            if i != p:
                to_consider.append(i)

        if len(to_consider) == 0:
            to_consider = s

        s = to_consider

        for i in s:
            m = [self.pacman[0], i]
            game.mock_move([m])
            yield [m], 1
            game.revert_move()
