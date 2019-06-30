package controller;

import bluetooth.Bluetooth;
import model.Data;
import view.AnimationTimer;
import view.BlView;
import view.InfoView;

import javax.swing.*;
import java.util.Timer;

public class Controller extends JFrame {

    private static final long serialVersionUID = 1L;
    private Data model;
    private Bluetooth bluetooth;
    private InfoView info_view;
    private BlView bluetooth_view;
    private AnimationTimer task;
    private Timer timer;

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
    }

    public void switchToInfo(InfoView info_view){
        remove(bluetooth_view);
        this.info_view = info_view;
        setSize(800,800);
        add(info_view);
    }


    public static void main(String[] args) {
        new Controller();
    }
}
