from typing import List, Tuple

from Game import Game
from Game.Game import Move
from InterestingStuff.Game.PacmanPlayer import PacmanPlayer
from InterestingStuff.Game.Player import Player
from InterestingStuff.GameUtils import get_closest_food_dist
import random


class DoublePacman(PacmanPlayer):
    def win(self, game):
        pass

    def __init__(self, game: Game, pacman):
        super().__init__(game, pacman)

    def evaluation(self, game: Game) -> float:
        val = 0
        min_food_dist = 0
        for p in game.g_map.pacman:
            val += game.g_map.distance_evaluator.get_dist(p, self.game.g_map.Blinky,
                                                         game.g_map.distance_evaluator.get_dist_euclid)

            val += game.g_map.distance_evaluator.get_dist(p, self.game.g_map.Speedy,
                                                          game.g_map.distance_evaluator.get_dist_manhattan)

            val += game.g_map.distance_evaluator.get_dist(p, self.game.g_map.Inky,

                                                          game.g_map.distance_evaluator.get_dist_manhattan)

            val += game.g_map.distance_evaluator.get_dist(p, self.game.g_map.Clyde,
                                                          game.g_map.distance_evaluator.get_dist_manhattan)

            min_food_dist += get_closest_food_dist((p.x, p.y), self.game.g_map)

        if min_food_dist == 0:
            min_food_dist = 100

        ret = self.game.mocked_score - (1 / val) * game.scatter() + (1 / min_food_dist)

        if game.scatter() == -1:
            ret = 1 / val

        return ret

    def heuristics(self, game: Game) -> List:
        self.pacman = self.game.g_map.pacman
        if len(self.pacman) > 1:
            p1_moves = game.rules.get_walkable_neighbouring(self.pacman[0].x, self.pacman[0].y)
            for clyde_move in p1_moves:
                m2 = [self.pacman[0], clyde_move]
                game.mock_move([m2])
                p2_moves = game.rules.get_walkable_neighbouring(self.pacman[1].x, self.pacman[1].y)
                for inky_move in p2_moves:
                    m3 = [self.pacman[1], inky_move]
                    moves = (m2, m3)
                    game.mock_move([m3])
                    yield moves, 2
                    game.revert_move()
                game.revert_move()
        else:
            p2_moves = game.rules.get_walkable_neighbouring(self.pacman[0].x, self.pacman[0].y)
            for inky_move in p2_moves:
                m3 = [self.pacman[0], inky_move]
                moves = [m3]
                game.mock_move([m3])
                yield moves, 1
                game.revert_move()
