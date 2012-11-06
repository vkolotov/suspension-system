package org.vol.velocomp.exceptions;

public class BikeNotFoundException extends ConnectionException {

    public BikeNotFoundException() {
    }

    public BikeNotFoundException(String detailMessage) {
        super(detailMessage);
    }

    public BikeNotFoundException(String detailMessage, Throwable throwable) {
        super(detailMessage, throwable);
    }

    public BikeNotFoundException(Throwable throwable) {
        super(throwable);
    }
}
