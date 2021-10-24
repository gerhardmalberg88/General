import logging
import threading
import time

from threading import Thread, Lock

countCustomers = 0

def Barber():
    while true:
        lock.acquire()
        if customers_waiting > 0:
            customers_waiting = customers_waiting -1
            lock.release()
            cut_hair(duration=4.0)
        else:
            lock.release()
            sleep()

def Customer_enters():
    lock.acquire()
    if customers_waiting == 3:
        lock.release()
        leave()
    else:
        enter(waiting_room)
        customers_waiting = customers_waiting + 1
        lock.release()
        barber.wakeup()

def main():
    customers=[]
    INPUT_DEFAULT = 110110

    for i in INPUT_DEFAULT:
        customers.push(i)

    for i in INPUT:
        customers.push(i)

    Barber.start_thread()
    while customers > 0:
        c = customers.pop()
        if c == 1:
            Customer_enters()
        sleep(duration=1.0)

# A) How many customers the barber manages to serve in the following cases?

# B) Take the above entered digits of your student number and add 100 to it (e.g., 321 + 100 = 421). 
# Create INPUT by converting this number to 16-bit binary representation. 
# Each bit from INPUT is pushed to the stack ’customers’ starting from the most significant bit. 
# E.g. 011 translates to push(0), push(1), push(1). Run the program now with your input appended after the default one.