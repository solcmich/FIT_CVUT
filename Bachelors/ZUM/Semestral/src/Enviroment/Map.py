# from Enviroment.MapPart import *
from collections import namedtuple

from Core import Movable
from Enviroment.MapPart import *
from Core.Constants import SPACE_VISUAL
from Ghosts.Ghost import *
from InterestingStuff.GameUtils import GameDistance
from Pacman import *
from Pacman.Pacman import Pacman


class GameMap:
    def __init__(self, file_path):
        self.w = 0
        self.h = 0
        self.bonuses_cnt = 0
        self.pacman = list()
        self.Blinky = None
        self.Inky = None
        self.Clyde = None
        self.Speedy = None
        self.parts = self.parse_level(file_path)
        self.set_map()
        self.distance_evaluator = GameDistance(self)

    def __hash__(self):
        ret = 0
        for i in self.parts:
            for x in i:
                ret += hash(x)
        return ret

    def set_map(self):
        for p in self.pacman:
            p.set_map(self)
        if self.Blinky is not None:
            self.Blinky.set_map(self)
        if self.Speedy is not None:
            self.Speedy.set_map(self)
        if self.Clyde is not None:
            self.Clyde.set_map(self)
        if self.Inky is not None:
            self.Inky.set_map(self)

    def parse_character(self, pos, char) -> MapPart:
        h = int(self.h)
        if char == PLAYER_VISUAL:
            p = Pacman(pos, h, char)
            self.pacman.append(p)
            return p
        elif char == BLINKY_VISUAL:
            self.Blinky = Blinky(pos, h, char)
            return self.Blinky
        elif char == CLYDE_VISUAL:
            self.Clyde = Clyde(pos, h, char)
            return self.Clyde
        elif char == INKY_VISUAL:
            self.Inky = Inky(pos, h, char)
            return self.Inky
        elif char == SPEEDY_VISUAL:
            self.Speedy = Speedy(pos, h, char)
            return self.Speedy
        elif char == WALL_VISUAL:
            return Obstacle(pos, h, char)
        elif char == WALL2_VISUAL:
            return Obstacle(pos, h, char)
        elif char == COIN_VISUAL:
            self.bonuses_cnt += 1
            return Coin(pos, h, char)
        elif char == CHERRY_VISUAL:
            self.bonuses_cnt += 1
            return Cherry(pos, h, char)
        elif char == SPACE_VISUAL:
            return Space(pos, h, char)
        raise IndentationError

    def parse_line(self, line):
        self.w = len(line)
        r = list()
        for c in range(len(line)):
            r.append(self.parse_character(c, line[c]))
        return r

    def parse_level(self, file_path):
        f = open(file_path, "r")
        c = f.readlines()
        r = list()
        for i in c:
            r.append(self.parse_line(i[:-1]))
            self.h += 1
        return r

    def set_at_pos(self, x, y, part: Movable):
        self.parts[y][x] = part

    def reset_at_pos(self, x, y):
        self.parts[y][x] = Space(x, y, SPACE_VISUAL)

    def pacman_death(self):
        pass

    def get_at_pos(self, x, y):
        return self.parts[y][x]

    def __str__(self):
        ret = ""
        a = 0
        for i in self.parts:
            for x in i:
                ret += str(x)
            ret += '\n'
        return ret

