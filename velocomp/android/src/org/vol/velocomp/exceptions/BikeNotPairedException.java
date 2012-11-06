package org.vol.velocomp.exceptions;

public class BikeNotPairedException extends ConnectionException {

    public BikeNotPairedException() {
    }

    public BikeNotPairedException(String detailMessage) {
        super(detailMessage);
    }

    public BikeNotPairedException(String detailMessage, Throwable throwable) {
        super(detailMessage, throwable);
    }

    public BikeNotPairedException(Throwable throwable) {
        super(throwable);
    }
}
