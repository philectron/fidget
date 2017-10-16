"""
@project_name: Robot Collection - Project 02: Fidget

@file_name: fidget.py

@description: Controls the robot by combining this with Arduino code to form
an I2C interface. In the I2C connection, this is the master.

@compatibility: Raspberry Pi 3 Model B

@author: Phi Luu
@location: Corvallis, Oregon, United States
@created: July 13, 2017
@updated: October 14, 2017
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
    data = i2c.read_byte_data(SLAVE_ADDRESS, 1)

    return data


def print_instruction():
    """
    Prints the instructions to control the robot.

    Returns nothing.
    """
    print('Program is running.\n')
    print('Press W to move forward.')
    print('Press A to turn left.')
    print('Press S to move backward.')
    print('Press D to turn right.')
    print('Press Q to rotate camera to the left.')
    print('Press E to rotate camera to the right.\n')


"""main"""
try:
    print_instruction()
    # camera.start_preview()
    # a list of valid keys for the robot
    VALID_KEYS = 'wasdqeWASDQE'
    # convert keyboard input to a number
    KEY_TO_INT = {'w': 1, 'a': 2, 's': 3, 'd': 4, 'q': 5, 'e': 6}
    # convert number to a feedback from slave
    INT_TO_KEY = [
        'STOPPED.', 'MOVING FORWARD.', 'TURNING LEFT.',
        'MOVING BACKWARD.', 'TURNING RIGHT.'
    ]

    while True:
        # init and get input from keyboard
        user_input = get_keyboard_input()

        # convert from user's input (string) to an integer
        if (user_input in VALID_KEYS) and (len(user_input) == 1):
            user_input = user_input.lower()
            input_int = KEY_TO_INT[user_input]
        else:
            input_int = 0

        # write the corresponding integer to the slave
        i2c_write(input_int)
        print('%d has been written to slave.' % (input_int))

        # cool down
        time.sleep(1)

        # read status from the slave
        print('Has just read from slave: ' + INT_TO_KEY[i2c_read()])
except KeyboardInterrupt:
    # camera.stop_preview()
    print('\nProgram has been terminated.')
