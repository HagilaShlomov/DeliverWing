
import React, { useState } from "react";
import { getUsers } from "../data/Users";
import { useNavigate } from "react-router-dom";

const Login = ({ onLogin }) => {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [message, setMessage] = useState("");
  const navigate = useNavigate();

  // const handleSubmit = (e) => {
  //   e.preventDefault();
  //   const users = getUsers();
  //   const user = users.find((u) => u.username === username && u.password === password);

  //   if (user) {
  //     onLogin();
  //     navigate("/add-order");
  //   } else {
  //     setMessage("שם משתמש או סיסמא לא נכונים.");
  //   }
  // };

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
        onLogin();
        navigate("/add-order");

      } else {
        setMessage("שם משתמש או סיסמא לא נכונים.");
      }
    } catch (error) {
      setMessage("שגיאה בהתחברות לשרת");
    }
  };
  


  return (
    <div>
      <h2>התחברות</h2>
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
        <button type="submit">התחבר</button>
      </form>
      <p>{message}</p>
    </div>
  );
};

export default Login;