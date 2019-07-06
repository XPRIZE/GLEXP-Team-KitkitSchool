package com.maq.kitkitProvider;

/**
 * Created by ingtellect on 9/1/17.
 */


public class User {
    public static boolean DEFAULT_OPEN_LIBRARY = true;
    public static boolean DEFAULT_OPEN_TOOLS = true;

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
    private String _displayName;
    private boolean _openLibrary;
    private boolean _openTools;
    private boolean _unlockFishBowl;
    private boolean _unlockWritingBoard;
    private boolean _finishWritingBoardTutorial;

    public User() {
        this._id = 0;
        this._username = "user";
        this._numStars = 0;
        this._finishTutorial = false;
        this._unlockDrum = true;
        this._unlockMarimba = true;
        this._unlockDrawing = true;
        this._unlockColoring = true;
        this._unlockBlackboard = true;
        this._finishLauncherTutorial = false;
        this._displayName = "";
        this._openLibrary = DEFAULT_OPEN_LIBRARY;
        this._openTools = DEFAULT_OPEN_TOOLS;
        this._unlockFishBowl = false;
        this._unlockWritingBoard = true;
        this._finishWritingBoardTutorial = true;
    }

    public User(int id, String username, int stars) {
        this._id = id;
        this._username = username;
        this._numStars = stars;
        this._finishTutorial = false;
        this._unlockDrum = true;
        this._unlockMarimba = true;
        this._unlockDrawing = true;
        this._unlockColoring = true;
        this._unlockBlackboard = true;
        this._finishLauncherTutorial = false;
        this._displayName = "";
        this._openLibrary = DEFAULT_OPEN_LIBRARY;
        this._openTools = DEFAULT_OPEN_TOOLS;
        this._unlockFishBowl = false;
        this._unlockWritingBoard = true;
        this._finishWritingBoardTutorial = true;
    }

    public User(String productname, int stars) {
        this._username = productname;
        this._numStars = stars;
        this._finishTutorial = false;
        this._unlockDrum = true;
        this._unlockMarimba = true;
        this._unlockDrawing = true;
        this._unlockColoring = true;
        this._unlockBlackboard = true;
        this._finishLauncherTutorial = false;
        this._displayName = "";
        this._openLibrary = DEFAULT_OPEN_LIBRARY;
        this._openTools = DEFAULT_OPEN_TOOLS;
        this._unlockFishBowl = false;
        this._unlockWritingBoard = true;
        this._finishWritingBoardTutorial = true;
    }

    public int getID() {
        return this._id;
    }

    public void setID(int id) {
        this._id = id;
    }

    public String getUserName() {
        return this._username;
    }

    public void setUserName(String username) {
        this._username = username;
    }

    public int getNumStars() {
        return this._numStars;
    }

    public void setNumStars(int stars) {
        this._numStars = stars;
    }

    public boolean isFinishTutorial() {
        return true;
    }

    public void setFinishTutorial(boolean unlock) {
        this._finishTutorial = unlock;
    }

    public boolean isUnlockDrum() {
        return this._unlockDrum;
    }

    public void setUnlockDrum(boolean unlock) {
        this._unlockDrum = unlock;
    }

    public boolean isUnlockMarimba() {
        return this._unlockMarimba;
    }

    public void setUnlockMarimba(boolean unlock) {
        this._unlockMarimba = unlock;
    }

    public boolean isUnlockDrawing() {
        return this._unlockDrawing;
    }

    public void setUnlockDrawing(boolean unlock) {
        this._unlockDrawing = unlock;
    }

    public boolean isUnlockColoring() {
        return this._unlockColoring;
    }

    public void setUnlockColoring(boolean unlock) {
        this._unlockColoring = unlock;
    }

    public boolean isUnlockBlackboard() {
        return this._unlockBlackboard;
    }

    public void setUnlockBlackboard(boolean unlock) {
        this._unlockBlackboard = unlock;
    }

    public boolean isFinishLauncherTutorial() {
        return this._finishLauncherTutorial;
    }

    public void setFinishLauncherTutorial(boolean unlock) {
        this._finishLauncherTutorial = unlock;
    }

    public String getDisplayName() {
        return _displayName;
    }

    public void setDisplayName(String name) {
        _displayName = name;
    }

    public boolean isOpenLibrary() {
        return _openLibrary;
    }

    public void setOpenLibrary(boolean open) {
        _openLibrary = open;
    }

    public boolean isOpenTools() {
        return _openTools;
    }

    public void setOpenTools(boolean open) {
        _openTools = open;
    }

    public boolean isUnlockFishBowl() {
        return this._unlockFishBowl;
    }

    public void setUnlockFishBowl(boolean unlock) {
        this._unlockFishBowl = unlock;
    }

    public boolean isUnlockWritingBoard() {
        return this._unlockWritingBoard;
    }

    public void setUnlockWritingBoard(boolean unlock) {
        this._unlockWritingBoard = unlock;
    }

    public boolean isFinishWritingBoardTutorial() {
        return this._finishWritingBoardTutorial;
    }

    public void setFinishWritingBoardTutorial(boolean unlock) {
        this._finishWritingBoardTutorial = unlock;
    }

}