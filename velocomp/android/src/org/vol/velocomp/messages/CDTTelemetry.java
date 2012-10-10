package org.vol.velocomp.messages;

public class CDTTelemetry extends ManualTelemetry {
    @FieldOrder(200)
    public byte gradient;
    @FieldOrder(201)
    public byte climbGradient;
    @FieldOrder(202)
    public byte descendGradient;
}


