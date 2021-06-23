from Core import Movable
#from Enviroment.Map import MapPart, GameMap
from Enviroment.MapPart import Bonus


def is_walkable(game_map, coors, movable):
    res = False, 0
    try:
        res = game_map.parts[coors[1]][coors[0]].on_step(movable)
    except IndexError:
        return False, 0
    finally:
        return res


def get_possible_steps(game_map, start, pacman):
    ret = list()
    for i in range(-1, 2):
        for x in range(-1, 2):
            w = is_walkable(game_map, (start[0] + i, start[1] + x), pacman)
            if not (i == 0 and x == 0) and abs(i) + abs(x) < 2 and w[0]:
                ret.append(((start[0] + i, start[1] + x), w[1]))
    return ret


def backtrace(parent_map, start, end):
    path = [end]
    while path[-1] != start:
        path.append(parent_map[path[-1]])
    return len(path)


def get_closest_food_dist(starting, game_map):
    parent = {}
    visited = set()
    q = list()
    q.append(starting)
    visited.add(starting)
    iters = 0
    ret = 0
    while len(q) > 0:
        curr = q.pop(0)
        if issubclass(type(game_map.parts[curr[1]][curr[0]]), Bonus):
            ret += backtrace(parent, starting, curr)
            break
        iters += 1
        n = get_possible_steps(game_map, curr, game_map.parts[starting[1]][starting[0]])
        for i in n:
            if i[0] in visited:
                continue
            else:
                q.append(i[0])
                visited.add(i[0])
                parent[i[0]] = curr
    return ret


class GameDistance:

    def __init__(self, game_map):
        self.game_map = game_map

    def backtrace(self, parent_map, start, end):
        path = [end]
        while path[-1] != start:
            path.append(parent_map[path[-1]])
        return len(path)
        
    def get_dist_actual(self, pos1, pos2):
        parent = {}
        visited = set()
        q = list()
        q.append(pos1)
        visited.add(pos1)
        iters = 0
        ret = 0
        while len(q) > 0:
            curr = q.pop(0)
            if curr == pos2:
                ret += self.backtrace(parent, pos1, curr)
                break
            iters += 1
            n = get_possible_steps(self.game_map, curr)
            for i in n:
                if i[0] in visited:
                    continue
                else:
                    q.append(i[0])
                    visited.add(i[0])
                    parent[i[0]] = curr
        return ret


    def get_dist_manhattan(self, pos1, pos2):
        return abs(pos1[0] - pos2[0]) + abs(pos1[1] - pos2[1])

    def get_dist_euclid(self, pos1, pos2):
        return ((pos1[0] - pos2[0])**2 + (pos1[1] - pos2[1])**2)**0.5

    def get_dist(self, m1, m2, manner=get_dist_euclid):
        return manner((m1.x, m1.y), (m2.x, m2.y))








