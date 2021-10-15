def Barber()
    while true:
        lock.acquire()
        if customers_waiting > 0
            customers_waiting--
            lock.release()
            cut_hair(duration=4.0)
        else
            lock.release()
            sleep()

def Customer_enters()
    lock.acquire()
    if customers_waiting == 3:
        lock.release()
        leave()
    else:
        enter(waiting_room)
        customers_waiting++
        lock.release()
        barber.wakeup()

def main()
    customers=[]
    INPUT_DEFAULT = 110110

    for i in INPUT_DEFAULT
        customers.push(i)

    for i in INPUT
        customers.push(i)

    customers_waiting = 0
    Barber.start_thread()
    while customers > 0:
        c = customers.pop()
            if c==1:
                Customer_enters()
        sleep(duration=1.0)