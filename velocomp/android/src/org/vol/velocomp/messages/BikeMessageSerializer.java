package org.vol.velocomp.messages;


import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class BikeMessageSerializer {

    public static class FieldComparator implements Comparator<Field> {
        @Override
        public int compare(Field first, Field second) {
            return first.getAnnotation(FieldOrder.class).value() - second.getAnnotation(FieldOrder.class).value();
        }
    }

    public static FieldComparator FIELD_COMPARATOR = new FieldComparator();


    public static void serialize(DataOutputStream dataOutputStream, Object message)
            throws IOException {
        try {
            for (Field field : getSerializableFields(message)) {
                Class fieldType = field.getType();

                if (fieldType.equals(byte.class)) {
                    dataOutputStream.writeByte((Byte) field.get(message));
                } else if (fieldType.equals(short.class)) {
                    shuffleBytes(dataOutputStream, field.getShort(message), 2);
                    //dataOutputStream.writeShort((short) shuffleBytes(field.getLong(message), 2));
                } else if (fieldType.equals(int.class)) {
                    //dataOutputStream.writeInt((int) shuffleBytes(field.getLong(message), 4));
                    shuffleBytes(dataOutputStream, field.getInt(message), 4);
                } else if (fieldType.equals(long.class)) {
                    //dataOutputStream.writeLong(shuffleBytes(field.getLong(message), 8));
                    shuffleBytes(dataOutputStream, field.getLong(message), 8);
                } else if (fieldType.equals(float.class)) {
                    //dataOutputStream.writeInt((int) shuffleBytes(Float.floatToIntBits(field.getFloat(message)), 4));
                    shuffleBytes(dataOutputStream, Float.floatToIntBits(field.getFloat(message)), 4);
                } else if (fieldType.equals(double.class)) {
                    //dataOutputStream.writeInt((int) shuffleBytes(Float.floatToIntBits(field.getFloat(message)), 4));
                    shuffleBytes(dataOutputStream, Float.floatToIntBits((float)field.getDouble(message)), 4);
                } else {
                    throw new IllegalStateException("Type " + fieldType + " is not supported for serialization");
                }

            }
        } catch (IllegalAccessException e) {
            throw new IllegalStateException(e);
        }
    }

    public static <Message> Message deserialize(Class<Message> clazz, DataInputStream dataInputStream)
            throws IOException {
        try {
            Message message = clazz.newInstance();
            for (Field field : getSerializableFields(message)) {
                Class fieldType = field.getType();
                if (fieldType.equals(byte.class)) {
                    checkBytesAvailable(dataInputStream, 1);
                    field.setByte(message, dataInputStream.readByte());
                } else if (fieldType.equals(short.class)) {
                    checkBytesAvailable(dataInputStream, 2);
                    field.setShort(message, (short) shuffleBytes(dataInputStream, 2));
                } else if (fieldType.equals(int.class)) {
                    checkBytesAvailable(dataInputStream, 4);
                    field.setInt(message, (int) shuffleBytes(dataInputStream, 4));
                } else if (fieldType.equals(long.class)) {
                    checkBytesAvailable(dataInputStream, 8);
                    field.setLong(message, shuffleBytes(dataInputStream, 8));
                } else if (fieldType.equals(float.class)) {
                    checkBytesAvailable(dataInputStream, 4);
                    field.setFloat(message, Float.intBitsToFloat((int) shuffleBytes(dataInputStream, 4)));
                } else if (fieldType.equals(double.class)) {
                    checkBytesAvailable(dataInputStream, 4);
                    field.setDouble(message, Float.intBitsToFloat((int) shuffleBytes(dataInputStream, 4)));
                } else {
                    throw new IllegalStateException("Type " + fieldType + " is not supported for deserialization");
                }
            }
            return message;
        } catch (InstantiationException e) {
            throw new IllegalStateException(e);
        } catch (IllegalAccessException e) {
            throw new IllegalStateException(e);
        }
    }

    public static List<Field> getSerializableFields(Object message) {
        List<Field> result = new ArrayList<Field>();
        for (Field field : message.getClass().getFields()) {
            if (field.isAnnotationPresent(FieldOrder.class)) {
                if (!field.getType().isPrimitive()) {
                    throw new IllegalStateException("Only primitive types are supported");
                }
                result.add(field);
            }
        }
        Collections.sort(result, FIELD_COMPARATOR);
        return result;
    }

    private static void checkBytesAvailable(DataInputStream dataInputStream, int count)
            throws IOException {
        if (dataInputStream.available() < count) {
            int tryings = 0;
            while (dataInputStream.available() < count) {
                try {
                    Thread.sleep(10);
                } catch (InterruptedException e) {

                }
                if (tryings >= 100) {
                    throw new IOException("Bytes was not enough");
                }
                tryings++;
            }
        }
    }

    private static long shuffleBytes(DataInputStream dataInputStream, int typeLength) throws IOException {
        long result = 0;
        for (int i = 0; i < typeLength; i++) {
            long nextByte = dataInputStream.readUnsignedByte();
            result |= nextByte << 8 * i;
        }
        return result;
    }

    private static void shuffleBytes(DataOutputStream dataOutputStream, long value, int typeLength) throws IOException {

        for (int i = 0; i < typeLength; i++) {
            long nextByte = (value >>> 8 * i) & 0xFF;
            dataOutputStream.writeByte((int) nextByte);
        }
    }
}
