package com.example.dao;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import com.example.model.Move;
import com.example.util.DatabaseUtil;

public class GameDAO {
    public int createGameSession(int userId) {
        String query = "INSERT INTO game_sessions (user_id) VALUES (?)";
        try (Connection conn = DatabaseUtil.getConnection();
             PreparedStatement pstmt = conn.prepareStatement(query, Statement.RETURN_GENERATED_KEYS)) {
            
            pstmt.setInt(1, userId);
            pstmt.executeUpdate();
            
            try (ResultSet rs = pstmt.getGeneratedKeys()) {
                if (rs.next()) {
                    return rs.getInt(1);
                }
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return -1;
    }

    public void endGameSession(int gameSessionId, int score) {
        String query = "UPDATE game_sessions SET end_time = CURRENT_TIMESTAMP, score = ? WHERE id = ?";
        try (Connection conn = DatabaseUtil.getConnection();
             PreparedStatement pstmt = conn.prepareStatement(query)) {
            
            pstmt.setInt(1, score);
            pstmt.setInt(2, gameSessionId);
            pstmt.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public void recordMove(Move move) {
        String query = "INSERT INTO moves (game_session_id, x_coordinate, y_coordinate, direction) VALUES (?, ?, ?, ?)";
        try (Connection conn = DatabaseUtil.getConnection();
             PreparedStatement pstmt = conn.prepareStatement(query)) {
            
            pstmt.setInt(1, move.getGameSessionId());
            pstmt.setInt(2, move.getXCoordinate());
            pstmt.setInt(3, move.getYCoordinate());
            pstmt.setString(4, move.getDirection());
            pstmt.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
} 