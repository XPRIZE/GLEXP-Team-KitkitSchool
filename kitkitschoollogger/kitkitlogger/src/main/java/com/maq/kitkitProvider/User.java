package com.maq.kitkitProvider;

/**
 * Created by ingtellect on 9/1/17.
 */


public class User<string> {
    public static boolean DEFAULT_OPEN_LIBRARY = true;
    public static boolean DEFAULT_OPEN_TOOLS = true;

    private int _id;
    private String _username;
    private String _gender;
    private string _age;
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
    /////////////////////
    private int _gamesClearedInTotal_L;
    private int _gamesClearedInTotal_M;
    /////////////////////

    public User() {
        this._id = 0;
        this._username = "user";
        this._gender = "male";
        this._age = (string) "0";
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
        this._unlockWritingBoard = false;
        this._finishWritingBoardTutorial = false;
        this._gamesClearedInTotal_L = 0;
        this._gamesClearedInTotal_M = 0;
    }

    public User(String displayname, String gender, String age) {
        this._id = 0;
        this._username = displayname;
        this._gender = gender;
        this._age = (string) age;
        this._numStars = 0;
        this._finishTutorial = false;
        this._unlockDrum = true;
        this._unlockMarimba = true;
        this._unlockDrawing = true;
        this._unlockColoring = true;
        this._unlockBlackboard = true;
        this._finishLauncherTutorial = false;
        this._displayName = displayname;
        this._openLibrary = DEFAULT_OPEN_LIBRARY;
        this._openTools = DEFAULT_OPEN_TOOLS;
        this._unlockFishBowl = false;
        this._unlockWritingBoard = false;
        this._finishWritingBoardTutorial = false;
        this._gamesClearedInTotal_L = 0;
        this._gamesClearedInTotal_M = 0;
    }

    public User(int id, String username, int stars) {
        this._id = id;
        this._username = username;
        this._gender = "male";
        this._age = (string) "0";
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
        this._unlockWritingBoard = false;
        this._finishWritingBoardTutorial = false;
        this._gamesClearedInTotal_L = 0;
        this._gamesClearedInTotal_M = 0;
    }

    public User(String productname, int stars) {
        this._username = productname;
        this._gender = "male";
        this._age = (string) "0";
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
        this._unlockWritingBoard = false;
        this._finishWritingBoardTutorial = false;
        this._gamesClearedInTotal_L = 0;
        this._gamesClearedInTotal_M = 0;
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

    public void setgender(String gender) {
        this._gender = gender;
    }

    public String getgender() {
        return this._gender;
    }

    public void setAge(string age) {
        this._age = age;
    }

    public String getAge() {
        return (String) this._age;
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

    public void setDisplayName(String name) {
        this._displayName = name;
    }

    public String getDisplayName() {
        return this._displayName;
    }

    public void setOpenLibrary(boolean open) {
        _openLibrary = open;
    }

    public boolean isOpenLibrary() {
        return _openLibrary;
    }

    public void setOpenTools(boolean open) {
        _openTools = open;
    }

    public boolean isOpenTools() {
        return _openTools;
    }

    public void setUnlockFishBowl(boolean unlock) { this._unlockFishBowl = unlock; }

    public boolean isUnlockFishBowl() {return this._unlockFishBowl;}

    public void setUnlockWritingBoard(boolean unlock) { this._unlockWritingBoard = unlock; }

    public boolean isUnlockWritingBoard() {return this._unlockWritingBoard;}

    public void setFinishWritingBoardTutorial(boolean unlock) { this._finishWritingBoardTutorial = unlock; }

    public boolean isFinishWritingBoardTutorial() {return this._finishWritingBoardTutorial;}

    /////////////////////////
    public void setGamesClearedInTotal_L(int gamesClearedInTotal_L) {this._gamesClearedInTotal_L = gamesClearedInTotal_L; }

    public int getGamesClearedInTotal_L() {return this._gamesClearedInTotal_L; }

    public void setGamesClearedInTotal_M(int gamesClearedInTotal_M) {this._gamesClearedInTotal_M = gamesClearedInTotal_M; }

    public int getGamesClearedInTotal_M() {return this._gamesClearedInTotal_M; }
    /////////////////////////
}