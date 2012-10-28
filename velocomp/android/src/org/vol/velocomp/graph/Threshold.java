package org.vol.velocomp.graph;

public class Threshold {

    protected int color;
    protected int thickness;
    protected double value;

    public Threshold(int color, int thickness, double value) {
        this.color = color;
        this.thickness = thickness;
        this.value = value;
    }

    public int getColor() {
        return color;
    }

    public void setColor(int color) {
        this.color = color;
    }

    public int getThickness() {
        return thickness;
    }

    public void setThickness(int thickness) {
        this.thickness = thickness;
    }

    public double getValue() {
        return value;
    }

    public void setValue(double value) {
        this.value = value;
    }
}
