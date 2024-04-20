package sicurezza;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PipedInputStream;
import java.util.concurrent.atomic.AtomicBoolean;

public class ScansionaInput implements Runnable{
    
    private PipedInputStream pis = null;
    private AtomicBoolean isRunning = new AtomicBoolean(false);

    public ScansionaInput(PipedInputStream pis) {
        this.pis = pis;
    }

    public void run(){
        isRunning.set(true);
        BufferedReader br = new BufferedReader(new InputStreamReader(pis));
        while (isRunning.get()){
            String stringa = null;
            try {
                stringa = br.readLine();
            } catch (IOException e) {
                e.printStackTrace();
            }
            System.out.println("ScansionaInput - ricevuto: " + stringa);
            if (stringa.equals("abcde") || stringa.equals("1234")) {
                System.out.println("pericolo");
            } else {
                System.out.println("ok");
            }
        }
    }

    public void terminaScansionaInput(){
        isRunning.set(false);
        Thread.currentThread().interrupt();
    }
}
