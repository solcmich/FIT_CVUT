from Core.Movable import Movable
from Enviroment.MapPart import Space
from Game import Game
from Pacman.Pacman import Pacman
import copy


class Ghost(Movable):
    def __init__(self, x, y, visual):
        super().__init__(x, y, visual)
        self.scatter = False

    def on_step(self, movable):
        if issubclass(type(movable), Pacman):
            if self.scatter:
                return True, 100
            movable.die()
            return True, 0
        else:
            return False, 0

    def update(self, x, y):
        if self.scatter:
            self.visual = 'X'
        else:
            self.visual = self.origin_visual
        self.game_map.set_at_pos(self.x, self.y, self.origin)
        if not issubclass(type(self.game_map.get_at_pos(x, y)), Pacman):
            self.origin = self.game_map.get_at_pos(x, y)
        else:
            self.origin = Space(x, y, ' ')
        self.game_map.set_at_pos(x, y, self)
        self.x = x
        self.y = y


class Blinky(Ghost):
    def __init__(self, x, y, visual):
        super().__init__(x, y, visual)

    def on_step(self, movable: Movable):
        return super().on_step(movable)


class Inky(Ghost):
    def __init__(self, x, y, visual):
        super().__init__(x, y, visual)

    def on_step(self,  movable: Movable):
        return super().on_step(movable)


class Clyde(Ghost):
    def __init__(self, x, y, visual):
        super().__init__(x, y, visual)

    def on_step(self,  movable: Movable):
        return super().on_step(movable)


class Speedy(Ghost):
    def __init__(self, x, y, visual):
        super().__init__(x, y, visual)

    def on_step(self,  movable: Movable):
        return super().on_step(movable)


