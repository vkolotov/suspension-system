package org.vol.velocomp.messages;

public class AutomaticTelemetry extends Telemetry {
    @FieldOrder(201)
    public short sprungSeverityThreshold;
    @FieldOrder(202)
    public short unsprungSeverityThreshold;
    @FieldOrder(203)
    public short timeout;
    @ArraySize(20)
    @FieldOrder(204)
    public short sprungReadingsX[];
    @ArraySize(20)
    @FieldOrder(205)
    public short  unsprungReadingsX[];
    @FieldOrder(206)
    public byte dataLength;
}


