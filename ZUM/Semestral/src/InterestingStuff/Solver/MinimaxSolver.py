import copy
from typing import Generator

from Core import Movable
from InterestingStuff.Game import Player
from InterestingStuff.Solver.Solver import Solver
import sys


class MinimaxSolver(Solver):

    def __init__(self, game, pacman: Player, ghosts: Player, depth):
        super().__init__(game, pacman, ghosts)
        self.max_depth = depth
        self.knots = 0
        self.last_knots = self.knots

    def steps_generator(self) -> Generator:
        i = 0
        while not self.game.is_over:
            to_control = self.player2 if i % 2 == 0 else self.player1
            yield self.resolve_next_step(to_control)
            i += 1

    def resolve_next_step(self, to_control):
        res = self.examine_search_tree(0, to_control, -sys.maxsize, sys.maxsize)
        print('Knots {0}'.format(self.knots))
        self.knots = 0
        if res[1] is None:
            return self.game
        for i in res[1]:
            self.game.accept_move(i[0], i[1])
        return self.game

    def examine_search_tree(self, depth, curr: Player, alpha, beta, knots=0):
        # check for end state
        end = self.game.end_state()
        if self.max_depth == depth or end[0]:
            if end[0]:
                return (100 - depth) * self.game.scatter() * end[1], None
            val = self.player1.evaluation(self.game)
            return val, None

        last_move = None

        if curr == self.player1:
            max_eval = -sys.maxsize
            curr_max = max_eval
            for m in self.player1.heuristics(self.game):
                self.knots += 1
                # Recur to minimizing player
                res = self.examine_search_tree(depth + 1, self.player2, alpha, beta, knots)
                if res[0] > curr_max:
                    curr_max = res[0]  # Winning after less rounds is better
                    if len(m[0]) > 0:
                        last_move = m[0]
                alpha = max(alpha, res[0])
                if beta <= alpha:  # Prune
                    for _ in range(m[1]):
                        self.game.revert_move()
                    break
            return curr_max, last_move
        else:
            min_eval = sys.maxsize
            curr_min = min_eval
            for m in self.player2.heuristics(self.game):
                self.knots += 1
                # Recur to minimizing player
                res = self.examine_search_tree(depth + 1, self.player1, alpha, beta, knots)
                if res[0] < curr_min:
                    curr_min = res[0]
                    if len(m[0]) > 0:
                        last_move = m[0]
                beta = min(beta, res[0])
                if beta <= alpha:  # Prune
                    for _ in range(m[1]):
                        self.game.revert_move()
                    break
            return curr_min, last_move
