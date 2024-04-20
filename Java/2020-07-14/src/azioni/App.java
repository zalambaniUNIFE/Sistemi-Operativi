package azioni;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class App {
    public static void main(String[] args) {
        DatiAzionari datiAzionari = new DatiAzionari();
        BufferedReader stdin = new BufferedReader(new InputStreamReader(System.in));

        System.out.println("Inserire l'indice da monitorare: ");
        String indice = null;

        try {
            indice = stdin.readLine();
        } catch (IOException e) {
            e.printStackTrace();
        }

        datiAzionari.setIndice(indice);
        RecuperaDati rd = new RecuperaDati(datiAzionari);
        rd.start();

        while(true){
            try {
                Thread.sleep(7000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            int valore = datiAzionari.getValore();
            System.out.println("Indice: " + datiAzionari.getIndice() + " valore: " + valore);
            if (valore > 70 || valore < 30){
                System.out.println("Valore fuori scala, termino");
                rd.terminaRecuperaDati();
                break;
            }
        }
        try {
            rd.join();
        } catch (InterruptedException e) {}
        System.out.println("Termino il thread main");
    }
}
