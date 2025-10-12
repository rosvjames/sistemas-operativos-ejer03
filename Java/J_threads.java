import java.util.concurrent.Semaphore;

public class J_threads {
  static final int N = 8;
  static final int[] buffer = new int[N];
  static int in = 0, out = 0;

  static final Semaphore empty = new Semaphore(N);
  static final Semaphore full  = new Semaphore(0);
  static final Semaphore mutex = new Semaphore(1);

  static class Producer extends Thread {
    public void run() {
      int x = 1;
      while (true) {
        try {
          empty.acquire();
          mutex.acquire();
          buffer[in] = x;
          in = (in + 1) % N;
          mutex.release();
          full.release();

          System.out.println("produced " + x);
          x++;
          Thread.sleep(100); 
        } catch (InterruptedException e) { return; }
      }
    }
  }

  static class Consumer extends Thread {
    public void run() {
      while (true) {
        try {
          full.acquire();
          mutex.acquire();
          int v = buffer[out];
          out = (out + 1) % N;
          mutex.release();
          empty.release();

          System.out.println("consumed " + v);
          Thread.sleep(150); // opcional
        } catch (InterruptedException e) { return; }
      }
    }
  }

  public static void main(String[] args) throws Exception {
    Thread p = new Producer();
    Thread c = new Consumer();
    p.start(); c.start();
    p.join();  c.join(); // demo infinito: Ctrl+C para terminar
  }
}
