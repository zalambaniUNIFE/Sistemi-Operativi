import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;

public class App {
    public static void main(String[] args) {
        PipedOutputStream pos = new PipedOutputStream();
        try {
            PipedInputStream pis = new PipedInputStream(pos);
            BufferedReader br = new BufferedReader(new InputStreamReader(pis));
            RilevatoreNotizie rn = new RilevatoreNotizie(pos);
            rn.start();
            int count = 0;
            for (int i=0; i<8; i++){
                int condivisioni = Integer.parseInt(br.readLine());
                String categoria = br.readLine();
                if (condivisioni < 20){
                    System.out.println(categoria + " - Bufala (" + condivisioni + " condivisioni)");
                    count++;
                } else if (condivisioni <= 300){
                    System.out.println(categoria + " - Attendibile (" + condivisioni + " condivisioni)");
                    count = 0;
                } else {
                    System.out.println(categoria + " - Tendenza (" + condivisioni + " condivisioni)");
                    count = 0;
                }
                if (count == 3){
                    System.out.println("ATTENZIONE");
                }
            }
            rn.fermaRilevatoreNotizie();
            br.close();
            rn.join();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("Terminato");
    }
}
