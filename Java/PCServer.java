// PCServer.java
// Compilar: javac PCServer.java
// Ejecutar: java PCServer [puerto]
// Por defecto puerto 5555
import java.io.*;
import java.net.*;
import java.util.concurrent.*;

public class PCServer {
  static final int N = 8;
  static final int DEFAULT_PORT = 5555;

  public static void main(String[] args) throws Exception {
    int port = (args.length > 0) ? Integer.parseInt(args[0]) : DEFAULT_PORT;

    // Buffer acotado = "recurso compartido" entre procesos via red
    final BlockingQueue<Integer> q = new ArrayBlockingQueue<>(N);

    // ServerSocket con reuseAddress para facilitar rerun inmediato
    ServerSocket server = new ServerSocket();
    server.setReuseAddress(true);
    server.bind(new InetSocketAddress("127.0.0.1", port), 50);

    // Cierre ordenado al Ctrl+C
    Runtime.getRuntime().addShutdownHook(new Thread(() -> {
      try { server.close(); } catch (IOException ignored) {}
      System.out.println("\n[server] cerrado");
    }));

    System.out.println("[server] escuchando en 127.0.0.1:" + port);

    while (true) {
      Socket s = server.accept();
      new Thread(() -> {
        try (s;
             BufferedReader in = new BufferedReader(new InputStreamReader(s.getInputStream()));
             PrintWriter out = new PrintWriter(s.getOutputStream(), true)) {

          String line;
          while ((line = in.readLine()) != null) {
            if (line.startsWith("PUT ")) {
              int val = Integer.parseInt(line.substring(4).trim());
              q.put(val);           // bloquea si lleno
              out.println("OK");
            } else if (line.equals("TAKE")) {
              int v = q.take();     // bloquea si vacío
              out.println(v);
            } else {
              out.println("ERR");
            }
          }
        } catch (Exception e) {
          // conexión cerrada / error → salir hilo
        }
      }, "client-handler").start();
    }
  }
}
