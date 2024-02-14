# cd path
# python Board_Game.py
game_mode = ""
time = 0
moves = 0
turn_time = 0
blue_time = 0
red_time = 0
blue_moves = 0
red_moves = 0
turns_taken = 0
occupied = 0
turns = 0
new_piece = ""
while game_mode != "time" and game_mode != "turns":
    game_mode = str(input("Choose a game mode(time/turns): "))
    if game_mode != "time" and game_mode != "turns":
        print("Not a valid answer")
if game_mode == "time":
    while time == 0:
        try:
            time = int(input("How much time?: "))
        except ValueError:
            print("Not a valid answer")
        if time == 0:
            print("Not a valid answer")
    while turn_time == 0:
        try:
            turn_time = int(input("How much time per turn?: "))
        except ValueError:
            print("Not a valid answer")
        if turn_time == 0:
            print("Not a valid answer")
elif game_mode == "turns":
    while turns == 0:
        try:
            turns = int(input("How many turns?: "))
        except ValueError:
            print("Not a valid answer")
        if turns == 0:
            print("Not a valid answer")
import pygame
import os
from random import randint
pygame.init()
pygame.display.set_caption("Board Game")
pygame.mouse.set_visible(True)
win = pygame.display.set_mode((840,680))
font = pygame.font.SysFont(None, 47)
red_turn = False
square_x = 40
square_y = 30
click = 0
images = {}
moves = 0
start_row = 0
start_col = 0
distance_moveable = 0
distance_attackable = 0
treasure = 0
piece_selected = ""
new_piece = ""
pieces = ["a1b","a1r","a2b","a2r","d1b","d1r","d2b","d2r","r1b","r1r","r2b","r2r","t1b","t1r","t2b","t2r","Background","P1","P2","asteroid","portal","treasure","border"]
for piece in pieces:
    #C:\\Users\\21kch\\Desktop\\VS Code\\Python\\
    images[piece] = pygame.image.load(r'' + os.getcwd() + '\\img\\' + piece + '.png')
def draw_board(win):
    board_x = 100
    board_y = 100
    for x in range(17):
        pygame.draw.line(win, (255, 255, 255), (board_x+square_x*x,100), (board_x+square_x*x, 580), 1)
    for y in range(17):
        pygame.draw.line(win, (255, 255, 255), (100, board_y+square_y*y), (740, board_y+square_y*y), 1)
def movable(piece_selected, start_row, start_col, mouse_row, mouse_col, board, distance_moveable):
    distance = abs(mouse_row-start_row)+abs(mouse_col-start_col)
    if distance != 0:
        if board[mouse_row][mouse_col] == "---" or board[mouse_row][mouse_col] == "portal" or board[mouse_row][mouse_col] == "treasure":
            if piece_selected=="a1r" or piece_selected =="a2r" or piece_selected =="a1b" or piece_selected =="a2b":
                distance_moveable=2
            elif piece_selected == "d1r" or piece_selected =="d2r" or piece_selected =="d1b" or piece_selected =="d2b" or piece_selected =="r1r" or piece_selected =="r2r" or piece_selected =="r1b" or piece_selected =="r2b":
                distance_moveable=1
            elif piece_selected == "t1r" or piece_selected =="t1b" or piece_selected == "t2r" or piece_selected == "t2b":
                distance_moveable=4
        if distance <= distance_moveable:
            return True
    else:
        return False
def attackable(piece_selected, start_row, start_col, mouse_row, mouse_col, board, distance_attackable):
    distance = abs(mouse_row-start_row)+abs(mouse_col-start_col)
    if distance != 0 and board[mouse_row][mouse_col] != "---" and board[mouse_row][mouse_col] != "portal" and board[mouse_row][mouse_col] != "asteroid" and board[mouse_row][mouse_col] != "treasure":
        if piece_selected=="a1r" or piece_selected =="a2r" or piece_selected =="a1b" or piece_selected =="a2b":
            distance_attackable=1
        elif piece_selected == "d1r" or piece_selected =="d2r" or piece_selected =="d1b" or piece_selected =="d2b":
            distance_attackable=1
        elif piece_selected == "r1r" or piece_selected =="r1b":
            distance_attackable=3
        elif piece_selected == "r2r" or piece_selected == "r2b":
            distance_attackable=4
        elif piece_selected == "t1r" or piece_selected =="t1b":
            distance_attackable=0
        elif piece_selected == "t2r" or piece_selected == "t2b":
            distance_attackable=1
        if distance <= distance_attackable:
            return True
    else:
        return False
    return False
def red_selected(piece_selected):
    if piece_selected == "a1r" or piece_selected == "a2r" or piece_selected == "d1r" or piece_selected == "d2r" or piece_selected == "r1r" or piece_selected == "r2r" or piece_selected == "t1r" or piece_selected == "t2r":
        return True
    else:
        return False
def declare_winner(red_pieces, blue_pieces):
            if red_pieces > blue_pieces:
                win.fill((0, 0, 0))
                text = font.render("Red Wins!", True, (255,0,0))
                win.blit(text,(360,340))
                pygame.display.update()
                pygame.time.delay(5000)
            elif red_pieces < blue_pieces:
                win.fill((0, 0, 0))
                text = font.render("Blue Wins!", True, (0,0,255))
                win.blit(text,(360,340))
                pygame.display.update()
                pygame.time.delay(5000)
            else:
                win.fill((0, 0, 0))
                text = font.render("It's a tie!", True, (0,255,0))
                win.blit(text,(360,340))
                pygame.display.update()
                pygame.time.delay(5000)
board = [
    ["r1r","r1r","r1r","r1r","r1r","r1r","r1r","r1r","r1r","r1r","r1r","r1r","r1r","r1r","r1r","r1r"],
    ["a1r","a1r","a1r","a1r","a1r","a1r","a1r","a1r","a1r","a1r","a1r","a1r","a1r","a1r","a1r","a1r"],
    ["d1r","d1r","d1r","d1r","d1r","d1r","d1r","d1r","d1r","d1r","d1r","d1r","d1r","d1r","d1r","d1r"],
    ["t1r","t1r","t1r","t1r","t1r","t1r","t1r","t1r","t1r","t1r","t1r","t1r","t1r","t1r","t1r","t1r"],
    ["---","---","---","---","---","---","---","---","---","---","---","---","---","---","---","---"],
    ["---","---","---","---","---","---","---","---","---","---","---","---","---","---","---","---"],
    ["---","---","---","---","---","---","---","---","---","---","---","---","---","---","---","---"],
    ["---","---","---","---","---","---","---","---","---","---","---","---","---","---","---","---"],
    ["---","---","---","---","---","---","---","---","---","---","---","---","---","---","---","---"],
    ["---","---","---","---","---","---","---","---","---","---","---","---","---","---","---","---"],
    ["---","---","---","---","---","---","---","---","---","---","---","---","---","---","---","---"],
    ["---","---","---","---","---","---","---","---","---","---","---","---","---","---","---","---"],
    ["t1b","t1b","t1b","t1b","t1b","t1b","t1b","t1b","t1b","t1b","t1b","t1b","t1b","t1b","t1b","t1b"],
    ["d1b","d1b","d1b","d1b","d1b","d1b","d1b","d1b","d1b","d1b","d1b","d1b","d1b","d1b","d1b","d1b"],
    ["a1b","a1b","a1b","a1b","a1b","a1b","a1b","a1b","a1b","a1b","a1b","a1b","a1b","a1b","a1b","a1b"],
    ["r1b","r1b","r1b","r1b","r1b","r1b","r1b","r1b","r1b","r1b","r1b","r1b","r1b","r1b","r1b","r1b"]]
for x in range(15):
    board[randint(4,11)][randint(0,15)]="asteroid"
for x in range(1):
    board[randint(7,8)][randint(0,15)]="treasure"
board[5][randint(0,15)]="portal"
board[10][randint(0,15)]="portal"
run = True
while run:
    pygame.time.delay(10)
    win.fill((0, 0, 0))
    win.blit(pygame.transform.scale(images["Background"], (640, 480)), (100, 100))
    draw_board(win)
    win.blit(pygame.transform.scale(images["border"], (641, 481)), (100, 100))
    mouse_pos = pygame.mouse.get_pos()
    if click == 2:
        for r in range(16):
            for c in range(16):
                if movable(piece_selected, start_row, start_col, r, c, board, distance_moveable)==True:
                    pygame.draw.rect(win, (0,255,0), ((c)*square_x+100, (r)*square_y+100, 40, 30))
        for r in range(16):
            for c in range(16):
                if attackable(piece_selected, start_row, start_col, r, c, board, distance_attackable)==True:
                    pygame.draw.rect(win, (255,0,0), ((c)*square_x+100, (r)*square_y+100, 40, 30))
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False
        elif event.type ==pygame.MOUSEBUTTONDOWN:
            mouse_col = ((mouse_pos[0]-100)//square_x)
            mouse_row = ((mouse_pos[1]-100)//square_y)
            if -1<mouse_row<16 and -1<mouse_col<16:
                if click==0:
                    piece_selected = board[mouse_row][mouse_col]
                    if piece_selected != "---" and piece_selected != "asteroid" and piece_selected != "treasure" and piece_selected !="portal":
                        if red_turn==red_selected(piece_selected):
                            click = 1
                if click==1:
                    piece_selected = board[mouse_row][mouse_col]
                    if red_turn==red_selected(piece_selected):
                        if piece_selected != "---" and piece_selected != "asteroid" and piece_selected != "treasure" and piece_selected !="portal":
                            start_row = mouse_row
                            start_col = mouse_col
                            click = 2
                else:
                    if attackable(piece_selected, start_row, start_col, mouse_row, mouse_col, board, distance_attackable)==True:
                        if board[mouse_row][mouse_col] != "---":
                            if piece_selected == "t2r" or piece_selected == "t2b":
                                if board[mouse_row][mouse_col] == "a1r":
                                    board[mouse_row][mouse_col] = "a2r"
                                elif board[mouse_row][mouse_col] == "a1b":
                                    board[mouse_row][mouse_col] = "a2b"
                                elif board[mouse_row][mouse_col] == "d1r":
                                    board[mouse_row][mouse_col] = "d2r"
                                elif board[mouse_row][mouse_col] == "d1b":
                                    board[mouse_row][mouse_col] = "d2b"
                                elif board[mouse_row][mouse_col] == "r1r":
                                    board[mouse_row][mouse_col] = "r2r"
                                elif board[mouse_row][mouse_col] == "r1b":
                                    board[mouse_row][mouse_col] = "r2b"
                                elif board[mouse_row][mouse_col] == "t1r":
                                    board[mouse_row][mouse_col] = "t2r"
                                elif board[mouse_row][mouse_col] == "t1b":
                                    board[mouse_row][mouse_col] = "t2b"
                                moves+=1
                                if piece_selected == "t2r":
                                    board[start_row][start_col] = "t1r"
                                elif piece_selected == "t2b":
                                    board[start_row][start_col] = "t1b"
                            if board[mouse_row][mouse_col] != "d1r" and board[mouse_row][mouse_col] != "d2r" and board[mouse_row][mouse_col] != "d1b" and board[mouse_row][mouse_col] != "d2b":
                                if piece_selected != "t2r" and piece_selected != "t2b":
                                    board[mouse_row][mouse_col] = "---"
                                    moves += 1
                            elif piece_selected == "a1r" or piece_selected == "a1b":
                                if board[mouse_row][mouse_col] != "d2r" and board[mouse_row][mouse_col] != "d2b":
                                    board[mouse_row][mouse_col] = "---"
                                    moves += 1
                            elif piece_selected == "a2r" or piece_selected == "a2b":
                                board[mouse_row][mouse_col] = "---"
                                moves += 1
                    elif movable(piece_selected, start_row, start_col, mouse_row, mouse_col, board, distance_moveable)==True:
                        if board[mouse_row][mouse_col] == "---":
                            board[mouse_row][mouse_col] = piece_selected
                            board[start_row][start_col] = "---"
                            moves += 1
                        elif board[mouse_row][mouse_col] == "portal":
                            for r in range(16):
                                for c in range(16):
                                    if board[r][c] == "portal" and r != mouse_row:
                                        if red_turn ==True:
                                            board[r+1][c] = piece_selected
                                        else:
                                            board[r-1][c] = piece_selected
                                        board[start_row][start_col] = "---"
                                        moves += 1
                        elif board[mouse_row][mouse_col] == "treasure":
                            if piece_selected == "t1b" or piece_selected == "t1r":
                                treasure = 0
                            else:
                                treasure = 1
                            if treasure == 0:
                                print("Upgrade")
                                if piece_selected == "t1r" or piece_selected == "t1b":
                                    if piece_selected == "t1r":
                                        piece_selected = "t2r"
                                    elif piece_selected == "t1b":
                                        piece_selected = "t2b"
                                    board[mouse_row][mouse_col] = piece_selected
                                    board[start_row][start_col] = "---"
                                moves += 1
                            else:
                                if piece_selected == "t2r" or piece_selected == "t2b":
                                    new_piece = ""
                                    if red_turn == True:
                                        while new_piece != "a1r" and new_piece != "r1r" and new_piece != "t1r" and new_piece != "d1r":
                                            new_piece = input("Select a new piece: ")
                                            if new_piece != "a1r" and new_piece != "r1r" and new_piece != "t1r" and new_piece != "d1r":
                                                print("Invalid piece")
                                            else:
                                                board[start_row][start_col] = new_piece
                                                board[mouse_row][mouse_col] = piece_selected
                                                moves += 1
                                    else:
                                        while new_piece != "a1b" and new_piece != "r1b" and new_piece != "t1b" and new_piece != "d1b":
                                            new_piece = input("Select a new piece: ")
                                            if new_piece != "a1b" and new_piece != "r1b" and new_piece != "t1b" and new_piece != "d1b":
                                                print("Invalid piece")
                                            else:
                                                board[start_row][start_col] = new_piece
                                                board[mouse_row][mouse_col] = piece_selected
                                elif piece_selected == "a1r":
                                    board[start_row][start_col] = "---"
                                    board[mouse_row][mouse_col] = "a2r"
                                elif piece_selected == "a1b":
                                    board[start_row][start_col] = "---"
                                    board[mouse_row][mouse_col] = "a2b"
                                elif piece_selected == "d1r":
                                    board[start_row][start_col] = "---"
                                    board[mouse_row][mouse_col] = "d2r"
                                elif piece_selected == "d1b":
                                    board[start_row][start_col] = "---"
                                    board[mouse_row][mouse_col] = "d2b"
                                elif piece_selected == "r1r":
                                    board[start_row][start_col] = "---"
                                    board[mouse_row][mouse_col] = "r2r"
                                elif piece_selected == "r1b":
                                    board[start_row][start_col] = "---"
                                    board[mouse_row][mouse_col] = "r2b"
                                elif piece_selected == "t1r":
                                    board[start_row][start_col] = "---"
                                    board[mouse_row][mouse_col] = "t2r"
                                elif piece_selected == "t1b":
                                    board[start_row][start_col] = "---"
                                    board[mouse_row][mouse_col] = "t2b"
                                else:
                                    board[start_row][start_col] = "---"
                                    board[mouse_row][mouse_col] = piece_selected
                                moves += 1
                    else:
                        click = 1
                        piece_selected = "---"
                    click = 1
                    piece_selected = ""
    if moves == 3:
        red_turn = not red_turn
        moves = 0
        turns_taken += 1
        turns -= 1
        if game_mode == "time":
            red_time = 0
            blue_time = 0
    if turns_taken == 3:
        spawned = False
        occupied = 0
        for r in range(7,9):
            for c in range(16):
                if board[r][c] != "---":
                    occupied += 1
        if occupied < 32:
            while spawned == False:
                r = randint(7,8)
                c = randint(0,15)
                if board[r][c] == "---":
                    board[r][c] = "treasure"
                    spawned = True
        turns_taken = 0
    red_pieces = 0
    blue_pieces = 0
    for r in range(16):
        for c in range(16):
            piece = board[r][c]
            if piece != "---":
                win.blit(images[piece], (c*40+100, r*30+100))
            if piece == "a1r" or piece == "a2r" or piece == "d1r" or piece == "d2r" or piece == "r1r" or piece == "r2r" or piece == "t1r" or piece == "t2r":
                red_pieces += 1
            if piece == "a1b" or piece == "a2b" or piece == "d1b" or piece == "d2b" or piece == "r1b" or piece == "r2b" or piece == "t1b" or piece == "t2b":
                blue_pieces += 1
    if game_mode == "turns":
        text = font.render("Turns: " + str(round(turns,1)), True, (0,255,0))
        win.blit(text,(300,24))
        if turns == 0:
            declare_winner(red_pieces, blue_pieces)
            run = False
    if game_mode == "time":
        if red_turn == False:
            if round(blue_time,1) == round(turn_time,1):
                red_turn = True
                blue_time = 0
                piece_selected = ""
                moves = 0
                turns_taken +=1
        else:
            if round(red_time,1) == round(turn_time,1):
                red_turn = False
                red_time = 0
                piece_selected = ""
                moves = 0
                turns_taken += 1
        if time>=0:
            time -= 0.02
        else:
            time = 0
        if time <= 0:
            declare_winner(red_pieces, blue_pieces)
            run = False
        text = font.render("Time: " + str(round(time,1)), True, (0,255,0))
        win.blit(text,(300,24))
    if red_pieces == 0 or blue_pieces == 0:
        declare_winner(red_pieces, blue_pieces)
    text = font.render("A  B  C  D  E  F  G  H", True, (255,255,255))
    win.blit(text,(107,585))
    text = font.render("I", True, (255,255,255))
    win.blit(text,(437,585))
    text = font.render("J  K  L  M  N  O  P", True, (255,255,255))
    win.blit(text,(470,585))
    text = font.render("Blue Time: " + str(round(blue_time,1)), True, (0,0,255))
    win.blit(text,(560,640))
    text = font.render("Red Time: " + str(round(red_time,1)), True, (255,0,0))
    win.blit(text,(560,24))
    text = font.render("Blue pieces: " + str(blue_pieces), True, (0,0,255))
    win.blit(text,(40,640))
    text = font.render("Red pieces: " + str(red_pieces), True, (255,0,0))
    win.blit(text,(40,24))
    for x in range(1,10):
        text = font.render(str(x), True, (255,255,255))
        win.blit(text,(80,70+30*x))
    for x in range(10,17):
        text = font.render(str(x), True, (255,255,255))
        win.blit(text,(60,70+30*x))
    if red_turn == True:
        win.blit(images["P2"], (mouse_pos[0]-12, mouse_pos[1]-12))
        red_time += 0.02
    else:
        win.blit(images["P1"], (mouse_pos[0]-12, mouse_pos[1]-12))
        blue_time += 0.02
    pygame.display.update()
pygame.quit()
