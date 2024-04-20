package temperatura;

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
            System.exit(-1);
        }
        GeneraDati gd = new GeneraDati(pos);
        Thread threadgd = new Thread(gd);
        threadgd.start();
        float precedenteUmidità = 0.0F;
        float attualeUmidità = 0.0F;
        boolean first = true;
        ObjectInputStream ois = null;
        try {
            ois = new ObjectInputStream(pis);
        } catch (IOException e) {
            e.printStackTrace();
        }

        while(true){
            Misure misure = null;
            try {
                misure = (Misure)ois.readObject();
            } catch (ClassNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
            attualeUmidità = misure.getUmidità();
            if (!first){
                float variazione = (float) Math.abs(attualeUmidità - precedenteUmidità) / precedenteUmidità;
                System.out.println("Variazione umidita': " + variazione * 100 + "% (prec: " +  (int)precedenteUmidità + ", att: " + (int)attualeUmidità + ")");
                if (variazione > 0.2){
                    System.out.println("Variazione maggiore di 20%, termino");
                    gd.terminaGeneraDati();
                    try {
                        threadgd.join();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    break;
                }
            }
            else
                first = false;
            precedenteUmidità = attualeUmidità;
        }
    }
}
