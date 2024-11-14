import threading
import time
import random

readers_count=0
resource_lock=threading.Lock()
readers_lock=threading.Lock()
execution_time_limit=5

def reader(reader_id):
    global readers_count
    start_time=time.time()
    while time.time()-start_time < execution_time_limit:
        with readers_lock:
            readers_count+=1
            if readers_count==1:
                resource_lock.acquire()

        print(f"Reader {reader_id} is reading.")
        time.sleep(random.random())

        with readers_lock:
            readers_count-=1
            if readers_count==0:
                resource_lock.release()

def writer(writer_id):
    start_time=time.time()
    while time.time()-start_time< execution_time_limit:
        print(f"Writer {writer_id} is trying to write.")
        resource_lock.acquire()
        print(f"Writer {writer_id} is writing.")
        time.sleep(random.random())
        resource_lock.release()

readers=[threading.Thread(target=reader,args=(i,))for i in range(5)]
writers=[threading.Thread(target=writer,args=(i,))for i in range(4)]

for r in readers:
    r.start()
for w in writers:
    w.start()

for r in readers:
    r.join()
for w in writers:
    w.join()