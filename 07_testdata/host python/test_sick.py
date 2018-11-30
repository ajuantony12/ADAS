#!/usr/bin/python
import cv2
import sys, getopt
from sick import *

print ("<<<< initing sick")

def main(argv):
    port = "com1"
    baud = "9600"
    try:
        opts, args = getopt.getopt(argv,"hc:b:",["com","baud="])
    except getopt.GetoptError:
        print ('test_sick.py -c <COM Port> -b <baud rate>')
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print ('test_sick.py -c <COM Port> -b <baud rate>')
            sys.exit()
        elif opt in ('-c', '--com'):
            port = arg
        elif opt in ('-b', '--baud'):
            baud = int(arg, 10)
    print ('test_sick.py -c ' + port + ' -b ' + str(baud))
    sick = SICK(port, baud, True)
    count = 30
#   while count > 0:
#       print (".")
#       if sick.get_frame():
#           print (sick.cartesian)
#           cv2.imshow("img", sick.image)
#           cv2.waitKey(5)
#           count = count-1

if __name__ == "__main__":
   main(sys.argv[1:])