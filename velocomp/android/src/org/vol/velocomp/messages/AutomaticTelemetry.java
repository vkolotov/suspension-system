package org.vol.velocomp.messages;

public class AutomaticTelemetry extends Telemetry {
    @FieldOrder(201)
    public short rockingSeverityThreshold;
    @FieldOrder(202)
    public short bumpingSeverityThreshold;
    @FieldOrder(203)
    public short timeout;
    @ArraySize(20)
    @FieldOrder(204)
    public short rockingReadings[];
    @ArraySize(20)
    @FieldOrder(205)
    public short bumpingReadings[];
    @FieldOrder(206)
    public byte dataLength;
}


