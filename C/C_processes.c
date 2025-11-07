// pc_proc_clean.c
// gcc pc_proc_clean.c -o pc_proc_clean -pthread -lrt
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#define N 8

typedef struct { int buffer[N], in, out; } shm_t;

// ---- Recursos globales para poder limpiarlos en el handler ----
static int      fd = -1;
static shm_t*   shm = NULL;
static sem_t   *empty = NULL, *full = NULL, *mutex = NULL;
static pid_t    parent_pid;

// Nombres POSIX 
static const char *SHM_NAME  = "/c_buf";
static const char *SEM_EMPTY = "/c_empty";
static const char *SEM_FULL  = "/c_full";
static const char *SEM_MUTEX = "/c_mutex";

// Cerrar/Desmapear (ambos procesos)
static void close_resources(void) {
    if (empty) { sem_close(empty); empty = NULL; }
    if (full)  { sem_close(full);  full  = NULL; }
    if (mutex) { sem_close(mutex); mutex = NULL; }
    if (shm)   { munmap(shm, sizeof(shm_t)); shm = NULL; }
    if (fd>=0) { close(fd); fd = -1; }
}

// Unlink (solo una vez: idealmente el padre)
static void unlink_resources(void) {
    sem_unlink(SEM_EMPTY);
    sem_unlink(SEM_FULL);
    sem_unlink(SEM_MUTEX);
    shm_unlink(SHM_NAME);
}

// Handler de señales: cierra en ambos; el padre también hace unlink
static void on_signal(int sig) {
    (void)sig;
    pid_t me = getpid();
    close_resources();
    if (me == parent_pid) {
        unlink_resources();
    }
    _exit(0);
}

int main(void){
  parent_pid = getpid();
  signal(SIGINT,  on_signal);  // Ctrl+C
  signal(SIGTERM, on_signal);

  // --- SHM ---
  fd = shm_open(SHM_NAME, O_CREAT|O_RDWR, 0600);
  ftruncate(fd, sizeof(shm_t));
  shm = mmap(NULL, sizeof(shm_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  shm->in = shm->out = 0;

  // --- Semáforos con nombre ---
  empty = sem_open(SEM_EMPTY, O_CREAT, 0600, N);
  full  = sem_open(SEM_FULL,  O_CREAT, 0600, 0);
  mutex = sem_open(SEM_MUTEX, O_CREAT, 0600, 1);

  pid_t pid = fork();
  if (pid==0) { // child: consumer
    for(;;){
      sem_wait(full); sem_wait(mutex);
        int v = shm->buffer[shm->out]; shm->out = (shm->out+1)%N;
        printf("consumed %d\n", v);
      sem_post(mutex); sem_post(empty);
      usleep(150000);
    }
  } else { // parent: producer
    for (int x=1;;++x){
      sem_wait(empty); sem_wait(mutex);
        shm->buffer[shm->in]=x; shm->in=(shm->in+1)%N;
        printf("produced %d\n", x); // opcional si quieres ver el lado productor
      sem_post(mutex); sem_post(full);
      usleep(100000);
    }
  }

  // (No se llega aquí en el demo; la limpieza real la hace el handler)
  close_resources();
  if (getpid() == parent_pid) unlink_resources();
  return 0;
}
