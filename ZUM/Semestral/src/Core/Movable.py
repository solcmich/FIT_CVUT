from abc import abstractmethod

from Core.Constants import SPACE_VISUAL
from Enviroment.MapPart import MapPart, Space


class Movable(MapPart):

    @abstractmethod
    def on_step(self, movable):
        pass

    def __init__(self, x, y, visual):
        super().__init__(x, y, visual)
        self.visual = visual
        self.origin_visual = visual
        self.game_map = None
        self.origin = Space(x, y, SPACE_VISUAL)
        self.last_pos = x, y

    def set_map(self, game_map):
        self.game_map = game_map

    def update(self, x, y):
        self.game_map.set_at_pos(self.x, self.y, self.origin)
        self.origin = self.game_map.get_at_pos(x, y)
        self.game_map.set_at_pos(x, y, self)
        self.last_pos = (x, y)
        self.x = x
        self.y = y
