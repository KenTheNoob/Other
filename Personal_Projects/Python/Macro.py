#simple macro
from pynput.keyboard import Key, KeyCode, Controller as KeyController, Listener as KeyListener
from pynput.mouse import Button, Controller as MouseController
import time
keyboard = KeyController()
mouse = MouseController()
def on_press(key):
    if key == KeyCode.from_char('-'):
        mouse.click(Button.left)
        time.sleep(0.5)
        keyboard.press('r')
        keyboard.release('r')
        time.sleep(0.05)
        keyboard.press('r')
        keyboard.release('r')
    if key == KeyCode.from_char(';'):
        mouse.click(Button.left)
    if key == Key.esc:
        print("Ending macro")
        return False
def on_release(key):
    pass

# Collect events until released
key_listener = KeyListener(on_press=on_press, on_release=on_release)
key_listener.start()
key_listener.join()