# pc_processes.py
# Ejecutar: python3 pc_processes.py
from multiprocessing import Process, Queue
import time

def producer(q: Queue):
    x = 1
    while True:
        q.put(x)            # bloquea si la cola está llena
        # print(f"put {x}") # opcional
        x += 1
        time.sleep(0.10)  

def consumer(q: Queue):
    while True:
        v = q.get()         # bloquea si la cola está vacía
        print(f"consumed {v}")
        time.sleep(0.15) 

if __name__ == "__main__":
    try:
        q = Queue(maxsize=8)
        p_prod = Process(target=producer, args=(q,), daemon=True)
        p_cons = Process(target=consumer, args=(q,), daemon=True)
        p_prod.start(); p_cons.start()

        # Mantener vivo el proceso principal; Ctrl+C para salir
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        pass  # procesos daemon terminan cuando sale el principal
