from Game.Game import Game
from InterestingStuff.Game import Player
from InterestingStuff.Game.CantTurnPacman import CantTurnPacman
from InterestingStuff.Game.CrossroadSmartPacman import CrossroadSmartPacman
from InterestingStuff.Game.DoublePacman import DoublePacman
from InterestingStuff.Game.GhostsPlayer import GhostsPlayer
from InterestingStuff.Game.PacmanPlayer import PacmanPlayer
from InterestingStuff.Solver.MinimaxSolver import *
import time


class GameContainer:
    def __init__(self, game, pacmanPlayer, ghostPlayer, minimax_depth=3):
        self.pacman_player = pacmanPlayer
        self.ghost_player = ghostPlayer
        self.solver = MinimaxSolver(game, pacmanPlayer, ghostPlayer, minimax_depth)

    def play(self):
        for i in self.solver.steps_generator():
            print(i.g_map)
            time.sleep(.2)


print('Hello, please choose map, enter 1 for bigger map or 2 for smaller one')
m = input()
m = 'Map1' if m == '1' else 'Map2'

print('Please choose one of following configurations: ')
print('1 - Classic game: 4 ghosts vs 1 pacman using minimax with depth 4')
print('2 - 4 ghosts vs 1 Cant turn pacman using minimax with depth 4')
print('3 - 4 ghosts vs 2 pacmans using minimax with depth 3')
val = input('Enter config: ')

if val == '1':
    g = Game("data/"+m+"_Solo.txt")
    pacman = PacmanPlayer(g, g.g_map.pacman)
    ghosts = GhostsPlayer(g, g.g_map.Blinky, g.g_map.Speedy, g.g_map.Clyde, g.g_map.Inky)
    game1 = GameContainer(g, pacman, ghosts)
    game1.play()
elif val == '2':
    g = Game("data/"+m+"_Solo.txt")
    pacman = CantTurnPacman(g, g.g_map.pacman)
    ghosts = GhostsPlayer(g, g.g_map.Blinky, g.g_map.Speedy, g.g_map.Clyde, g.g_map.Inky)
    game1 = GameContainer(g, pacman, ghosts)
    game1.play()
else:
    g = Game("data/"+m+"_Duo.txt")
    pacman = DoublePacman(g, g.g_map.pacman)
    ghosts = GhostsPlayer(g, g.g_map.Blinky, g.g_map.Speedy, g.g_map.Clyde, g.g_map.Inky)
    game1 = GameContainer(g, pacman, ghosts)
    game1.play()
