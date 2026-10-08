import Login from './components/Login';
import Signup from './components/Signup';
import AddOrder from './components/AddOrder';
import React, { useState, useEffect } from "react";
import { BrowserRouter as Router, Routes, Route, Navigate, Link, useLocation, useNavigate } from "react-router-dom";
import "./App.css";

const App = () => {
  const [isAuthenticated, setIsAuthenticated] = useState(localStorage.getItem("isAuthenticated") === "true");

  useEffect(() => {
    localStorage.setItem("isAuthenticated", isAuthenticated);
  }, [isAuthenticated]);

  return (
    <Router>
      <div className='App'>
        <Navigation isAuthenticated={isAuthenticated} setIsAuthenticated={setIsAuthenticated} />
        <Routes>
          <Route path="/signup" element={<Signup />} />
          <Route path="/login" element={<Login onLogin={() => setIsAuthenticated(true)} />} />
          <Route path="/add-order" element={isAuthenticated ? <AddOrder /> : <Navigate to="/signup" />} />
          <Route path="*" element={<NavigateToLastPage isAuthenticated={isAuthenticated} />} />
        </Routes>
      </div>
    </Router>
  );
};

const Navigation = ({ isAuthenticated, setIsAuthenticated }) => {
  const location = useLocation();
  const navigate = useNavigate(); // ייבוא useNavigate כדי לאפשר ניווט או רענון
  
  useEffect(() => {
    localStorage.setItem("lastPath", location.pathname);
  }, [location]);

  // פונקציה שתטפל בלחיצה על הלינק "הוספת הזמנה"
  const handleAddOrderClick = (e) => {
    if (location.pathname === "/add-order") {
      e.preventDefault(); // מונע את פעולת ברירת המחדל של הלינק
      window.location.reload(); // רענון מלא של הדף
    } else {
      navigate("/add-order");
    }
  };

  return (
    <div className="menu">
      {!isAuthenticated ? (
        <>
          <Link to="/login" className="link">התחברות</Link>
          <Link to="/signup" className="link">הרשמה</Link>
        </>
      ) : (
        <>
          {/* שימוש בפונקציה handleAddOrderClick עבור הלינק "הוספת הזמנה" */}
          <Link to="/add-order" className="link" onClick={handleAddOrderClick}>הוספת הזמנה</Link>
          <button onClick={() => {
            setIsAuthenticated(false);
            localStorage.removeItem("isAuthenticated");
            localStorage.removeItem("lastPath");
          }} className="link">התנתקות</button>
        </>
      )}
    </div>
  );
};

const NavigateToLastPage = ({ isAuthenticated }) => {
  const navigate = useNavigate();

  useEffect(() => {
    const lastPath = localStorage.getItem("lastPath") || (isAuthenticated ? "/add-order" : "/signup");
    navigate(lastPath, { replace: true });
  }, [isAuthenticated, navigate]);

  return null;
};

export default App;
