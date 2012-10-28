package org.vol.velocomp.utils;

import java.util.LinkedList;

public class FixedSizeQueue<E> extends LinkedList<E> {

    private int limit;

    public FixedSizeQueue(int limit) {
        this.limit = limit;
    }

    @Override
    public boolean add(E o) {
        super.add(o);
        while (size() > limit) { super.remove(); }
        return true;
    }
}
