from collections import namedtuple

from Core.Constants import *
from Enviroment.MapPart import Cherry
from Ghosts.Ghost import *
from Enviroment.Map import GameMap
import time
import os

from Pacman import Pacman

Move = namedtuple("Move", "movable pos orig_pos map_part last_pos")


class Game:
    def __init__(self, file):
        self.file = file
        self.g_map = GameMap(file)
        self.score = 0
        self.mocked_score = 0
        self.rules = GameRules(self)
        self.moves = list()
        self.scatter_left = 0
        self.is_over = False
        self.collected = 0

    def apply_scatter(self, val):
        for g in [self.g_map.Blinky, self.g_map.Inky, self.g_map.Clyde, self.g_map.Speedy]:
            if g is not None:
                g.scatter = val

    def mock_move(self, moves):
        a = []
        for move in moves:
            move = Move(move[0], move[1], (move[0].x, move[0].y), self.g_map.parts[move[1][1]][move[1][0]],
                        move[0].last_pos)
            if issubclass(type(move.movable), Pacman.Pacman):
                self.mocked_score += self.g_map.parts[move.pos[1]][move.pos[0]].on_step(move.movable)[1]
            self.accept_move(move.movable, move.pos, True, True)
            a.append(move)
        self.moves.append(a)

    def revert_move(self):
        m = self.moves.pop()
        for move in m:
            if issubclass(type(move.movable), Pacman.Pacman):
                self.mocked_score -= move.map_part.on_step(move.movable)[1]
                if issubclass(type(move.map_part), Cherry):
                    self.apply_scatter(False)

            self.g_map.parts[move.pos[1]][move.pos[0]] = move.map_part
            self.accept_move(move.movable, move.orig_pos, move.last_pos, True, True)

    def accept_move(self, movable: Movable, pos, last_move=None, revert=False, mocking=False):

        val = self.g_map.parts[pos[1]][pos[0]].on_step(movable)[1]
        if issubclass(type(self.g_map.parts[pos[1]][pos[0]]), Cherry) and issubclass(type(movable),
                                                                                     Pacman.Pacman) and not revert:
            self.apply_scatter(True)
        if mocking and issubclass(type(movable), Pacman.Pacman):
            movable.update_mocking(pos[0], pos[1], last_move)
        elif revert:
            movable.update(pos[0], pos[1])
        else:
            if issubclass(type(self.g_map.parts[pos[1]][pos[0]]), Cherry) and issubclass(type(movable),
                                                                                         Pacman.Pacman):
                self.scatter_left = 100

            if self.scatter() == -1 and self.scatter_left == 0 and issubclass(type(movable), Pacman.Pacman):
                if not revert:
                    self.apply_scatter(False)

            elif self.scatter_left > 0 and issubclass(type(movable), Pacman.Pacman):
                self.scatter_left -= 1
            self.score += val
            if val > 0:
                self.collected += 1

            self.eat_ghost(movable, *pos)
            movable.update(pos[0], pos[1])
            self.check_pacmans()

            if self.collected == self.g_map.bonuses_cnt:
                print('Pacman won!')
                self.is_over = True

    def is_walkable(self, game_map, coors):
        res = False, 0
        try:
            res = game_map.parts[coors[1]][coors[0]].on_step()
        except IndexError:
            return False, 0
        finally:
            return res

    def get_free_neighbouring(self, game_map, start):
        ret = list()
        for i in range(-1, 2):
            for x in range(-1, 2):
                w = self.is_walkable(game_map, (start[0] + i, start[1] + x))
                if not (i == 0 and x == 0) and abs(i) + abs(x) < 2 and w[0]:
                    ret.append((start[0] + i, start[1] + x))
        return ret

    def get_legal_actions(self, movable: Movable):
        pass

    def scatter(self):
        for g in [self.g_map.Blinky, self.g_map.Inky, self.g_map.Clyde, self.g_map.Speedy]:
            if g is not None:
                return -1 if g.scatter else 1
        return 1

    def check_pacmans(self):
        for g in [self.g_map.Blinky, self.g_map.Inky, self.g_map.Clyde, self.g_map.Speedy]:
            if g is not None:
                if (abs(g.x - self.g_map.pacman[0].x) + abs(g.y - self.g_map.pacman[0].y)) == 0:
                    if len(self.g_map.pacman) == 1:
                        print('Ghosts won!')
                        self.is_over = True
                    else:
                        self.g_map.pacman = [self.g_map.pacman[1]]
                if len(self.g_map.pacman) > 1 and (
                        abs(g.x - self.g_map.pacman[1].x) + abs(g.y - self.g_map.pacman[1].y)) == 0:
                    self.g_map.pacman = [self.g_map.pacman[0]]

    def end_state(self):
        if self.collected == self.g_map.bonuses_cnt:
            return True, 1
        for g in [self.g_map.Blinky, self.g_map.Inky, self.g_map.Clyde, self.g_map.Speedy]:
            if g is not None:
                if (abs(g.x - self.g_map.pacman[0].x) + abs(g.y - self.g_map.pacman[0].y)) == 0:
                    return True, -1
                if len(self.g_map.pacman) > 1 and (
                        abs(g.x - self.g_map.pacman[1].x) + abs(g.y - self.g_map.pacman[1].y)) == 0:
                    return True, -1
        return False, 0

    def eat_ghost(self, movable, x, y):
        part = self.g_map.parts[y][x]
        if issubclass(type(part), Ghost) and part.scatter:
            part.update(*part.initial_pos)
            print('Step on gost')
        if issubclass(type(movable), Ghost) and issubclass(type(part), Pacman.Pacman) and movable.scatter:
            part.update(*movable.initial_pos)
            print('Step on gost')

    def get_configuration(self):
        return hash(self.g_map)


class GameRules:
    def __init__(self, game):
        self.game = game

    def get_walkable_neighbouring(self, x, y):
        ret = list()
        for a in range(-1, 2):
            for b in range(-1, 2):
                if not (a == 0 and b == 0):
                    w = self.is_walkable(self.game.g_map, (x + a, y + b), self.game.g_map.parts[y][x])
                    if abs(a) + abs(b) < 2 and w[0]:
                        ret.append((x + a, y + b))
        return ret

    @staticmethod
    def is_walkable(game_map: GameMap, coors, movable):
        res = False, 0
        try:
            res = game_map.parts[coors[1]][coors[0]].on_step(movable)
        except IndexError:
            return False, 0
        finally:
            return res
