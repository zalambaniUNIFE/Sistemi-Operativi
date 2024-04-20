package azienda;

import java.io.IOException;
import java.io.ObjectOutputStream;
import java.io.PipedOutputStream;
import java.util.concurrent.atomic.AtomicBoolean;

public class Machine extends Thread{
    private PipedOutputStream pos = null;
    private AtomicBoolean isRunning = new AtomicBoolean(false);
    private int prodottiCreati = 0;

    public Machine(PipedOutputStream pos) {
        this.pos = pos;
    }

    public void run(){
        isRunning.set(true);
        ObjectOutputStream oos = null;
        try {
            oos = new ObjectOutputStream(pos);
        } catch (IOException e) {
            e.printStackTrace();
            System.exit(-1);
        }

        while(isRunning.get()){
            float qly = (float)(Math.random() * (2000) - 1000);
            prodottiCreati++;
            Prodotti prodotti = new Prodotti(prodottiCreati, qly);
            try {
                oos.writeObject(prodotti);
            } catch (IOException e) {
                e.printStackTrace();
            }
            try {
                Thread.sleep(750);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public void terminaMachine(){
        isRunning.set(false);
    }
}
