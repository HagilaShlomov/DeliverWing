const USERS_KEY = "userArray";

// פונקציה לשליפת המשתמשים מה-LocalStorage
export const getUsers = () => {
  const users = localStorage.getItem(USERS_KEY);
  return users ? JSON.parse(users) : []; // אם יש נתונים, נמיר אותם למערך, אחרת נחזיר מערך ריק
};

// פונקציה להוספת משתמש חדש ושמירתו ב-LocalStorage
export const addUser = (newUser) => {
  const users = getUsers();
  users.push(newUser);
  localStorage.setItem(USERS_KEY, JSON.stringify(users));
};