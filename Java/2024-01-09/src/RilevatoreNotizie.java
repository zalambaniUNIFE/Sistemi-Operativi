import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PipedOutputStream;
import java.util.concurrent.atomic.AtomicBoolean;

public class RilevatoreNotizie extends Thread{
    private PipedOutputStream pos = null;
    private AtomicBoolean isRunning = new AtomicBoolean(false);

    public RilevatoreNotizie(PipedOutputStream pos) {
        this.pos = pos;
    }

    @Override
    public void run(){
        isRunning.set(true);
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(pos));
        String[] categoria = new String[3];
        categoria[0] = "cronaca";
        categoria[1] = "sport";
        categoria[2] = "politica";
        while (isRunning.get()){
            try {
                bw.write((int)(Math.random() * 501.0) + "");
                bw.newLine();
                bw.write(categoria[(int)(Math.random() * 3.0)]);
                bw.newLine();
                bw.flush();
            } catch (IOException e) {
                e.printStackTrace();
            }
            try {
                Thread.sleep(5000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public void fermaRilevatoreNotizie(){
        isRunning.set(false);
    }
}
