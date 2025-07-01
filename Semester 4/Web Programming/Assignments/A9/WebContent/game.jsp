<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
    String username = (String) session.getAttribute("username");
    if (username == null) {
        response.sendRedirect("login.jsp");
        return;
    }
%>
<html>
<head>
    <title>Snake Game</title>
    <style>
        #gameCanvas { border: 1px solid #000; }
    </style>
</head>
<body>
<h2>Welcome, <%= username %>! Play Snake</h2>
<canvas id="gameCanvas" width="400" height="400"></canvas>
<script>
// Placeholder for Snake game logic (to be implemented)
</script>
</body>
</html> 