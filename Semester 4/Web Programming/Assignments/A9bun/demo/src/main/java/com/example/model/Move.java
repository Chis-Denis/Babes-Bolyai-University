package com.example.model;

import java.sql.Timestamp;

public class Move {
    private int id;
    private int gameSessionId;
    private Timestamp moveTime;
    private int xCoordinate;
    private int yCoordinate;
    private String direction;

    public Move() {}

    public Move(int gameSessionId, int xCoordinate, int yCoordinate, String direction) {
        this.gameSessionId = gameSessionId;
        this.xCoordinate = xCoordinate;
        this.yCoordinate = yCoordinate;
        this.direction = direction;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getGameSessionId() {
        return gameSessionId;
    }

    public void setGameSessionId(int gameSessionId) {
        this.gameSessionId = gameSessionId;
    }

    public Timestamp getMoveTime() {
        return moveTime;
    }

    public void setMoveTime(Timestamp moveTime) {
        this.moveTime = moveTime;
    }

    public int getXCoordinate() {
        return xCoordinate;
    }

    public void setXCoordinate(int xCoordinate) {
        this.xCoordinate = xCoordinate;
    }

    public int getYCoordinate() {
        return yCoordinate;
    }

    public void setYCoordinate(int yCoordinate) {
        this.yCoordinate = yCoordinate;
    }

    public String getDirection() {
        return direction;
    }

    public void setDirection(String direction) {
        this.direction = direction;
    }
} 