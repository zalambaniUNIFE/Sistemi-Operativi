package azienda;

import java.io.Serializable;

public class Prodotti implements Serializable{
    private int pezziProdotti = 0;
    private float prodotti = 0.0F;

    public Prodotti(int pezziProdotti, float prodotti) {
        this.pezziProdotti = pezziProdotti;
        this.prodotti = prodotti;
    }
    
    public int getPezziProdotti() {
        return this.pezziProdotti;
    }

    public float getQualita(){
        return prodotti;
    } 
}
