package temperatura;

import java.io.IOException;
import java.io.ObjectOutputStream;
import java.io.PipedOutputStream;
import java.util.concurrent.atomic.AtomicBoolean;

public class GeneraDati implements Runnable{
    private PipedOutputStream pos = null;
    private AtomicBoolean isRunning = new AtomicBoolean(false);
    
    public GeneraDati(PipedOutputStream pos) {
        this.pos = pos;
    }
    
    public void run() {
        isRunning.set(true);
        ObjectOutputStream oos = null;
        try {
            oos = new ObjectOutputStream(pos);
        } catch (IOException e) {
            e.printStackTrace();
        }
        while (isRunning.get()){
            float temperatura = (float) (Math.random() * (30.0) - 5.0);
            int umidità = (int) (Math.random() * (60.0) + 15.0);
            Misure misure = new Misure(temperatura, umidità);

            try {
                oos.writeObject(misure);
            } catch (IOException e) {
                e.printStackTrace();
            }

            try {
                Thread.sleep(2500);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public void terminaGeneraDati(){
        isRunning.set(false);
    }
    
}
