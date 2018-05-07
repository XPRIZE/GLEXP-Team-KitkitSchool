package com.enuma.kitkitProvider;

/**
 * Created by ingtellect on 9/1/17.
 */


public class User {

    private int _id;
    private String _username;
    private int _numStars;
    private boolean _finishTutorial;
    private boolean _unlockDrum;
    private boolean _unlockMarimba;
    private boolean _unlockDrawing;
    private boolean _unlockColoring;
    private boolean _unlockBlackboard;
    private boolean _finishLauncherTutorial;

    public User() {
        this._id = 0;
        this._username = "user";
        this._numStars = 0;
        this._finishTutorial = false;
        this._unlockDrum = false;
        this._unlockMarimba = false;
        this._unlockDrawing = false;
        this._unlockColoring = false;
        this._unlockBlackboard = false;
        this._finishLauncherTutorial = false;
    }

    public User(int id, String username, int stars) {
        this._id = id;
        this._username = username;
        this._numStars = stars;
        this._finishTutorial = false;
        this._unlockDrum = false;
        this._unlockMarimba = false;
        this._unlockDrawing = false;
        this._unlockColoring = false;
        this._unlockBlackboard = false;
        this._finishLauncherTutorial = false;
    }

    public User(String productname, int stars) {
        this._username = productname;
        this._numStars = stars;
        this._finishTutorial = false;
        this._unlockDrum = false;
        this._unlockMarimba = false;
        this._unlockDrawing = false;
        this._unlockColoring = false;
        this._unlockBlackboard = false;
        this._finishLauncherTutorial = false;
    }

    public void setID(int id) {
        this._id = id;
    }

    public int getID() {
        return this._id;
    }

    public void setUserName(String username) {
        this._username = username;
    }

    public String getUserName() {
        return this._username;
    }

    public void setNumStars(int stars) {
        this._numStars = stars;
    }

    public int getNumStars() {
        return this._numStars;
    }

    public void setFinishTutorial(boolean unlock) { this._finishTutorial = unlock; }

    public boolean isFinishTutorial() {return true; /*this._finishTutorial;*/}

    public void setUnlockDrum(boolean unlock) { this._unlockDrum = unlock; }

    public boolean isUnlockDrum() {return this._unlockDrum;}

    public void setUnlockMarimba(boolean unlock) { this._unlockMarimba = unlock; }

    public boolean isUnlockMarimba() {return this._unlockMarimba;}

    public void setUnlockDrawing(boolean unlock) { this._unlockDrawing = unlock; }

    public boolean isUnlockDrawing() {return this._unlockDrawing;}

    public void setUnlockColoring(boolean unlock) { this._unlockColoring = unlock; }

    public boolean isUnlockColoring() {return this._unlockColoring;}


    public void setUnlockBlackboard(boolean unlock) { this._unlockBlackboard = unlock; }

    public boolean isUnlockBlackboard() {return this._unlockBlackboard;}

    public void setFinishLauncherTutorial(boolean unlock) { this._finishLauncherTutorial = unlock; }

    public boolean isFinishLauncherTutorial() {return this._finishLauncherTutorial;}
}