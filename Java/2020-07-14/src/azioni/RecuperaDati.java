package azioni;

import java.util.concurrent.atomic.AtomicBoolean;

public class RecuperaDati extends Thread {
    private DatiAzionari datiAzionari = null;
    private AtomicBoolean isRunning = new AtomicBoolean(false);

    public RecuperaDati(DatiAzionari datiAzionari) {
        this.datiAzionari = datiAzionari;
    }

    public void run(){
        isRunning.set(true);
        String indice = datiAzionari.getIndice();
        while (isRunning.get()){
            String postIndice = datiAzionari.getIndice();
            if (!indice.equals(postIndice)){
                System.out.println("warning - indice cambiato in " + postIndice);
                indice = postIndice;
            }
            int valore = (int)(Math.random() * (60.0) + 20.0);
            datiAzionari.setValore(valore);
            
            System.out.println("RecuperaDati -- " + indice + ": " + valore);

            try {
                Thread.sleep(5000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public void terminaRecuperaDati(){
        isRunning.set(false);
        Thread.currentThread().interrupt();
    }
}
