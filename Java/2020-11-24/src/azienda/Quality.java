package azienda;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.PipedInputStream;
import java.util.concurrent.atomic.AtomicBoolean;

public class Quality extends Thread{
    private PipedInputStream pis = null;
    private Prodotti prodotti = null;
    private AtomicBoolean isRunning = new AtomicBoolean(false);

    public Quality(PipedInputStream pis, Prodotti prodotti) {
        this.pis = pis;
        this.prodotti = prodotti;
    }

    public void run(){
        isRunning.set(true);
        ObjectInputStream ois = null;
        try {
            ois = new ObjectInputStream(pis);
        } catch (IOException e) {
            e.printStackTrace();
        }

        while (isRunning.get()){
            try {
                prodotti = (Prodotti) ois.readObject();
            } catch (ClassNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }

            if (prodotti.getQualita() < 0)
                System.out.println("Prodotto: " + prodotti.getPezziProdotti() + " - Errore (" + prodotti.getQualita() + ")");
            else
                System.out.println("Prodotto: " + prodotti.getPezziProdotti() + " - OK (" + prodotti.getQualita() + ")");
        }
    }

    public void terminaQuality(){
        isRunning.set(false);
    }
}
