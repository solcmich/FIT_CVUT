

#  Represents pacman class as a cooperative agent
from typing import List
import sys

from Core.Movable import Movable
from Ghosts import Ghost
from InterestingStuff.Game.Player import Player
import time


class Pacman(Movable):
    def __init__(self, x, y, visual):
        super().__init__(x, y, visual)
        self.death = False

    def on_step(self, movable):
        if issubclass(type(movable), Ghost.Ghost):
            self.die()
            return True, -100
        return False, 0

    def die(self):
        self.death = True

    def update(self, x, y):
        self.game_map.reset_at_pos(self.x, self.y)
        self.origin = self.game_map.get_at_pos(x, y)
        self.game_map.set_at_pos(x, y, self)
        self.last_pos = (self.x, self.y)
        self.x = x
        self.y = y

    def update_mocking(self, x, y, last_pos):
        # self.game_map.set_at_pos(self.x, self.y, self.origin)
        self.origin = self.game_map.get_at_pos(x, y)
        self.game_map.set_at_pos(x, y, self)
        self.last_pos = last_pos
        self.x = x
        self.y = y
