package view;

import controller.Controller;

import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.geom.Rectangle2D;

public class InfoView extends JPanel {

    private static final long serialVersionUID = 1L;
    private Controller contr;
    private int N = 8;
    private Rectangle2D[][] rect = new Rectangle2D[N][N];
    private boolean periodic = false;
    private int counter = 0;


    public InfoView(Controller contr) {
        super(true);
        this.contr = contr;
        setFocusable(true);
        addKeyListener(new KeyAdapter() {
            public void keyPressed(KeyEvent e) {
                final int k = e.getKeyCode();
                if (k == KeyEvent.VK_U) {
                    periodic = false;
                    contr.refresh();
                } else if (k == KeyEvent.VK_P) {
                    periodic = true;
                } else if (k == KeyEvent.VK_Q) {
                    periodic = false;
                }
            }
        });
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                rect[i][j] = new Rectangle2D.Double(10 + i * 50, 10 + j * 50, 45, 45);
    }

    public void paint(final Graphics g) {
        try {
            if (periodic) {
                if (counter >= 40) {
                    contr.refresh();
                    counter = 0;
                } else {
                    counter++;
                }
            }
            int[] data = contr.getData();
            int front = data[0], right = data[1], left = data[2];
            int x = data[3], y = data[4];
            int encoder_right = data[5], encoder_left = data[6];

            final Graphics2D g2d = (Graphics2D) g;
            g.setColor(Color.BLACK);
            g.fillRect(0, 0, getWidth(), getHeight());
            for (int i = 0; i < N; i++)
                for (int j = 0; j < N; j++) {
                    if (i == x && j == y)
                        g2d.setColor(Color.RED);
                    else
                        g2d.setColor(Color.WHITE);
                    g2d.fill(rect[i][j]);
                    g2d.draw(rect[i][j]);
                }
            g.setColor(Color.WHITE);
            g.drawString(front + " mm (FRONT)", 460, 30);
            g.drawString(right + " mm (RIGHT)", 460, 60);
            g.drawString(left + " mm (LEFT)", 460, 90);
            g.drawString(encoder_right + " (ENCODER_RIGHT)", 460, 120);
            g.drawString(encoder_left + " (ENCODER_LEFT)", 460, 150);

            g.drawString("Press U for update", 460, 300);
            g.drawString("Press P for starting periodic update", 460, 340);
            g.drawString("Press Q for stopping periodic update", 460, 380);
            requestFocus();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
