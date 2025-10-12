// ConsumerClient.java
// Compilar: javac ConsumerClient.java
// Ejecutar: java ConsumerClient [puerto]
// Por defecto 5555
import java.io.*;
import java.net.*;

public class ConsumerClient {
  public static void main(String[] args) throws Exception {
    int port = (args.length > 0) ? Integer.parseInt(args[0]) : 5555;
    try (Socket s = new Socket("127.0.0.1", port);
         BufferedReader in = new BufferedReader(new InputStreamReader(s.getInputStream()));
         PrintWriter out = new PrintWriter(s.getOutputStream(), true)) {

      while (true) {
        out.println("TAKE");
        String val = in.readLine();
        System.out.println("consumed " + val);
        Thread.sleep(150); // opcional
      }
    }
  }
}
