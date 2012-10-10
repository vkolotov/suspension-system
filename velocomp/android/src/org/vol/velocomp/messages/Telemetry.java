package org.vol.velocomp.messages;

public class Telemetry {

    @FieldOrder(0)
    public byte state;
    @FieldOrder(1)
    public byte speed;
    @FieldOrder(2)
    public short cadence;
}

