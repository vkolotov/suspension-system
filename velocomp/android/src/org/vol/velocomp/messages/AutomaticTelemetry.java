package org.vol.velocomp.messages;

public class AutomaticTelemetry extends Telemetry {
    @FieldOrder(201)
    public short severityThreshold;
    @FieldOrder(202)
    public short idleValue;
}


