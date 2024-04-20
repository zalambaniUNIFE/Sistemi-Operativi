package sicurezza;

import java.io.IOException;
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
        InputUtente iu = new InputUtente(pos);
        Thread tIU = new Thread(iu);
        tIU.start();
        ScansionaInput si = new ScansionaInput(pis);
        Thread tSI = new Thread(si);
        tSI.start();

        try {
            Thread.sleep(30000);
            System.out.println("Terminazione in corso...");
            iu.terminaInputUtente();
            si.terminaScansionaInput();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        try {
            tIU.join();
            tSI.join();
        } catch (InterruptedException e) {}
        System.out.println("Terminato");
        System.exit(0);
    }
}
