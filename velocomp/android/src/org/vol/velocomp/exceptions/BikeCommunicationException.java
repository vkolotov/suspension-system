package org.vol.velocomp.exceptions;

public class BikeCommunicationException extends ConnectionException {

    public BikeCommunicationException() {
    }

    public BikeCommunicationException(String detailMessage) {
        super(detailMessage);
    }

    public BikeCommunicationException(String detailMessage, Throwable throwable) {
        super(detailMessage, throwable);
    }

    public BikeCommunicationException(Throwable throwable) {
        super(throwable);
    }
}
