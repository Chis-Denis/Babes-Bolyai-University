package com.example.servlet;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import com.example.dao.GameDAO;
import com.example.model.Move;
import com.example.model.User;

@WebServlet("/game/*")
public class GameServlet extends HttpServlet {
    private GameDAO gameDAO;

    @Override
    public void init() throws ServletException {
        gameDAO = new GameDAO();
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) 
            throws ServletException, IOException {
        HttpSession session = request.getSession();
        User user = (User) session.getAttribute("user");
        
        if (user == null) {
            response.sendRedirect("login.jsp");
            return;
        }

        String action = request.getPathInfo();
        response.setContentType("application/json");
        PrintWriter out = response.getWriter();

        switch (action) {
            case "/start":
                int gameSessionId = gameDAO.createGameSession(user.getId());
                session.setAttribute("gameSessionId", gameSessionId);
                out.print("{\"gameSessionId\": " + gameSessionId + "}");
                break;

            case "/move":
                int sessionId = (int) session.getAttribute("gameSessionId");
                int x = Integer.parseInt(request.getParameter("x"));
                int y = Integer.parseInt(request.getParameter("y"));
                String direction = request.getParameter("direction");
                
                Move move = new Move(sessionId, x, y, direction);
                gameDAO.recordMove(move);
                out.print("{\"success\": true}");
                break;

            case "/end":
                sessionId = (int) session.getAttribute("gameSessionId");
                int score = Integer.parseInt(request.getParameter("score"));
                gameDAO.endGameSession(sessionId, score);
                session.removeAttribute("gameSessionId");
                out.print("{\"success\": true}");
                break;
        }
    }
} 