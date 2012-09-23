package org.vol.velocomp.messages;

public class Telemetry {
    @FieldOrder(0)
    public byte speed;
    @FieldOrder(1)
    public short cadence;
    @FieldOrder(2)
    public int distance;
    @FieldOrder(3)
    public short heartRate;
    @FieldOrder(4)
    public float floatField;
    @FieldOrder(5)
    public double doubleField;
}
