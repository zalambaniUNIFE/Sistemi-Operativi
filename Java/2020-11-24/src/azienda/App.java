package azienda;

import java.io.IOException;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;

public class App {
    public static void main(String[] args){
       PipedInputStream pis = new PipedInputStream();
       PipedOutputStream pos = null;
       try {
        pos = new PipedOutputStream(pis);
        } catch (IOException e) {
            e.printStackTrace();
            System.exit(-1);
        }
        
        Prodotti prodotti = new Prodotti(0, 0.0F);

        Machine machine = new Machine(pos);
        Quality quality = new Quality(pis, prodotti);
        Boolean keepGoing = true;

        machine.start();
        quality.start();
        while (keepGoing){
            try {
                Thread.sleep(15000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println("Termino i thread");
            machine.terminaMachine();
            quality.terminaQuality();
            keepGoing = false;
        }
        try {
            quality.join();
            machine.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("Termino il thread Main");
    }
}
