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
        SimulaVento simulaVento = new SimulaVento(pos);
        Thread tSV = new Thread(simulaVento);
        MonitoraVento monitoraVento = new MonitoraVento(pis);
        Thread tMV = new Thread(monitoraVento);
        
        tSV.start();
        tMV.start();

        try {
            Thread.sleep(45000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        simulaVento.terminaSimulaVento();
        monitoraVento.terminaMonitoraVento();
        try {
            tSV.join();
            tMV.join();
        } catch (InterruptedException e) {}
        System.out.println("Terminato");
    }
}
