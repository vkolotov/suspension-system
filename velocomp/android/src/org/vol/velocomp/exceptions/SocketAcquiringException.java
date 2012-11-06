package org.vol.velocomp.exceptions;

public class SocketAcquiringException extends ConnectionException {

    public SocketAcquiringException() {
    }

    public SocketAcquiringException(String detailMessage) {
        super(detailMessage);
    }

    public SocketAcquiringException(String detailMessage, Throwable throwable) {
        super(detailMessage, throwable);
    }

    public SocketAcquiringException(Throwable throwable) {
        super(throwable);
    }
}
