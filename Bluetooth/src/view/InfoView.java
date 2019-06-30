package view;

import controller.Controller;

import javax.swing.*;
import java.awt.*;

public class InfoView extends JPanel {

    private static final long serialVersionUID = 1L;
    private Controller contr;

    public InfoView(Controller contr) {
        super(true);
        this.contr = contr;
        setFocusable(true);
    }
    public void paint(final Graphics g) {

    }
}
