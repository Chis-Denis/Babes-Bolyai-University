<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>Register - Snake Game</title>
</head>
<body>
<h2>Register</h2>
<form method="post" action="register">
    Username: <input type="text" name="username" required><br>
    Password: <input type="password" name="password" required><br>
    <input type="submit" value="Register">
</form>
<p>Already have an account? <a href="login.jsp">Login here</a></p>
<% String error = request.getParameter("error");
   if (error != null) { %>
    <p style="color:red;">Username already exists.</p>
<% } %>
</body>
</html> 