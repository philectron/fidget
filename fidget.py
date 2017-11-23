"""
@file     fidget.py
@author   Phi Luu
@date     July 13, 2017

@brief    Robot Collection - Project 02: Fidget

@section  DESCRIPTION

This robot is a  keyboard-controlled robot that is capable of running on
wheels, recording videos, and avoiding obstacles.

This program is the "master" end of the I2C connection between a Raspberry
Pi and an Arduino UNO. It connects the robot with the computer through
Wi-Fi and transmits keyboard inputs to the Arduino through I2C connection.
"""

import sys
import termios
import time
import tty

import smbus

# from picamera import PiCamera

SLAVE_ADDRESS = 0x04

i2c = smbus.SMBus(1)

# camera = PiCamera()


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


def i2c_write(value):
    """
    Writes data to slave in the I2C connection.

    Args:
        value: an integer.

    Returns:
        -1 if fail to write.
         Otherwise, returns nothing.
    """
    i2c.write_byte(SLAVE_ADDRESS, value)

    return -1


def i2c_read():
    """
    Reads data from slave in the I2C connection.

    Returns:
        An integer read from the slave.
    """
    data = i2c.read_byte(SLAVE_ADDRESS)

    return data


def print_instruction():
    """
    Prints the instructions to control the robot.

    Returns nothing.
    """
    print('Program is running.\n')
    print('W = move forward')
    print('S = move backward')
    print('A = steer left')
    print('D = steer right')
    print('Q = turn camera left')
    print('E = turn camera right')
    print('X = exit\n')


"""main"""
try:
    print_instruction()
    # camera.start_preview()
    # map available keys, each to a unique number
    KEYMAP = {' ': 0, 'w': 1, 'a': 2, 's': 3, 'd': 4,
              'z': 5, 'c': 6, 'q': 7, 'e': 8}

    while True:
        # get input from keyboard
        keypress = get_keyboard_input().lower()[0]

        # raise KeyboardInterrupt if press x
        if keypress == 'x':
            raise KeyboardInterrupt

        # if the key pressed is in the list of keys
        if keypress in KEYMAP.keys():
            # map the key to a number
            to_slave = KEYMAP[keypress]
            # send the number to the slave
            i2c_write(to_slave)
            print('Sent to slave: %d' % (to_slave))

        # read status from slave
        from_slave = i2c_read()

        if from_slave in KEYMAP.values():
            print('Received from slave: %d' % (from_slave))

except KeyboardInterrupt:
    # camera.stop_preview()
    print('\nProgram is terminated.')
