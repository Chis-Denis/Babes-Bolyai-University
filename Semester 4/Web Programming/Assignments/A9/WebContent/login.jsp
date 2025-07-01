<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>Login - Snake Game</title>
</head>
<body>
<h2>Login</h2>
<form method="post" action="login">
    Username: <input type="text" name="username" required><br>
    Password: <input type="password" name="password" required><br>
    <input type="submit" value="Login">
</form>
<p>Don't have an account? <a href="register.jsp">Register here</a></p>
<% String error = request.getParameter("error");
   if (error != null) { %>
    <p style="color:red;">Invalid username or password.</p>
<% } %>
</body>
</html> 