# Ejercicio 3 - Sistemas Operativos

Implementaciones del problema Productor-Consumidor en diferentes lenguajes y paradigmas.

## Estructura del proyecto

### C/
- `C_processes.c` - Implementación usando procesos
- `C_threads.c` - Implementación usando threads
- Ejecutables compilados incluidos

### Java/
- `J_threads.java` - Implementación usando threads
- `PCServer.java` - Servidor para comunicación cliente-servidor
- `ProducerClient.java` - Cliente productor
- `ConsumerClient.java` - Cliente consumidor

### Python/
- `pc_processes.py` - Implementación usando procesos
- `pc_threads.py` - Implementación usando threads

## Cómo ejecutar

### C
```bash
cd C/
gcc C_processes.c -o C_processes
./C_processes

gcc C_threads.c -o C_threads -lpthread
./C_threads
```

### Java
```bash
cd Java/
javac J_threads.java
java J_threads

# Para cliente-servidor:
javac *.java
java PCServer &
java ProducerClient
java ConsumerClient
```

### Python
```bash
cd Python/
python3 pc_processes.py
python3 pc_threads.py
```

## Descripción

Cada implementación demuestra diferentes enfoques para resolver el problema clásico del Productor-Consumidor:

- **Procesos**: Comunicación entre procesos separados
- **Threads**: Hilos compartiendo memoria dentro del mismo proceso
- **Cliente-Servidor**: Arquitectura distribuida con sockets

Todos los ejemplos incluyen sincronización adecuada para evitar condiciones de carrera.