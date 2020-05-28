from abc import abstractmethod

#from Core import Movable
from Core.Constants import *


class MapPart:
    def __init__(self, x, y, visual):
        self.x = x
        self.y = y
        self.visual = visual
        self.initial_pos = (x, y)

    @abstractmethod
    def on_step(self, movable):
        return False, 0

    def __str__(self):
        return self.visual


class Space(MapPart):
    def __init__(self, x, y, visual):
        super().__init__(x, y, visual)

    def on_step(self, movable):
        return True, 0


class Obstacle(MapPart):
    def __init__(self, x, y, visual):
        super().__init__(x, y, visual)

    def on_step(self, movable):
        return False, 0


class Bonus(MapPart):
    def __init__(self, x, y, visual, score):
        super().__init__(x, y, visual)
        self.score = score

    def on_step(self, movable):
        from Pacman import Pacman
        if issubclass(type(movable), Pacman.Pacman):

            return True, self.score
        return True, 0


class Coin(Bonus):
    def __init__(self, x, y, visual):
        super().__init__(x, y, visual, 1)

    def on_step(self, movable):
        return super().on_step(movable)


class Cherry(Bonus):
    def __init__(self,  x, y, visual):
        super().__init__(x, y, visual, 5)

    def on_step(self, movable):
        # print('Cherry eaten')
        return super().on_step(movable)
