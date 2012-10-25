package org.vol.velocomp.messages;

public class Telemetry {

    @FieldOrder(0)
    public short clockSpeed;
    @FieldOrder(1)
    public byte state;
    @FieldOrder(2)
    public byte speed;
    @FieldOrder(3)
    public short cadence;
}

