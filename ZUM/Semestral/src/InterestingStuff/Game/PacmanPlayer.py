from typing import List, Tuple

from Game import Game
from Game.Game import Move
from InterestingStuff.Game.Player import Player
from InterestingStuff.GameUtils import get_closest_food_dist
import random


class PacmanPlayer(Player):
    def win(self, game):
        pass

    def __init__(self, game: Game, pacman):
        super().__init__(game)
        self.pacman = pacman

    def evaluation(self, game: Game) -> float:
        val = game.g_map.distance_evaluator.get_dist(self.pacman[0], self.game.g_map.Blinky,
                                                     game.g_map.distance_evaluator.get_dist_manhattan)

        val += game.g_map.distance_evaluator.get_dist(self.pacman[0], self.game.g_map.Speedy,
                                                      game.g_map.distance_evaluator.get_dist_manhattan)

        val += game.g_map.distance_evaluator.get_dist(self.pacman[0], self.game.g_map.Inky,

                                                      game.g_map.distance_evaluator.get_dist_manhattan)

        val += game.g_map.distance_evaluator.get_dist(self.pacman[0], self.game.g_map.Clyde,
                                                      game.g_map.distance_evaluator.get_dist_manhattan)

        min_food_dist = get_closest_food_dist((self.pacman[0].x, self.pacman[0].y), self.game.g_map)

        if min_food_dist == 0:
            min_food_dist = 0.1

        ret = self.game.mocked_score - (1 / val) * game.scatter() + (1 / min_food_dist)

        if game.scatter() == -1:
            ret = 1 / val

        return ret

    def heuristics(self, game: Game) -> List:
        s = game.rules.get_walkable_neighbouring(self.pacman[0].x, self.pacman[0].y)
        s = sorted(s, key=lambda x: game.g_map.distance_evaluator.get_dist_manhattan(x, (
            self.game.g_map.Clyde.x, self.game.g_map.Clyde.y))
                                + game.g_map.distance_evaluator.get_dist_manhattan(x, (
            self.game.g_map.Inky.x, self.game.g_map.Inky.y)) +
                                game.g_map.distance_evaluator.get_dist_manhattan(x, (
                                    self.game.g_map.Speedy.x, self.game.g_map.Speedy.y))
                                + game.g_map.distance_evaluator.get_dist_manhattan(x, (
            self.game.g_map.Blinky.x, self.game.g_map.Blinky.y)))

        for i in s:
            m = [self.pacman[0], i]
            game.mock_move([m])
            yield [m], 1
            game.revert_move()
