import numpy as np
import time
import sys


class color:
    PURPLE = '\033[95m'
    CYAN = '\033[96m'
    DARKCYAN = '\033[36m'
    BLUE = '\033[94m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    RED = '\033[91m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
    END = '\033[0m'


USE_PRUNING = True
USE_DEPTH_EVALUATING = False
PLAYER_1_SYMBOL = 'X'
PLAYER_2_SYMBOL = 'O'
PLAYER_1 = 1
PLAYER_2 = -1
BLANK = 0


class Game:
    def __init__(self, board_shape, mode, minimax_depth):
        self.mode = mode
        self.board = np.zeros(board_shape)
        self.turn = 1
        self.minimax_depth = minimax_depth
        self.moves = 0
        self.last_move = (-1, -1)
        self.winning_combination = set()

    def check_main_diagonal(self, starting_point):
        s = self.board[starting_point[0]][starting_point[1]]
        if starting_point[0] < len(self.board) - 4 and starting_point[1] < len(self.board) - 4:
            if self.board[starting_point[0] + 1][starting_point[1] + 1] == s and \
                    self.board[starting_point[0] + 2][starting_point[1] + 2] == s \
                    and self.board[starting_point[0] + 3][starting_point[1] + 3] == s and \
                    self.board[starting_point[0] + 4][starting_point[1] + 4] == s:
                self.winning_combination = {(starting_point[0], starting_point[1]),
                                            (starting_point[0] + 1, starting_point[1] + 1),
                                            (starting_point[0] + 2, starting_point[1] + 2),
                                            (starting_point[0] + 3, starting_point[1] + 3),
                                            (starting_point[0] + 4, starting_point[1] + 4)}
                return True, s
        return False, s

    def check_diagonal(self, starting_point):
        s = self.board[starting_point[0]][starting_point[1]]
        if starting_point[0] < len(self.board) - 4 and starting_point[1] > 3:
            if self.board[starting_point[0] + 1][starting_point[1] - 1] == s and \
                    self.board[starting_point[0] + 2][starting_point[1] - 2] == s \
                    and self.board[starting_point[0] + 3][starting_point[1] - 3] == s \
                    and self.board[starting_point[0] + 4][starting_point[1] - 4] == s:
                self.winning_combination = {(starting_point[0], starting_point[1]),
                                            (starting_point[0] + 1, starting_point[1] - 1),
                                            (starting_point[0] + 2, starting_point[1] - 2),
                                            (starting_point[0] + 3, starting_point[1] - 3),
                                            (starting_point[0] + 4, starting_point[1] - 4)}
                return True, s
        return False, s

    def check_horizontal(self, starting_point):
        s = self.board[starting_point[0]][starting_point[1]]
        if starting_point[1] < len(self.board) - 4:
            if self.board[starting_point[0]][starting_point[1] + 1] == s \
                    and self.board[starting_point[0]][starting_point[1] + 2] == s \
                    and self.board[starting_point[0]][starting_point[1] + 3] == s \
                    and self.board[starting_point[0]][starting_point[1] + 4] == s:
                self.winning_combination = {(starting_point[0], starting_point[1]),
                                            (starting_point[0], starting_point[1] + 1),
                                            (starting_point[0], starting_point[1] + 2),
                                            (starting_point[0], starting_point[1] + 3),
                                            (starting_point[0], starting_point[1] + 4)}
                return True, s
        return False, s

    def check_vertical(self, starting_point):
        s = self.board[starting_point[0]][starting_point[1]]
        if starting_point[0] < len(self.board) - 4:
            if self.board[starting_point[0] + 1][starting_point[1]] == s \
                    and self.board[starting_point[0] + 2][starting_point[1]] == s \
                    and self.board[starting_point[0] + 3][starting_point[1]] == s \
                    and self.board[starting_point[0] + 4][starting_point[1]] == s:
                self.winning_combination = {(starting_point[0], starting_point[1]),
                                            (starting_point[0] + 1, starting_point[1]),
                                            (starting_point[0] + 2, starting_point[1]),
                                            (starting_point[0] + 3, starting_point[1]),
                                            (starting_point[0] + 4, starting_point[1])}
                return True, s
        return False, s

    # Wrapper for checkers
    def check_5_in_row(self, starting_point):
        md = self.check_main_diagonal(starting_point)
        if md[0]:
            return True, md[1]
        d = self.check_diagonal(starting_point)
        if d[0]:
            return True, d[1]
        h = self.check_horizontal(starting_point)
        if h[0]:
            return True, h[1]
        v = self.check_vertical(starting_point)
        if v[0]:
            return True, v[1]
        return False, 0

    # Checks for game ending
    # Returns: Tuple(bool if it is and ending state, -1 for O win, 1 for X win, 0 for Tie)
    def end_state(self):
        cnt = 0
        for i in range(0, len(self.board)):
            for j in range(0, len(self.board[i])):
                if self.board[i][j] != 0:
                    cnt += 1
                    res = self.check_5_in_row((i, j))
                    if res[0]:
                        return True, res[1]  # 1 or -1 win
        if cnt == len(self.board) ** 2:  # Found a tie
            return True, 0
        return False, 0

    # Checks if given point is within the board bounds
    def within_board(self, point):
        if point[0] >= len(self.board):
            return False
        if point[1] >= len(self.board):
            return False
        if point[0] < 0:
            return False
        if point[1] < 0:
            return False
        return True

    # For any given point neighbouring to some filled tile calculates the weight in direction
    # Values were adjusted mainly experimentally
    def weight_in_direction(self, starting_point, direction):
        initial_point = (starting_point[0] + direction[0], starting_point[1] + direction[1])
        if not self.within_board(initial_point) or self.board[initial_point[0]][initial_point[1]] == 0:
            return 0, 0
        symbol = self.board[initial_point[0]][initial_point[1]]
        p1 = (starting_point[0] + direction[0], starting_point[1] + direction[1])
        p2 = (starting_point[0] - direction[0], starting_point[1] - direction[1])
        cnt_possible = 1
        cnt_symbol = 1
        p1_possible = True  # Obstacle encountered in direction?
        p2_possible = True  # Obstacle encountered in opposite direction?
        for _ in range(4):
            if p1_possible and self.within_board(p1) \
                    and (self.board[p1[0]][p1[1]] == symbol or self.board[p1[0]][p1[1]] == BLANK):
                cnt_possible += 1
                if self.board[p1[0]][p1[1]] == symbol:
                    cnt_symbol += cnt_symbol + 1
            else:
                if self.within_board(p1) and self.board[p1[0]][p1[1]] == -symbol:
                    cnt_symbol -= 1  # Blocked by enemy symbol
                else:
                    cnt_symbol -= 1  # Blocked by wall
                p1_possible = False

            if p2_possible and self.within_board(p2) \
                    and (self.board[p2[0]][p2[1]] == symbol or self.board[p2[0]][p2[1]] == BLANK):
                cnt_possible += 1
                if self.board[p2[0]][p2[1]] == symbol:
                    cnt_symbol += cnt_symbol + 1
            else:
                if self.within_board(p2) and self.board[p2[0]][p2[1]] == -symbol:
                    cnt_symbol -= 1  # Blocked by enemy symbol
                else:
                    cnt_symbol -= 1  # Blocked by wall
                p2_possible = False

            # Update by direction
            p1 = (p1[0] + direction[0], p1[1] + direction[1])
            p2 = (p2[0] - direction[0], p2[1] - direction[1])

            if not p1_possible and not p2_possible:
                break

        if cnt_possible > 4:
            return cnt_symbol, symbol
        else:
            return 0, symbol

    # Calculated the weight of added tile by defined rules
    def weighted_enough(self, pos, min_weight_to_consider):
        val = 0
        for x in range(-1, 2):
            for y in range(-1, 2):
                w = self.weight_in_direction((pos[0], pos[1]), (x, y))
                val += w[0]
        if val >= min_weight_to_consider:
            return True, val
        return False, val

    # Heuristics definition:
    # Find any node, that is neighbouring to any filled node
    def free_tiles(self):
        r = list()
        ret = list()
        for i in range(len(self.board)):
            for j in range(len(self.board[i])):
                if self.board[i][j] == 0:
                    a = self.weighted_enough((i, j), 2)
                    if a[0]:
                        r.append(((i, j), a[1]))

        r.sort(key=lambda x: x[1], reverse=True)  # Sort by weight in descending order
        for i in r:
            ret.append(i[0])

        # No position found by our heuristics is winnable
        # Try to find 5 blank in a row, this state is not covered by our heuristics in some cases
        if len(ret) == 0:
            for i in range(len(self.board)):
                for j in range(len(self.board[i])):
                    a = self.check_5_in_row((i, j))
                    if a[0] and a[1] == BLANK:
                        ret.append((i, j))
                        return ret
        return ret

    @staticmethod
    def dist_manhattan(p1, p2):
        x = abs(p1[0] - p2[0])
        y = abs(p2[1] - p1[1])
        return x + y

    # Evaluate configuration from lastly added, prioritizes configurations with more 'spread' = bigger sum of
    # manhattan distances
    def evaluate_configuration(self, to_iterate):
        player1_score = 0
        player2_score = 0
        board_middle = (len(self.board)//2, len(self.board)//2)
        for i in to_iterate:
            if self.board[i[0]][i[1]] == PLAYER_1:
                player1_score += self.dist_manhattan(i, board_middle)
            else:
                player2_score += self.dist_manhattan(i, board_middle)
        return player1_score - player2_score

    @staticmethod
    def end_game(prompt):
        print(prompt)
        end = time.time()
        print('{} s elapsed'.format(round(end - start, 2)))
        exit(0)

    # Simply examines all possible scenarios how things can go with respect to choosing the best one.
    def minimax(self, depth, maximizing, alpha, beta, currently_added) -> tuple:
        res = self.end_state()
        if res[0]:
            if res[1] == self.turn:
                return 10, (-1, -1)  # Winner
            elif res[1] == -self.turn:
                return -10, (-1, -1)  # Looser
            return 0, (-1, -1)  # Tie

        if depth == self.minimax_depth:
            val = 0
            if USE_DEPTH_EVALUATING:
                c = self.evaluate_configuration(currently_added)
                currently_added.clear()
                val = max(min(c, 1), -1)
                if self.turn == maximizing:  # Player_2 is always maximizing
                    val = max(0, -val)
                else:
                    val = min(0, val)
            return val, (-1, -1)  # We don't see that far

        p = (-1, -1)
        # Idea is to start searching the tree from the top
        if maximizing == self.turn:  # If we present a max player, try to pick max from its children
            max_eval = -sys.maxsize
            curr_max = max_eval
            children = self.free_tiles()  # Heuristics
            if len(children) == 0:
                self.end_game('As for {} moves forward, this can be only tie, good job, ending the game.'.format(depth))
            for c in children:
                self.board[c[0]][c[1]] = self.turn  # Fill the search tree leaf
                currently_added.append((c[0], c[1]))
                res = self.minimax(depth + 1, -maximizing, alpha, beta, currently_added)  # Recur to minimizing player
                self.winning_combination.clear()
                currently_added.clear()
                if res[0] - depth > curr_max:
                    curr_max = res[0] - depth  # Winning after less rounds is better
                    p = c
                self.board[c[0]][c[1]] = BLANK  # Revert
                alpha = max(alpha, res[0])
                if beta <= alpha and USE_PRUNING:  # Prune
                    break
            return curr_max, p
        else:
            min_eval = sys.maxsize
            curr_min = min_eval
            children = self.free_tiles()
            if len(children) == 0:
                self.end_game('As for {} moves forward, this can be only tie, good job, ending the game.'.format(depth))
            for c in children:
                self.board[c[0]][c[1]] = -self.turn  # Fill the search tree leaf
                currently_added.append((c[0], c[1]))
                res = self.minimax(depth + 1, -maximizing, alpha, beta, currently_added)  # Recur to maximizing player
                self.winning_combination.clear()
                currently_added.clear()
                if res[0] < curr_min:
                    curr_min = res[0]
                    p = c
                self.board[c[0]][c[1]] = BLANK  # Revert
                beta = min(beta, res[0])
                if beta <= alpha and USE_PRUNING:  # Prune
                    break
            return curr_min, p

    def draw(self):
        print(end="  ")
        for i in range(0, len(self.board)):
            print(i, end=" ")
        print()
        for i in range(0, len(self.board)):
            print(i, end=" ")
            for j in range(0, len(self.board[i])):
                if (i, j) == self.last_move and (i, j) not in self.winning_combination:
                    print(color.BLUE, end='')
                if (i, j) in self.winning_combination:
                    print(color.YELLOW, end='')
                if self.board[i][j] == 1:
                    print('X', end=" ")
                elif self.board[i][j] == -1:
                    print('O', end=" ")
                else:
                    print('.', end=" ")
                if j > 10:
                    print(end=' ')
                print(color.END, end='')
            print()
        print('--------------------------------------------')

    def ai_move(self, player):
        if self.moves == 0:
            x = len(self.board) // 2
            y = len(self.board) // 2
            self.board[x][y] = player
        else:
            print('Thinking...')
            start_t = time.time()
            move = self.minimax(0, player, -sys.maxsize, sys.maxsize, list())
            end = time.time()
            print('Move is {} with value {} after {} seconds'.format((move[1][1], move[1][0]), move[0],
                                                                     round(end - start_t, 2)))
            self.board[move[1][0]][move[1][1]] = player
            self.last_move = (move[1][0], move[1][1])
        self.moves += 1

    def human_move(self, player):
        while True:
            i = input('Please enter your X, Y positions separated with ",": \n')
            pos = tuple(int(x) for x in i.split(","))
            if self.board[pos[1]][pos[0]] != 0:
                print('Invalid move!')
            else:
                self.board[pos[1]][pos[0]] = player
                self.moves += 1
                self.last_move = (pos[1], pos[0])
                break

    def start(self):
        res = self.end_state()
        self.moves = 0
        while not res[0]:
            self.ai_move(1)
            res = self.end_state()
            self.draw()
            if res[0]:
                break
            self.turn = -self.turn
            if self.mode == 1:
                self.human_move(-1)
            else:
                self.ai_move(-1)
            self.turn = -self.turn
            res = self.end_state()
            self.draw()
        print('{} won!'.format(res[1]))


print('Please enter the size of a board (8-12 recommended)')
size = int(input())
print('Do you want to play (1), or just watch (2)?')
m = int(input())
m = max(min(m, 2), 1)
print('Please enter how many moves forward the algorithm thinks range from 4 to 6. 4 is recommended')
d = int(input())
d = max(min(d, 6), 4)
g = Game((size, size), m, d)
start = time.time()
g.start()
