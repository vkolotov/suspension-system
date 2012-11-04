package org.vol.velocomp.messages;

public class Telemetry {

    @FieldOrder(0)
    public short clockSpeed;
    @FieldOrder(1)
    public short freeMemory;
    @FieldOrder(2)
    public byte state;
    @FieldOrder(3)
    public byte speed;
    @FieldOrder(4)
    public short cadence;
}

