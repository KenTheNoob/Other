# cd path
# python Piano.py
import pygame
import pygame.midi
import os
import math
from random import randint
pygame.init()
pygame.midi.init()
sound = pygame.midi.Output(0)
sound.set_instrument(0)
volume = 100
#Note(60 Middle C),Volume(0-127)
#sound.note_on(60, 127)
pygame.display.set_caption("Piano")
pygame.mouse.set_visible(True)
win = pygame.display.set_mode((1280,720), pygame.RESIZABLE)
sheet = pygame.image.load(r'' + os.path.abspath("img\\cleff.png"))
note = pygame.image.load(r'' + os.path.abspath("img\\note.png"))
font = pygame.font.SysFont(None, 45)
message = ''
run = True
correct = False
text_color = (0,0,0)
# Prevent held note from being played multiple times
note_on = False
win_ratio = (1,1)
black_key = 0
press_lshift = False
press_rshift = False
gamestate = 0
random = 4
cleff = 0
prev_random = 4
prev_cleff = 0
position = 85.75
treble_states = [True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True]
base_states = [True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True]
treble = ["F1","G1","A2","B2","C2","D2","E2","F2","G2","A3","B3","C3","D3","E3","F3","G3","A4","B4","C4","D4","E4"]
base = ["A0","B0","C0","D0","E0","F0","G0","A1","B1","C1","D1","E1","F1","G1","A2","B2","C2","D2","E2","F2","G2"]
mousestate = "up"
keystate = "up"
key = ['C','2','']
keypress = ['','','']
key_sig = "C"
key_sigs = ['C','G','D','A','E','B','F#','C#','F','Bb','Eb','Ab','Db','Gb','Cb']
key_sigs_i = 0
last_key = ""
def key_sig_press():
    # Apply key signature changes
    # Flats in sharp key signatures
    if key_sig in ('C','G','D','A','E','B','F#','C#'):
        if keypress[0] == 'A' and keypress[2] == 'b':
            keypress[0] = 'G'
            keypress[2] = '#'
            keypress[1] = chr(ord(keypress[1])-1)
        if keypress[0] == 'B' and keypress[2] == 'b':
            keypress[0] = 'A'
            keypress[2] = '#'
        if keypress[0] == 'C' and keypress[2] == 'b':
            keypress[0] = 'B'
            keypress[2] = ''
        if keypress [0] == 'D' and keypress[2] == 'b':
            keypress[0] = 'C'
            keypress[2] = '#'
        if keypress[0] == 'E' and keypress[2] == 'b':
            keypress[0] = 'D'
            keypress[2] = '#'
        if keypress[0] == 'F' and keypress[2] == 'b':
            keypress[0] = 'E'
            keypress[2] = ''
        if keypress[0] == 'G' and keypress[2] == 'b':
            keypress[0] = 'F'
            keypress[2] = '#'
    # Ab one octave down
    if keypress[0] == 'A' and keypress[2] == 'b':
        keypress[1] = chr(ord(keypress[1])-1)
    # Sharps in flat key signatures
    if key_sig in ('F','Bb','Eb','Ab','Db','Gb','Cb'):
        if keypress[0] == 'A' and keypress[2] == '#':
            keypress[0] = 'B'
            keypress[2] = 'b'
        if keypress[0] == 'B' and keypress[2] == '#':
            keypress[0] = 'C'
            keypress[2] = ''
        if keypress[0] == 'C' and keypress[2] == '#':
            keypress[0] = 'D'
            keypress[2] = 'b'
        if keypress [0] == 'D' and keypress[2] == '#':
            keypress[0] = 'E'
            keypress[2] = 'b'
        if keypress[0] == 'E' and keypress[2] == '#':
            keypress[0] = 'F'
            keypress[2] = ''
        if keypress[0] == 'F' and keypress[2] == '#':
            keypress[0] = 'G'
            keypress[2] = 'b'
        if keypress[0] == 'G' and keypress[2] == '#':
            keypress[0] = 'A'
            keypress[2] = 'b'
            keypress[1] = chr(ord(keypress[1])+1)
    # Naturals in certain sharp key signatures
    if keypress[0] == 'F' and keypress[2] == '' and key_sig in ('F#','C#'):
        keypress[0] = 'E'
        keypress[2] = '#'
    if keypress[0] == 'C' and keypress[2] == '' and key_sig == 'C#':
        keypress[0] = 'B'
        keypress[2] = '#'
    # Naturals in certain flat key signatures
    if keypress[0] == 'B' and keypress[2] == '' and key_sig in ('Gb','Cb'):
        keypress[0] = 'C'
        keypress[2] = 'b'
    if keypress[0] == 'E' and keypress[2] == '' and key_sig == 'Cb':
        keypress[0] = 'F'
        keypress[2] = 'b'
def key_sig_change(char):
    key[2] = ''
    if key_sig == "G":
        if char == 'F':
            key[2] = '#'
    if key_sig == "D":
        if char in ('F','C'):
            key[2] = '#'
    if key_sig == "A":
        if char in ('F','C','G'):
            key[2] = '#'
    if key_sig == "E":
        if char in ('F','C','G','D'):
            key[2] = '#'
    if key_sig == "B":
        if char in ('F','C','G','D','A'):
            key[2] = '#'
    if key_sig == "F#":
        if char in ('F','C','G','D','A','E'):
            key[2] = '#'
    if key_sig == "C#":
        if char in ('F','C','G','D','A','E','B'):
            key[2] = '#'
    if key_sig == "Cb":
        if char in ('F','C','G','D','A','E','B'):
            key[2] = 'b'
    if key_sig == "Gb":
        if char in ('C','G','D','A','E','B'):
            key[2] = 'b'
    if key_sig == "Db":
        if char in ('G','D','A','E','B'):
            key[2] = 'b'
    if key_sig == "Ab":
        if char in ('D','A','E','B'):
            key[2] = 'b'
    if key_sig == "Eb":
        if char in ('A','E','B'):
            key[2] = 'b'
    if key_sig == "Bb":
        if char in ('E','B'):
            key[2] = 'b'
    if key_sig == "F":
        if char == 'B':
            key[2] = 'b'
while run:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False
        elif event.type == pygame.VIDEORESIZE or event.type == pygame.VIDEOEXPOSE:
            new_size = pygame.display.get_surface().get_size()
            win_ratio = (new_size[0]/1280, new_size[1]/720)
            font = pygame.font.SysFont(None, int(45*min(win_ratio[0],win_ratio[1])))
            text = font.render('C', True, (0,0,0))
        if event.type == pygame.MOUSEBUTTONDOWN:
            mouse_pos = pygame.mouse.get_pos()
            mousestate = "down"
            #print(mouse_pos)
            line = False
            # Toggle notes create hitbox
            for i in range(21):
                if math.sqrt(pow(abs((1240+line*20)*win_ratio[0]-mouse_pos[0]),2)+pow(abs((163-i*7.7)*win_ratio[1]-mouse_pos[1]),2))<=5*min(win_ratio[0],win_ratio[1]):
                    treble_states[i] = not treble_states[i]
                line = not line
            line = False
            for i in range(21):
                if math.sqrt(pow(abs((1240+line*20)*win_ratio[0]-mouse_pos[0]),2)+pow(abs((337-i*7.7)*win_ratio[1]-mouse_pos[1]),2))<=5*min(win_ratio[0],win_ratio[1]):
                    base_states[i] = not base_states[i]
                line = not line
            # Toggle key signature
            if math.sqrt(pow(abs(90*win_ratio[0]-mouse_pos[0]),2)+pow(abs(173*win_ratio[1]-mouse_pos[1]),2))<=5*min(win_ratio[0],win_ratio[1]):
                key_sigs_i += 1
                if key_sigs_i == 15:
                    key_sigs_i = 0
                key_sig = key_sigs[key_sigs_i]
                key_sig_change(key[0])
            # Click black note
            black_key = 0
            if mouse_pos[1]>400*win_ratio[1] and mouse_pos[1]<600*win_ratio[1]:
                black_key = 1
                if mouse_pos[0]%(273*win_ratio[0])>32*win_ratio[0] and mouse_pos[0]%(273*win_ratio[0])<56*win_ratio[0]:
                    keypress = ['A',str(int(mouse_pos[0]/(273*win_ratio[0]))),'#']
                elif mouse_pos[0]%(273*win_ratio[0])>100*win_ratio[0] and mouse_pos[0]%(273*win_ratio[0])<124*win_ratio[0]:
                    keypress = ['C',str(int(mouse_pos[0]/(273*win_ratio[0]))),'#']
                elif mouse_pos[0]%(273*win_ratio[0])>146*win_ratio[0] and mouse_pos[0]%(273*win_ratio[0])<170*win_ratio[0]:
                    keypress = ['D',str(int(mouse_pos[0]/(273*win_ratio[0]))),'#']
                elif mouse_pos[0]%(273*win_ratio[0])>214*win_ratio[0] and mouse_pos[0]%(273*win_ratio[0])<238*win_ratio[0]:
                    keypress = ['F',str(int(mouse_pos[0]/(273*win_ratio[0]))),'#']
                # Subtract 11 to account for g going past 273 pixel modulus reset
                elif (mouse_pos[0]-11*win_ratio[0])%(273*win_ratio[0])+11*win_ratio[0]>259*win_ratio[0] and mouse_pos[0]%(273*win_ratio[0])<283*win_ratio[0]:
                    keypress = ['G',str(int((mouse_pos[0]-11*win_ratio[0])/(273*win_ratio[0]))),'#']
                else:
                    black_key = 0
            # Click natural note
            if black_key == 0 and mouse_pos[1]>400 and mouse_pos[1]<700:
                keypress = [chr(int(int(mouse_pos[0]/(39*win_ratio[0]))%7)+65),str(int(int(mouse_pos[0]/(39*win_ratio[0]))/7)),'']
            # Click off keyboard
            elif black_key == 0:
                keypress = ['','','']
            # Change black note according to key signature
            key_sig_press()
            # Check note
            if keypress != ['','','']:
                if keypress == key:
                    correct = True
                    text_color = (0,255,0)
                    message = keypress[0] + keypress[1] + keypress[2] + " is correct."
                else:
                    text_color = (255,0,0)
                    message = keypress[0] + keypress[1] + keypress[2] + " is incorrect. The correct answer is " + key[0] + key[1] + key[2] +  "."
            # One note at a time
            if mouse_pos[1]>400 and mouse_pos[1]<700:
                keystate = "up"
                note_on = False
        if event.type == pygame.MOUSEBUTTONUP:
            mousestate = "up"
        # Type note
        if event.type == pygame.KEYDOWN:
            keystate = "down"
            last_key = pygame.key.name(event.key)
            if event.key == pygame.K_LSHIFT:
                press_lshift = True
                press_rshift = False
            if event.key == pygame.K_RSHIFT:
                press_rshift = True
                press_lshift = False
            if event.key == pygame.K_a:
                keypress = ['A','3','']
            elif event.key == pygame.K_b:
                keypress = ['B','3','']
            elif event.key == pygame.K_c:
                keypress = ['C','2','']
            elif event.key == pygame.K_d:
                keypress = ['D','2','']
            elif event.key == pygame.K_e:
                keypress = ['E','2','']
            elif event.key == pygame.K_f:
                keypress = ['F','2','']
            elif event.key == pygame.K_g:
                keypress = ['G','2','']
            elif event.key == pygame.K_n:
                correct = True
                keypress = key
            else:
                keypress = ['','','']
            # Apply sharp if shift pressed
            if press_lshift:
                if keypress[0] != '':
                    keypress[2] = '#'
            if press_rshift:
                if keypress[0] != '':
                    keypress[2] = 'b'
            key_sig_press()
            #print(keypress)
            # Octave does not have to be the same for keypress
            if keypress[0] == key[0] and keypress[2] == key[2]:
                correct = True
                text_color = (0,255,0)
                keypress = key
                message = keypress[0] + keypress[1] + keypress[2] + " is correct."
            else:
                if keypress[0] != '':
                    text_color = (255,0,0)
                    if keypress[2] == '#':
                        message = keypress[0] + keypress[2] + " is incorrect. The correct answer is " + key[0] + key[2] + "."
                    else:
                        message = keypress[0] + keypress[2] + " is incorrect. The correct answer is " + key[0] + key[2] + "."
                else:
                    message = ''
            # One note at a time
            mousestate = "up"
            note_on = False
            # Sound
            if event.unicode == "+":
                volume += 10
            if event.key == pygame.K_MINUS:
                volume -= 10
            if volume < 0:
                volume = 0
            if volume > 100:
                volume = 100
        if event.type == pygame.KEYUP:
            # If a key other than the last one presssed is released, note should not change
            if pygame.key.name(event.key) == last_key:
                keystate = "up"
            if pygame.key.name(event.key) == 'left shift':
                press_lshift = False
            if pygame.key.name(event.key) == 'right shift':
                press_rshift = False
    # Clear screen
    win.fill((0,0,0))
    # Choose a new note
    if correct == True and (mousestate == "up" or keystate == "up"):
        # Check if there are enough states for a new random note
        total_states = 0
        for i in range(21):
            total_states += treble_states[i]
            total_states += base_states[i]
        # While it is the same note and there are more than 1 note to choose from or the note is toggled off
        while(total_states > 0 and ((random == prev_random and cleff == prev_cleff and total_states > 1) or (cleff == 0 and treble_states[random] == 0) or (cleff == 1 and base_states[random] == 0))):
            random = randint(0,20)
            cleff = randint(0,1)
        prev_random = random
        prev_cleff = cleff
        if cleff == 0:
            position = 116.75-random*7.75
            key = [treble[random][0],treble[random][1],'']
            key_sig_change(treble[random][0])
        if cleff == 1:
            position = 290.75-random*7.75
            key = [base[random][0],base[random][1],'']
            key_sig_change(base[random][0])
        #print("New key is: " + key[0] + key[1] + key[2])
        correct = False
    win.blit(pygame.transform.scale(sheet, (1280*win_ratio[0], 350*win_ratio[1])), (0, 0))
    # Key signature signs
    sharp_symbol = font.render('#',True, (0,0,0))
    sharp_symbol = pygame.transform.flip(sharp_symbol, True, False)
    sharp_symbol = pygame.transform.rotate(sharp_symbol, 90)
    sharp_symbol = pygame.transform.scale(sharp_symbol, (20*win_ratio[0],30*win_ratio[1]))
    flat_symbol = font.render('b',True, (0,0,0))
    if key_sig in ('G','D','A','E','B','F#','C#'):
        win.blit(sharp_symbol,(110*win_ratio[0],40*win_ratio[1]))
        if key_sig in ('D','A','E','B','F#','C#'):
            win.blit(sharp_symbol,(125*win_ratio[0],64*win_ratio[1]))
            if key_sig in ('A','E','B','F#','C#'):
                win.blit(sharp_symbol,(140*win_ratio[0],34*win_ratio[1]))
                if key_sig in ('E','B','F#','C#'):
                    win.blit(sharp_symbol,(155*win_ratio[0],57*win_ratio[1]))
                    if key_sig in ('B','F#','C#'):
                        win.blit(sharp_symbol,(170*win_ratio[0],80*win_ratio[1]))
                        if key_sig in ('F#','C#'):
                            win.blit(sharp_symbol,(185*win_ratio[0],49*win_ratio[1]))
                            if key_sig == 'C#':
                                win.blit(sharp_symbol,(200*win_ratio[0],73*win_ratio[1]))
    if key_sig in ('F','Bb','Eb','Ab','Db','Gb','Cb'):
        win.blit(flat_symbol,(110*win_ratio[0],73*win_ratio[1]))
        if key_sig in ('Bb','Eb','Ab','Db','Gb','Cb'):
            win.blit(flat_symbol,(125*win_ratio[0],49*win_ratio[1]))
            if key_sig in ('Eb','Ab','Db','Gb','Cb'):
                win.blit(flat_symbol,(140*win_ratio[0],80*win_ratio[1]))
                if key_sig in ('Ab','Db','Gb','Cb'):
                    win.blit(flat_symbol,(155*win_ratio[0],57*win_ratio[1]))
                    if key_sig in ('Db','Gb','Cb'):
                        win.blit(flat_symbol,(170*win_ratio[0],87*win_ratio[1]))
                        if key_sig in ('Gb','Cb'):
                            win.blit(flat_symbol,(185*win_ratio[0],64*win_ratio[1]))
                            if key_sig == 'Cb':
                                win.blit(flat_symbol,(200*win_ratio[0],94*win_ratio[1]))
    # Ledger lines
    if cleff == 0:
        if key[0] + key[1] in ("F1","G1","A2","B2","C2"):
            pygame.draw.line(win, (0,0,0), (500*win_ratio[0],132*win_ratio[1]), (540*win_ratio[0],132*win_ratio[1]))
            if key[0] + key[1] in ("F1","G1","A2"):
                pygame.draw.line(win, (0,0,0), (500*win_ratio[0],147*win_ratio[1]), (540*win_ratio[0],147*win_ratio[1]))
                if key[0] + key[1] == "F1":
                    pygame.draw.line(win, (0,0,0), (500*win_ratio[0],163*win_ratio[1]), (540*win_ratio[0],163*win_ratio[1]))
        if key[0] + key[1] in ("A4","B4","C4","D4","E4"):
            pygame.draw.line(win, (0,0,0), (500*win_ratio[0],40*win_ratio[1]), (540*win_ratio[0],40*win_ratio[1]))
            if key[0] + key[1] in ("C4","D4","E4"):
                pygame.draw.line(win, (0,0,0), (500*win_ratio[0],24*win_ratio[1]), (540*win_ratio[0],24*win_ratio[1]))
                if key[0] + key[1] == "E4":
                    pygame.draw.line(win, (0,0,0), (500*win_ratio[0],9*win_ratio[1]), (540*win_ratio[0],9*win_ratio[1]))
    if cleff == 1:
        if key[0] + key[1] in ("A0","B0","C0","D0","E0"):
            pygame.draw.line(win, (0,0,0), (500*win_ratio[0],306*win_ratio[1]), (540*win_ratio[0],306*win_ratio[1]))
            if key[0] + key[1] in ("A0","B0","C0"):
                pygame.draw.line(win, (0,0,0), (500*win_ratio[0],321*win_ratio[1]), (540*win_ratio[0],321*win_ratio[1]))
                if key[0] + key[1] == "A0":
                    pygame.draw.line(win, (0,0,0), (500*win_ratio[0],337*win_ratio[1]), (540*win_ratio[0],337*win_ratio[1]))
        if key[0] + key[1] in ("C2","D2","E2","F2","G2"):
            pygame.draw.line(win, (0,0,0), (500*win_ratio[0],214*win_ratio[1]), (540*win_ratio[0],214*win_ratio[1]))
            if key[0] + key[1] in ("E2","F2","G2"):
                pygame.draw.line(win, (0,0,0), (500*win_ratio[0],199*win_ratio[1]), (540*win_ratio[0],199*win_ratio[1]))
                if key[0] + key[1] == "G2":
                    pygame.draw.line(win, (0,0,0), (500*win_ratio[0],183*win_ratio[1]), (540*win_ratio[0],183*win_ratio[1]))
    # Toggle note draw circles
    line = False
    for i in range(21):
        if treble_states[i] == 0:
            color = 0
        else:
            color = 255
        pygame.draw.circle(win, (0,color,0), ((1240+line*20)*win_ratio[0],(163-i*7.7)*win_ratio[1]), 5*min(win_ratio[0],win_ratio[1]))
        line = not line
    line = False
    for i in range(21):
        if base_states[i] == 0:
            color = 0
        else:
            color = 255
        pygame.draw.circle(win, (0,color,0), ((1240+line*20)*win_ratio[0],(337-i*7.7)*win_ratio[1]), 5*min(win_ratio[0],win_ratio[1]))
        line = not line
    pygame.draw.line(win, (0,0,0), (1232*win_ratio[0],173*win_ratio[1]), (1280*win_ratio[0],173*win_ratio[1]))
    win.blit(pygame.transform.scale(note, (40*win_ratio[0], 60*win_ratio[1])), (500*win_ratio[0], position*win_ratio[1]))
    # Toggle key signature
    pygame.draw.circle(win, (0,9,255), (90*win_ratio[0],173*win_ratio[1]), 5*min(win_ratio[0],win_ratio[1]))
    # Draw natural notes
    for i in range(33):
        pygame.draw.rect(win,(255,255,255),(i*39*win_ratio[0],400*win_ratio[1],39*win_ratio[0]-1,300*win_ratio[1]))
    # Pressed down natural notes
    if (mousestate == "down" and keypress != ['','','']) or (keystate == "down" and keypress != ['','','']):
        if keypress[2] == '':
            if keypress[0] == 'A':
                pygame.draw.rect(win,(127,127,127),((int(keypress[1])*273)*win_ratio[0],400*win_ratio[1],39*win_ratio[0]-1,300*win_ratio[1]))
                if not note_on:
                    sound.note_on(33+int(keypress[1])*12, volume)
                    note_on = True
            elif keypress[0] == 'B':
                pygame.draw.rect(win,(127,127,127),((int(keypress[1])*273+39)*win_ratio[0],400*win_ratio[1],39*win_ratio[0]-1,300*win_ratio[1]))
                if not note_on:
                    sound.note_on(35+int(keypress[1])*12, volume)
                    note_on = True
            elif keypress[0] == 'C':
                pygame.draw.rect(win,(127,127,127),((int(keypress[1])*273+39*2)*win_ratio[0],400*win_ratio[1],39*win_ratio[0]-1,300*win_ratio[1]))
                if not note_on:
                    sound.note_on(36+int(keypress[1])*12, volume)
                    note_on = True
            elif keypress[0] == 'D':
                pygame.draw.rect(win,(127,127,127),((int(keypress[1])*273+39*3)*win_ratio[0],400*win_ratio[1],39*win_ratio[0]-1,300*win_ratio[1]))
                if not note_on:
                    sound.note_on(38+int(keypress[1])*12, volume)
                    note_on = True
            elif keypress[0] == 'E':
                pygame.draw.rect(win,(127,127,127),((int(keypress[1])*273+39*4)*win_ratio[0],400*win_ratio[1],39*win_ratio[0]-1,300*win_ratio[1]))
                if not note_on:
                    sound.note_on(40+int(keypress[1])*12, volume)
                    note_on = True
            elif keypress[0] == 'F':
                pygame.draw.rect(win,(127,127,127),((int(keypress[1])*273+39*5)*win_ratio[0],400*win_ratio[1],39*win_ratio[0]-1,300*win_ratio[1]))
                if not note_on:
                    sound.note_on(41+int(keypress[1])*12, volume)
                    note_on = True
            elif keypress[0] == 'G':
                pygame.draw.rect(win,(127,127,127),((int(keypress[1])*273+39*6)*win_ratio[0],400*win_ratio[1],39*win_ratio[0]-1,300*win_ratio[1]))
                if not note_on:
                    sound.note_on(43+int(keypress[1])*12, volume)
                    note_on = True
    # Draw B#(C), E#(F), Cb(B), Fb(E) before black notes
    if (mousestate == "down" and keypress != ['','','']) or (keystate == "down" and keypress != ['','','']):
        if keypress[2] == '#':
            if keypress[0] == 'E':
                pygame.draw.rect(win,(127,127,127),((int(keypress[1])*273+39*5)*win_ratio[0],400*win_ratio[1],39*win_ratio[0]-1,300*win_ratio[1]))
                if not note_on:
                    sound.note_on(41+int(keypress[1])*12, volume)
                    note_on = True
            if keypress[0] == 'B':
                pygame.draw.rect(win,(127,127,127),((int(keypress[1])*273+39*2)*win_ratio[0],400*win_ratio[1],39*win_ratio[0]-1,300*win_ratio[1]))
                if not note_on:
                    sound.note_on(36+int(keypress[1])*12, volume)
                    note_on = True
        if keypress[2] == 'b':
            if keypress[0] == 'C':
                pygame.draw.rect(win,(127,127,127),((int(keypress[1])*273+39)*win_ratio[0],400*win_ratio[1],39*win_ratio[0]-1,300*win_ratio[1]))
                if not note_on:
                    sound.note_on(35+int(keypress[1])*12, volume)
                    note_on = True
            if keypress[0] == 'F':
                pygame.draw.rect(win,(127,127,127),((int(keypress[1])*273+39*4)*win_ratio[0],400*win_ratio[1],39*win_ratio[0]-1,300*win_ratio[1]))
                if not note_on:
                    sound.note_on(40+int(keypress[1])*12, volume)
                    note_on = True
    # Draw sharps a,c,d then f,g(on top of naturals)
    for i in range(5):
        pygame.draw.rect(win,(0,0,0),((i*273+32)*win_ratio[0],400*win_ratio[1],24*win_ratio[0],200*win_ratio[1]))
        pygame.draw.rect(win,(0,0,0),((i*273+100)*win_ratio[0],400*win_ratio[1],24*win_ratio[0],200*win_ratio[1]))
        pygame.draw.rect(win,(0,0,0),((i*273+146)*win_ratio[0],400*win_ratio[1],24*win_ratio[0],200*win_ratio[1]))
    for i in range(4):
        pygame.draw.rect(win,(0,0,0),((i*273+214)*win_ratio[0],400*win_ratio[1],24*win_ratio[0],200*win_ratio[1]))
        pygame.draw.rect(win,(0,0,0),((i*273+259)*win_ratio[0],400*win_ratio[1],24*win_ratio[0],200*win_ratio[1]))
    # Pressed down black notes
    if (mousestate == "down" and keypress != ['','','']) or (keystate == "down" and keypress != ['','','']):
        if keypress[2] == '#':
            if keypress[0] == 'A':
                pygame.draw.rect(win,(127,127,127),((int(keypress[1])*273+32)*win_ratio[0],400*win_ratio[1],24*win_ratio[0],200*win_ratio[1]))
                if not note_on:
                    sound.note_on(34+int(keypress[1])*12, volume)
                    note_on = True
            elif keypress[0] == 'C':
                pygame.draw.rect(win,(127,127,127),((int(keypress[1])*273+100)*win_ratio[0],400*win_ratio[1],24*win_ratio[0],200*win_ratio[1]))
                if not note_on:
                    sound.note_on(37+int(keypress[1])*12, volume)
                    note_on = True
            elif keypress[0] == 'D':
                pygame.draw.rect(win,(127,127,127),((int(keypress[1])*273+146)*win_ratio[0],400*win_ratio[1],24*win_ratio[0],200*win_ratio[1]))
                if not note_on:
                    sound.note_on(39+int(keypress[1])*12, volume)
                    note_on = True
            elif keypress[0] == 'F':
                pygame.draw.rect(win,(127,127,127),((int(keypress[1])*273+214)*win_ratio[0],400*win_ratio[1],24*win_ratio[0],200*win_ratio[1]))
                if not note_on:
                    sound.note_on(42+int(keypress[1])*12, volume)
                    note_on = True
            elif keypress[0] == 'G':
                pygame.draw.rect(win,(127,127,127),((int(keypress[1])*273+259)*win_ratio[0],400*win_ratio[1],24*win_ratio[0],200*win_ratio[1]))
                if not note_on:
                    sound.note_on(44+int(keypress[1])*12, volume)
                    note_on = True
        elif keypress[2] == 'b':
            if keypress[0] == 'A':
                pygame.draw.rect(win,(127,127,127),((int(keypress[1])*273+259)*win_ratio[0],400*win_ratio[1],24*win_ratio[0],200*win_ratio[1]))
                if not note_on:
                    sound.note_on(44+int(keypress[1])*12, volume)
                    note_on = True
            if keypress[0] == 'B':
                pygame.draw.rect(win,(127,127,127),((int(keypress[1])*273+32)*win_ratio[0],400*win_ratio[1],24*win_ratio[0],200*win_ratio[1]))
                if not note_on:
                    sound.note_on(34+int(keypress[1])*12, volume)
                    note_on = True
            elif keypress[0] == 'D':
                pygame.draw.rect(win,(127,127,127),((int(keypress[1])*273+100)*win_ratio[0],400*win_ratio[1],24*win_ratio[0],200*win_ratio[1]))
                if not note_on:
                    sound.note_on(37+int(keypress[1])*12, volume)
                    note_on = True
            elif keypress[0] == 'E':
                pygame.draw.rect(win,(127,127,127),((int(keypress[1])*273+146)*win_ratio[0],400*win_ratio[1],24*win_ratio[0],200*win_ratio[1]))
                if not note_on:
                    sound.note_on(39+int(keypress[1])*12, volume)
                    note_on = True
            elif keypress[0] == 'G':
                pygame.draw.rect(win,(127,127,127),((int(keypress[1])*273+214)*win_ratio[0],400*win_ratio[1],24*win_ratio[0],200*win_ratio[1]))
                if not note_on:
                    sound.note_on(42+int(keypress[1])*12, volume)
                    note_on = True
    # Middle C
    win.blit(text,(630*win_ratio[0],650*win_ratio[1]))
    text2 = font.render(message, True, text_color)
    win.blit(text2,(10*win_ratio[0],350*win_ratio[1]))
    text3 = font.render(key_sig,True, (0,0,0))
    win.blit(text3,(110*win_ratio[0],160*win_ratio[1]))
    #pygame.time.delay(100)
    pygame.display.update()
def reset_note():
    for i in range(33,88):
        sound.note_off(i, volume)
note_on = False
del sound
pygame.midi.quit()
pygame.quit()