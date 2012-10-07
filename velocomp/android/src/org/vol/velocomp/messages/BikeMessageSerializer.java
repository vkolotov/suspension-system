package org.vol.velocomp.messages;


import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.lang.reflect.Array;
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

    public static short getSizeOf(Class messageType) {

        short result = 0;
        for (Field field : getSerializableFields(messageType)) {
            Class fieldType = field.getType();
            if (fieldType.isPrimitive()) {
                result += getPrimitiveFieldSize(fieldType);
            } else if (fieldType.isArray()) {
                Class arrayType = field.getType().getComponentType();
                int arrayLength = field.getAnnotation(ArraySize.class).value();
                short arrayTypeLength;
                if (arrayType.isPrimitive()) {
                    arrayTypeLength = getPrimitiveFieldSize(arrayType);
                } else if (arrayType.isArray()) {
                    throw new IllegalStateException("Multiarray is not supported");
                } else {
                    arrayTypeLength = getSizeOf(arrayType);
                }
                result += arrayTypeLength * arrayLength;
            } else {
                result += getSizeOf(fieldType);
            }

        }
        return result;
    }

    private static short getPrimitiveFieldSize(Class fieldType) {
        if (fieldType.equals(byte.class) || fieldType.equals(boolean.class)) {
            return Byte.SIZE / 8;
        } else if (fieldType.equals(short.class)) {
            return Short.SIZE / 8;
        } else if (fieldType.equals(int.class)) {
            return Integer.SIZE / 8;
        } else if (fieldType.equals(long.class)) {
            return Long.SIZE / 8;
        } else if (fieldType.equals(float.class)) {
            return Float.SIZE / 8;
        } else if (fieldType.equals(double.class)) {
            return Float.SIZE / 8;
        } else {
            throw new IllegalStateException("Type " + fieldType + " is not supported");
        }
    }

    public static void serialize(DataOutputStream dataOutputStream, Object message)
            throws IOException {
        try {
            for (Field field : getSerializableFields(message.getClass())) {
                Class fieldType = field.getType();
                if (fieldType.isPrimitive()) {
                    serializePrimitive(message, field, dataOutputStream);
                } else if (fieldType.isArray()) {
                    serializeArray(message, field, dataOutputStream);
                } else {
                    serialize(dataOutputStream, field.get(message));
                }

            }
        } catch (IllegalAccessException e) {
            throw new IllegalStateException(e);
        }
    }

    private static void serializeArray(Object message, Field field, DataOutputStream dataOutputStream) throws IOException, IllegalAccessException {
        if (!field.isAnnotationPresent(ArraySize.class)) {
            throw new IllegalStateException("Array is not annotated with ArraySize");
        }
        Class arrayType = field.getType().getComponentType();
        int size = field.getAnnotation(ArraySize.class).value();
        Object array = field.get(message);

        for (int i = 0; i < size; i++) {
            if (arrayType.equals(byte.class)) {
                dataOutputStream.writeByte(Array.getByte(array, i));
            } else if (arrayType.equals(boolean.class)) {
                dataOutputStream.writeByte(Array.getBoolean(array, i) ? 1 : 0);
            } else if (arrayType.equals(short.class)) {
                shuffleBytes(dataOutputStream, Array.getShort(array, i), 2);
            } else if (arrayType.equals(int.class)) {
                shuffleBytes(dataOutputStream, Array.getInt(array, i), 4);
            } else if (arrayType.equals(long.class)) {
                shuffleBytes(dataOutputStream, Array.getLong(array, i), 8);
            } else if (arrayType.equals(float.class)) {
                shuffleBytes(dataOutputStream, Float.floatToIntBits(Array.getFloat(array, i)), 4);
            } else if (arrayType.equals(double.class)) {
                shuffleBytes(dataOutputStream, Float.floatToIntBits((float)Array.getDouble(array, i)), 4);
            } else if (arrayType.isArray()) {
                throw new IllegalStateException("Multiarray is not supported");
            } else {
                serialize(dataOutputStream, Array.get(array, i));
            }
        }
    }

    private static <Message> void serializePrimitive(Message message, Field field, DataOutputStream dataOutputStream) throws IllegalAccessException, IOException {
        Class fieldType = field.getType();

        if (fieldType.equals(byte.class)) {
            dataOutputStream.writeByte((Byte) field.get(message));
        } else if (fieldType.equals(boolean.class)) {
            dataOutputStream.writeByte(field.getBoolean(message) ? 1 : 0);
        } else if (fieldType.equals(short.class)) {
            shuffleBytes(dataOutputStream, field.getShort(message), 2);
        } else if (fieldType.equals(int.class)) {
            shuffleBytes(dataOutputStream, field.getInt(message), 4);
        } else if (fieldType.equals(long.class)) {
            shuffleBytes(dataOutputStream, field.getLong(message), 8);
        } else if (fieldType.equals(float.class)) {
            shuffleBytes(dataOutputStream, Float.floatToIntBits(field.getFloat(message)), 4);
        } else if (fieldType.equals(double.class)) {
            shuffleBytes(dataOutputStream, Float.floatToIntBits((float)field.getDouble(message)), 4);
        } else {
            throw new IllegalStateException("Type " + fieldType + " is not supported for serialization");
        }
    }

    public static <Message> Message deserialize(Class<Message> clazz, DataInputStream dataInputStream)
            throws IOException {
        try {
            Message message = clazz.newInstance();
            for (Field field : getSerializableFields(message.getClass())) {
                Class fieldType = field.getType();

                if (fieldType.isPrimitive()) {
                    desearializePrimitive(message, field, dataInputStream);
                } else if (fieldType.isArray()) {
                    field.set(message, desearializeArray(field, dataInputStream));
                } else {
                    field.set(message, deserialize(field.getType(), dataInputStream));
                }
            }
            return message;
        } catch (InstantiationException e) {
            throw new IllegalStateException(e);
        } catch (IllegalAccessException e) {
            throw new IllegalStateException(e);
        }

    }

    private static Object desearializeArray(Field field, DataInputStream dataInputStream) throws IOException, IllegalAccessException {
        if (!field.isAnnotationPresent(ArraySize.class)) {
            throw new IllegalStateException("Array is not annotated with ArraySize");
        }
        Class arrayType = field.getType().getComponentType();
        int size = field.getAnnotation(ArraySize.class).value();
        Object array = Array.newInstance(arrayType, size);

        for (int i = 0; i < size; i++) {
            if (arrayType.equals(byte.class)) {
                checkBytesAvailable(dataInputStream, 1);
                Array.setByte(array, i, dataInputStream.readByte());
            } else if (arrayType.equals(boolean.class)) {
                checkBytesAvailable(dataInputStream, 1);
                Array.setBoolean(array, i, dataInputStream.readByte() == 0);
            } else if (arrayType.equals(short.class)) {
                checkBytesAvailable(dataInputStream, 2);
                Array.setShort(array, i, (short) shuffleBytes(dataInputStream, 2));
            } else if (arrayType.equals(int.class)) {
                checkBytesAvailable(dataInputStream, 4);
                Array.setInt(array, i, (int) shuffleBytes(dataInputStream, 4));
            } else if (arrayType.equals(long.class)) {
                checkBytesAvailable(dataInputStream, 8);
                Array.setLong(array, i, shuffleBytes(dataInputStream, 8));
            } else if (arrayType.equals(float.class)) {
                checkBytesAvailable(dataInputStream, 4);
                Array.setFloat(array, i, Float.intBitsToFloat((int) shuffleBytes(dataInputStream, 4)));
            } else if (arrayType.equals(double.class)) {
                checkBytesAvailable(dataInputStream, 4);
                Array.setDouble(array, i, Float.intBitsToFloat((int) shuffleBytes(dataInputStream, 4)));
            } else if (arrayType.isArray()) {
                throw new IllegalStateException("Multiarray is not supported");
            } else {
                Array.set(array, i, deserialize(arrayType, dataInputStream));
            }
        }
        return array;
    }

    private static <Message> void desearializePrimitive(Message message, Field field, DataInputStream dataInputStream) throws IOException, IllegalAccessException {
        Class fieldType = field.getType();
        if (fieldType.equals(byte.class)) {
            checkBytesAvailable(dataInputStream, 1);
            field.setByte(message, dataInputStream.readByte());
        } else if (fieldType.equals(boolean.class)) {
            checkBytesAvailable(dataInputStream, 1);
            field.setBoolean(message, dataInputStream.readByte() != 0);
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

    public static List<Field> getSerializableFields(Class messageClass) {
        List<Field> result = new ArrayList<Field>();
        for (Field field : messageClass.getFields()) {
            if (field.isAnnotationPresent(FieldOrder.class)) {
//                if (!field.getType().isPrimitive()) {
//                    throw new IllegalStateException("Only primitive types are supported");
//                }
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
