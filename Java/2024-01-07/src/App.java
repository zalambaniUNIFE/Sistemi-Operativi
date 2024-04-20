import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;

public class App {
    public static void main(String[] args) {
        PipedInputStream pis = new PipedInputStream();
        PipedOutputStream pos = null;
        try {
            pos = new PipedOutputStream(pis);
        } catch (IOException e) {
            e.printStackTrace();
        }
        GeneraTemporali gt = new GeneraTemporali(pos);
        gt.start();
        ObjectInputStream ois = null;
        try {
            ois = new ObjectInputStream(pis);
        } catch (IOException e) {
            e.printStackTrace();
        }
        Temporale t = null;
        int count = 0, c = 0;
        
        while (c < 10){
            try {
                t = (Temporale)ois.readObject();
            } catch (ClassNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
            if (t.getValore() >= 0 && t.getValore() <= 1){
                System.out.println(t.getGiorno() + " - assente (" + t.getValore() + " mm/h)");
                count = 0;
            }
            else if (t.getValore() >= 1 && t.getValore() <= 2){
                System.out.println(t.getGiorno() + " - debole (" + t.getValore() + " mm/h)");
                count = 0;
            }
            else if (t.getValore() >= 2 && t.getValore() <= 4){
                System.out.println(t.getGiorno() + " - leggera (" + t.getValore() + " mm/h)");
                count = 0;
            }
            else if (t.getValore() >= 4 && t.getValore() <= 6){
                System.out.println(t.getGiorno() + " - moderata (" + t.getValore() + " mm/h)");
                count = 0;
            }
            else if (t.getValore() > 6){
                System.out.println(t.getGiorno() + " - forte (" + t.getValore() + " mm/h)");
                count++;
            }

            if (count == 3){
                System.out.println("ATTENZIONE");
            }
            c++;
        }
        gt.terminaTemporali();
        try {
            gt.join();
        } catch (InterruptedException e) {}
        System.out.println("Terminato");
    }
}
