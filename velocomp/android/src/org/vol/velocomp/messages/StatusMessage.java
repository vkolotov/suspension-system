package org.vol.velocomp.messages;

public class StatusMessage {

    public static short OK = 0;
    public static short ERROR = 1;

    @FieldOrder(1)
    public byte status;

}
