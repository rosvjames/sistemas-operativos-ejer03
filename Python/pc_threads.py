# pc_threads.py
# Ejecutar: python3 pc_threads.py
import threading
import time

N = 8
buffer = [None] * N
in_idx = 0
out_idx = 0

empty = threading.Semaphore(N)
full  = threading.Semaphore(0)
mutex = threading.Lock()

def producer():
    global in_idx
    x = 1
    while True:
        empty.acquire()
        with mutex:
            buffer[in_idx] = x
            in_idx = (in_idx + 1) % N
        full.release()
        print(f"produced {x}")
        x += 1
        time.sleep(0.10)  

def consumer():
    global out_idx
    while True:
        full.acquire()
        with mutex:
            v = buffer[out_idx]
            out_idx = (out_idx + 1) % N
        empty.release()
        print(f"consumed {v}")
        time.sleep(0.15)  

if __name__ == "__main__":
    try:
        t1 = threading.Thread(target=producer, daemon=True)
        t2 = threading.Thread(target=consumer, daemon=True)
        t1.start(); t2.start()
        # Mantener vivo el hilo principal; Ctrl+C para salir
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        pass  # al salir, los hilos daemon terminan
