package org.vol.velocomp.views;

public enum Mode {
    MANUAL(0, "Manual"),
    CDT(10, "CDT"),
    AUTOMATIC(20, "Automatic"),

    COMMON_STATE(100, "Common"),
    SLEEP_STATE(101, "Sleep"),

    CLIMB_STATE(11, "Climb"),
    DESCENT_STATE(12, "Descent"),
    TRAIL_STATE(13, "Trail"),

    ABSORB_STATE(21, "Absorb"),
    ACTIVE_STATE(22, "Active"),
    BURNOUT_STATE(23, "Burnout"),
    IDLE_STATE(24, "Idle"),
    PEDAL_STATE(25, "Pedal"),
    STANDOVER_STATE(26, "Standover");

    private int state;
    private String name;

    Mode(int state, String name) {
        this.state = state;
        this.name = name;
    }

    int getBoard() {
        return state / 10;
    }

    public String getName() {
        return name;
    }

    public static Mode matchState(int state) {
        if (state < 10) {
            return  MANUAL;
        } else if (state >= 10 && state < 20) {
            return CDT;
        } else {
            return AUTOMATIC;
        }
    }

    public static Mode getMode(int state) {
        for (Mode mode : values()) {
            if (mode.state == state) {
                return mode;
            }
        }
        return null;
    }
}
