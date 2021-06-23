from typing import List

from Game import Game
from Ghosts import Ghost
from InterestingStuff.Game.PacmanPlayer import PacmanPlayer
from Pacman.Pacman import Pacman


class CrossroadSmartPacman(PacmanPlayer):

    def __init__(self, game: Game, pacman):
        super().__init__(game, pacman)

    def is_parpendicular(self, p1, p2):
        if abs(p1[0] - p2[0]) == 2 and (p1[1] - p2[1]) == 0:
            return False

        if abs(p1[1] - p2[1]) == 2 and (p1[0] - p2[0]) == 0:
            return False

        return True

    def heuristics(self, game: Game) -> List:
        s = game.rules.get_walkable_neighbouring(self.pacman[0].x, self.pacman[0].y)
        p = self.pacman[0].last_pos
        to_consider = []
        crossroad = False
        move_cnt = 0
        q = list()
        x = []
        while not crossroad:
            for i in s:
                if self.is_parpendicular(i, p) and i != p:
                    crossroad = True
                if i != p:
                    to_consider.append(i)
            q.append(to_consider)

            if len(to_consider) == 1 and not crossroad:
                if len(x) == 0:
                    x = [self.pacman[0], to_consider[0]]
                m = [self.pacman[0], to_consider[0]]
                game.mock_move([m])
                move_cnt += 1
                s = game.rules.get_walkable_neighbouring(self.pacman[0].x, self.pacman[0].y)
                p = self.pacman[0].last_pos
            to_consider = list()

        if len(to_consider) == 0:
            to_consider = s

        s = q.pop()

        for i in s:
            # m = Move(self.pacman, i, (self.pacman.x, self.pacman.y), self.game.g_map.parts[i[1]][i[0]])
            m = [self.pacman[0], i]
            game.mock_move([m])
            yield [x], move_cnt + 1
            game.revert_move()

        for _ in range(move_cnt):
            game.revert_move()
