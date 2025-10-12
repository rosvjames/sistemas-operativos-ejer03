// ProducerClient.java
// Compilar: javac ProducerClient.java
// Ejecutar: java ProducerClient [puerto]
// Por defecto 5555
import java.io.*;
import java.net.*;

public class ProducerClient {
  public static void main(String[] args) throws Exception {
    int port = (args.length > 0) ? Integer.parseInt(args[0]) : 5555;
    try (Socket s = new Socket("127.0.0.1", port);
         BufferedReader in = new BufferedReader(new InputStreamReader(s.getInputStream()));
         PrintWriter out = new PrintWriter(s.getOutputStream(), true)) {

      int x = 1;
      while (true) {
        out.println("PUT " + x);
        String resp = in.readLine(); // "OK"
        // System.out.println("put " + x + " -> " + resp);
        x++;
        Thread.sleep(100); // opcional
      }
    }
  }
}
