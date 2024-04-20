import java.io.Serializable;

public class Temporale implements Serializable {

    private float valore;
    private String giorno;

    public Temporale(float valore, String giorno) {
        this.valore = valore;
        this.giorno = giorno;
    }

    public float getValore(){
        return valore;
    }

    public String getGiorno(){
        return giorno;
    }
}
