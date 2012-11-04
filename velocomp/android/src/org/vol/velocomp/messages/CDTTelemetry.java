package org.vol.velocomp.messages;

public class CDTTelemetry extends ManualTelemetry {
    @FieldOrder(200)
    public byte gradient;
    @FieldOrder(201)
    public byte climbGradient;
    @FieldOrder(202)
    public byte descendGradient;
    @FieldOrder(204)
    @ArraySize(10)
    public short[] rawGradients;
    @FieldOrder(205)
    @ArraySize(10)
    public short[] filteredGradients;
    @FieldOrder(206)
    public byte dataLength;
}


