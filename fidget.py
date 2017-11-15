"""
@file     fidget.py
@author   Phi Luu
@date     July 13, 2017

@brief    Robot Collection - Project 02: Fidget

@section  DESCRIPTION

This robot is a computer keyboard controlled robot that is capable of running on
wheels, recording videos, and avoiding obstacles.
"""

import sys
import termios
import time
import tty

from picamera import PiCamera


def get_keyboard_input():
    """
    Determines which key has been pressed by the user on the keyboard by
    accessing the system files.

    Returns:
        The pressed key as a string.

    Source from Chris Mason at
    https://www.instructables.com/id/Controlling-a-Raspberry-Pi-RC-Car-With-a-Keyboard/
    """
    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)

    try:
        tty.setraw(sys.stdin.fileno())
        ch = sys.stdin.read(1)
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)

    return ch


"""main"""
try:
    # print instructions
    print('Program started.\n')
    print('W: move forward')
    print('S: move backward')
    print('A: steer left')
    print('D: steer right')
    print('Q: turn camera left')
    print('E: turn camera right\n')

    # turn off all motors
    # TODO

    # start the camera
    camera = PiCamera()
    camera.start_preview()

    while True:
        # get keyboard input
        keypress = get_keyboard_input().lower()[0]

        # move forward if w is pressed
        if keypress == 'w':
            # TODO

        # move backward if s is pressed
        if keypress == 's':
            # TODO

        # steer left if a is pressed
        if keypress == 'a':
            # TODO

        # steer right if d is pressed
        if keypress == 'd':
            # TODO

        # turn camera to the left if q is pressed
        if keypress == 'q':
            # TODO

        # turn camera to the right if e is pressed
        if keypress == 'e':
            # TODO

        # clean up keypress and get ready for the next input
        keypress = ''

except KeyboardInterrupt:
    # clean up gpio
    # TODO

    # stop camera
    camera.stop_preview()

    # print ending message
    print('\nProgram ended.')
