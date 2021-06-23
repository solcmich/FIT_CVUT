import copy
from typing import List

from Game import Game
from Game.Game import Move
from Ghosts import Ghost
from InterestingStuff.Game.Player import Player
from InterestingStuff.GameUtils import GameDistance
import random


class GhostsPlayer(Player):
    def __init__(self, game: Game, blinky: Ghost, speedy: Ghost, clyde: Ghost, inky: Ghost):
        super().__init__(game)
        self.blinky = blinky
        self.speedy = speedy
        self.clyde = clyde
        self.inky = inky

    @staticmethod
    def get_sorted_moves(ghost, pacman, game):
        moves = game.rules.get_walkable_neighbouring(ghost.x, ghost.y)
        return sorted(moves, key=lambda x: game.g_map.distance_evaluator.get_dist_manhattan((
            pacman.x, pacman.y), x), reverse=True)

    def heuristics(self, game) -> List:
        blinky_moves = self.get_sorted_moves(self.blinky, self.game.g_map.pacman[0], game)
        for blinky_move in blinky_moves:
            m = [self.blinky, blinky_move]
            game.mock_move([m])
            speedy_moves = self.get_sorted_moves(self.speedy, self.game.g_map.pacman[0], game)
            for speedy_move in speedy_moves:
                m1 = [self.speedy, speedy_move]
                game.mock_move([m1])
                clyde_moves = self.get_sorted_moves(self.clyde, self.game.g_map.pacman[0], game)
                for clyde_move in clyde_moves:
                    m2 = [self.clyde, clyde_move]
                    game.mock_move([m2])
                    inky_moves = self.get_sorted_moves(self.inky, self.game.g_map.pacman[0], game)
                    for inky_move in inky_moves:
                        m3 = [self.inky, inky_move]
                        moves = (m, m1, m2, m3)
                        game.mock_move([m3])
                        yield moves, 4
                        game.revert_move()
                    game.revert_move()
                game.revert_move()
            game.revert_move()

    def evaluation(self, game) -> float:
        player = self.game.g_map.pacman
        val = game.g_map.distance_evaluator.get_dist(player, self.blinky,
                                                     game.g_map.distance_evaluator.get_dist_actual)

        val += game.g_map.distance_evaluator.get_dist(player, self.speedy,
                                                      game.g_map.distance_evaluator.get_dist_actual)

        return 50 / val

    def win(self, game):
        player = self.game.g_map.pacman
        val = game.g_map.distance_evaluator.get_dist(player, self.blinky,
                                                     game
                                                     .g_map.distance_evaluator.get_dist_actual)
        return val <= 1
