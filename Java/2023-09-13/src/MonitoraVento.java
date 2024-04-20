import java.io.IOException;
import java.io.PipedInputStream;
import java.nio.charset.Charset;
import java.util.Arrays;
import java.util.Scanner;
import java.util.concurrent.atomic.AtomicBoolean;

public class MonitoraVento implements Runnable{
    private PipedInputStream pis;
    private AtomicBoolean isRunning = new AtomicBoolean(false);

    public MonitoraVento(PipedInputStream pis) {
        this.pis = pis;
    }

    @Override
    public void run() {
        isRunning.set(true);
        int nread;
        byte lettura_bytes[] = new byte[1024];
        Scanner sc;
        while (isRunning.get()){
            try {
                nread = pis.read(lettura_bytes);
            } catch (IOException e) {
                nread = 0;
            }
            byte effettivamente_letti[]= Arrays.copyOfRange(lettura_bytes,0,nread);
            String message = new String(effettivamente_letti, Charset.forName("UTF-8"));
            sc = new Scanner(message);
            int velocita = sc.nextInt();
            if (velocita <= 10)
                System.out.println("vento leggero - " + velocita + " km/h");
            else if (velocita <= 30)
                System.out.println("vento moderato - " + velocita + " km/h");
            else if (velocita <= 50)
                System.out.println("vento forte - " + velocita + " km/h");
            else if (velocita <= 75)
                System.out.println("vento tempestoso - " + velocita + " km/h");
            else
                System.out.println("vento violento - " + velocita + " km/h");
            sc.close();
        }
    }

    public void terminaMonitoraVento(){
        isRunning.set(false);
    }
    
}
