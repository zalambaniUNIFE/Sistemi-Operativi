import java.io.IOException;
import java.io.PipedOutputStream;
import java.nio.charset.Charset;
import java.util.concurrent.atomic.AtomicBoolean;

public class SimulaVento implements Runnable {
    private PipedOutputStream pos = null;
    private AtomicBoolean isRunning = new AtomicBoolean(false);

    public SimulaVento(PipedOutputStream pos) {
        this.pos = pos;
    }

    @Override
    public void run() {
        isRunning.set(true);
        String velocita = null;
        while(isRunning.get()){
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            int valore = (int)(Math.random() * 101.0);
            velocita = Integer.toString(valore);
            byte[] flusso_bytes = velocita.getBytes(Charset.forName("UTF-8"));
            try {
                pos.write(flusso_bytes,0,flusso_bytes.length);
                pos.flush();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public void terminaSimulaVento(){
        isRunning.set(false);
    }
    
}
