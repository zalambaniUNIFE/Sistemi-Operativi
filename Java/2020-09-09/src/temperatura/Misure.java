package temperatura;

import java.io.Serializable;

public class Misure implements Serializable{
    private float temperatura;
    private int umidità;

     public Misure(float temperatura, int umidità) {
        this.temperatura = temperatura;
        this.umidità = umidità;
    }

    public float getTemperatura(){
        return temperatura;
    }

    public int getUmidità(){
        return umidità;
    }
}
