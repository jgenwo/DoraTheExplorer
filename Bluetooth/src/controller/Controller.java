package controller;

import bluetooth.Bluetooth;
import model.Data;
import view.AnimationTimer;
import view.BlView;
import view.InfoView;

import javax.swing.*;
import java.io.IOException;
import java.util.Timer;

public class Controller extends JFrame {

    private static final long serialVersionUID = 1L;
    private Data model;
    private Bluetooth bluetooth;
    private InfoView info_view;
    private BlView bluetooth_view;
    private AnimationTimer task;
    private Timer timer;
    private boolean periodic = false;

    public Controller() {
        super("MICROMOUSE");
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setSize(500, 300);
        model = new Data();
        bluetooth = new Bluetooth();
        bluetooth_view = new BlView(this, bluetooth);
        add(bluetooth_view);
        setVisible(true);
        timer = new java.util.Timer();
        task = new AnimationTimer(this);
        timer.scheduleAtFixedRate(task, 0, 20);

        //switchToInfo(new InfoView(this));
    }

    public void switchToInfo(InfoView info_view) {
        remove(bluetooth_view);
        this.info_view = info_view;
        setSize(800, 460);
        add(info_view);
    }

    public int[] getData() {
        return new int[]{model.getFront(), model.getRight(), model.getLeft(), model.getX(), model.getY(), model.getEncoder_right(), model.getEncoder_left()};
    }

    public void refresh() {
        int[] dat;
        String data = "";
        try {
            bluetooth.write("RTS");
            data = bluetooth.read();
        } catch (IOException e) {
        }
        dat = parseData(data);
        model.setFront(dat[0]);
        model.setRight(dat[1]);
        model.setLeft(dat[2]);
        model.setX(dat[3]);
        model.setY(dat[4]);
        model.setEncoder_right(dat[5]);
        model.setEncoder_left(dat[6]);
    }

    /**
     * @param data "UPDATE{F:value;R:value;L:value;X:value;Y:value;ER:value;EL:value;}"
     * @return
     */
    public int[] parseData(String data) {
        int[] dat = new int[7];
        if (data.startsWith("UPDATE")) {
            data = data.substring(data.indexOf('{') + 1, data.indexOf('}'));
            for (int i = 0; i < dat.length; i++) {
                dat[i] = Integer.parseInt(data.substring(data.indexOf(':') + 1, data.indexOf(';')));
                data = data.substring(data.indexOf(";") + 1);
            }
        }
        return dat;
    }


    public static void main(String[] args) {
        new Controller();
    }
}
