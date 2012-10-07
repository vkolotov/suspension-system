package org.vol.velocomp.messages;



public class Configuration {

    @FieldOrder(0)
    public boolean virgin;
    @FieldOrder(1)
    public SystemConfig system;
    @FieldOrder(2)
    public ButtonsSystem buttons;
    @FieldOrder(3)
    public SpeedSystemConfig speed;
    @FieldOrder(4)
    public CadenceSystemConfig cadence;
    @FieldOrder(5)
    public SuspensionSystemConfig frontSuspension;
    @FieldOrder(6)
    public SuspensionSystemConfig rearSuspension;
    @FieldOrder(7)
    public AccelerometerSystemConfig sprungAccelerometerSystem;
    @FieldOrder(8)
    public UnsprungAccelerometerSystemConfig unsprungAccelerometerSystem;
    @FieldOrder(9)
    public PowerSaveSystemConfig powerSave;
    @FieldOrder(10)
    public SemiautomaticStateConfig semiautomaticStateConfig;


    public static class SystemConfig {
        @FieldOrder(0)
        public byte mode;
        @FieldOrder(1)
        public short wheelBase;
        @FieldOrder(2)
        public short maxUnlockTimeout;
        @FieldOrder(3)
        public float headTubeGradient;
        @FieldOrder(4)
        public float maxBumpsPerMeter;
    }

    public static class ButtonsSystem {
        @FieldOrder(0)
        public byte frontPin;
        @FieldOrder(1)
        public byte modePin;
        @FieldOrder(2)
        public byte rearPin;
        @FieldOrder(3)
        public short debounceDuration;
        @FieldOrder(4)
        public byte frontPinReferenceValue;
        @FieldOrder(5)
        public byte modePinReferenceValue;
        @FieldOrder(6)
        public byte rearPinReferenceValue;
    }

    public static class FrequencySystemConfig {
        @FieldOrder(0)
        public byte pin;
        @FieldOrder(1)
        public short minTime;
        @FieldOrder(2)
        public short maxTime;
        @FieldOrder(3)
        public byte referenceValue;
    }

    public static class SpeedSystemConfig {
        @FieldOrder(0)
        public FrequencySystemConfig frequencySystemConfig;
        @FieldOrder(1)
        public short wheelLength;
    }

    public static class CadenceSystemConfig {
        @FieldOrder(0)
        public FrequencySystemConfig frequencySystemConfig;
        @FieldOrder(1)
        public short timeoutCorrection;
    }

    public static class SuspensionSystemConfig {
        @FieldOrder(0)
        public byte controlPin;
        @FieldOrder(1)
        public byte feedbackPin;
        @FieldOrder(2)
        public short calibrationDelay;
        @FieldOrder(3)
        public byte calibrationThreshold;
        @FieldOrder(4)
        public byte calibrationStep;
        @FieldOrder(5)
        public short minAngle;
        @FieldOrder(6)
        public short maxAngle;
        @FieldOrder(7)
        public byte modes;
        @FieldOrder(8)
        @ArraySize(9)
        public short[] angles;
    }

    public static class AccelerometerSystemConfig {
        @FieldOrder(0)
        public short address;
        @FieldOrder(1)
        public byte range;
        @FieldOrder(2)
        public short severityThreshold;
        @FieldOrder(3)
        public short noiseThreshold;
    }

    public static class UnsprungAccelerometerSystemConfig {
        @FieldOrder(0)
        public AccelerometerSystemConfig accelerometerSystemConfig;
        @FieldOrder(1)
        public short measuringPeriod;
    }

    public static class PowerSaveSystemConfig {
        @FieldOrder(0)
        public byte servoRelayPin;
        @FieldOrder(1)
        public byte serialRelayPin;
        @FieldOrder(2)
        public byte i2cRelayPin;
        @FieldOrder(3)
        public byte sleepInterruption;
        @FieldOrder(4)
        public short servoStandByTimeout;
        @FieldOrder(5)
        public int sleepTimeout;
        @FieldOrder(6)
        public boolean isServoPowerOn;
        @FieldOrder(7)
        public boolean isSerialPowerOn;
        @FieldOrder(8)
        public boolean isi2cPowerOn;
    }

    public static class SemiautomaticStateConfig {
        @FieldOrder(0)
        public float climbGradient;
        @FieldOrder(1)
        public float descentGradient;
        @FieldOrder(2)
        public float transitionGap;
        @FieldOrder(3)
        public short averageDegreeMeasuringPeriod;
    }
}
