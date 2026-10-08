import React, { useState } from "react";
import { getUsers, addUser } from "../data/Users";
import { Link, useNavigate } from "react-router-dom";

const Signup = () => {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [message, setMessage] = useState("");
  const navigate = useNavigate();

const handleSubmit = async (e) => {
  e.preventDefault();

  try {
    const response = await fetch("http://localhost:8080/login", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({ username, password }),
    });

    const data = await response.json();
    console.log("Response from server:", data);

    if (response.ok) {
      navigate("/add-order");

    } else {
      setMessage("שם משתמש או סיסמא לא נכונים.");
    }
  } catch (error) {
    setMessage("שגיאה בהתחברות לשרת");
  }
















    const users = getUsers();
    const userExists = users.some((u) => u.username === username);

    if (!username || !password) {
      setMessage("נא למלא את כל השדות.");
      return;
    }
    if (userExists) {
      setMessage("שם המשתמש כבר קיים.");
    } else {
      const newUser = { id: users.length + 1, username, password };
      addUser(newUser);
      setMessage("נרשמת בהצלחה! אפשר להתחבר עכשיו.");
      setTimeout(() => navigate("/login"), 1500);
    }
  };

  return (
    <div>
      <h2>הרשמה</h2>
      <form onSubmit={handleSubmit}>
        <input
          type="text"
          placeholder="שם משתמש"
          value={username}
          onChange={(e) => setUsername(e.target.value)}
        />
        <input
          type="password"
          placeholder="סיסמא"
          value={password}
          onChange={(e) => setPassword(e.target.value)}
        />
        <button type="submit">הרשם</button>
      </form>
      <p>{message}</p>
      <p>כבר רשום? <Link to="/login">היכנס כאן</Link></p>
    </div>
  );
};

export default Signup;
