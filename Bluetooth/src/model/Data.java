package model;

public class Data {
    private int front, right, left;
    private int x, y;
    private int encoder_right, encoder_left;

    public void setFront(int front) {
        this.front = front;
    }

    public void setRight(int right) {
        this.right = right;
    }

    public void setLeft(int left) {
        this.left = left;
    }

    public void setX(int x) {
        this.x = x;
    }

    public void setY(int y) {
        this.y = y;
    }

    public void setEncoder_right(int encoder_right) {
        this.encoder_right = encoder_right;
    }

    public void setEncoder_left(int encoder_left) {
        this.encoder_left = encoder_left;
    }

    public int getFront() {
        return front;
    }

    public int getRight() {
        return right;
    }

    public int getLeft() {
        return left;
    }

    public int getX() {
        return x;
    }

    public int getY() {
        return y;
    }

    public int getEncoder_right() {
        return encoder_right;
    }

    public int getEncoder_left() {
        return encoder_left;
    }
}