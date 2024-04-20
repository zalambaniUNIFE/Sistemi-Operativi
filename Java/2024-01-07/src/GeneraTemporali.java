import java.io.IOException;
import java.io.ObjectOutputStream;
import java.io.PipedOutputStream;
import java.util.concurrent.atomic.AtomicBoolean;

public class GeneraTemporali extends Thread{
    private PipedOutputStream pos = null;
    private AtomicBoolean isRunning = new AtomicBoolean(false);

    public GeneraTemporali(PipedOutputStream pos) {
        this.pos = pos;
    }

    @Override
    public void run(){
        isRunning.set(true);
        ObjectOutputStream oos = null;
        try {
            oos = new ObjectOutputStream(pos);
        } catch (IOException e) {

            e.printStackTrace();
        }
        
        String[] settimana = new String[7];
        settimana[0] = "lunedi'";
        settimana[1] = "martedi'";
        settimana[2] = "mercoledi'";
        settimana[3] = "giovedi'";
        settimana[4] = "venerdi'";
        settimana[5] = "sabato";
        settimana[6] = "domenica";
        int count = 0;
        
        while (isRunning.get()){
            Temporale temporale = new Temporale(((float)(Math.random() * 9.0)), settimana[count]);
            try {
                oos.writeObject(temporale);
                oos.flush();
            } catch (IOException e) {
                e.printStackTrace();
            }
            if (count != 6)
                count++;
            else
                count = 0;
            try {
                Thread.sleep(3000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public void terminaTemporali(){
        isRunning.set(false);
        Thread.currentThread().interrupt();
    }
}
