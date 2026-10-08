import React, { useState, useEffect } from "react"; 
import { getAdress, addAdress } from "../data/AdressArray"; 
// import {PathCanvas} from "./PathCanvas" 

const drawPathOnCanvas = (points) => { 
  const canvas = document.getElementById("pathCanvas"); 
  if (!canvas) { 
    console.error("❌ הקנבס לא נמצא"); 
    return; 
  } 
  const ctx = canvas.getContext("2d"); 

  // ניקוי הקנבס לפני ציור חדש 
  ctx.clearRect(0, 0, canvas.width, canvas.height); 

  // הגדרת צבע המסלול 
  ctx.strokeStyle = "blue"; 
  ctx.lineWidth = 2; 

  // ציור המסלול 
  if (points.length > 1) { 
    ctx.beginPath(); 
    ctx.moveTo(points[0].x, points[0].y); 
    points.forEach((point) => { 
      ctx.lineTo(point.x, point.y); 
    }); 
    ctx.stroke(); 
  } 
}; 

const AddOrder = () => { 
  const [formData, setFormData] = useState({ 
    city: "", 
    street: "", 
    houseNumber: "", 
    itemCount: "", 
    itemWeight: "", 
  }); 
  const [addresses, setAddresses] = useState([]); 

  useEffect(() => { 
    setAddresses(getAdress()); 
  }, [formData]); 

  const handleChange = (e) => { 
    const { name, value } = e.target; 
    setFormData({ 
      ...formData, 
      [name]: 
        name === "houseNumber" || name === "itemCount" || name === "itemWeight" 
          ? value.replace(/\D/g, "") 
          : value, 
    }); 
  }; 

  const handleSubmit = async (e) => { 
    e.preventDefault(); 
    addAdress(formData); // מוסיף את הכתובת ל- localStorage 
    const updatedAddresses = getAdress(); // טוען מחדש את הנתונים מה- localStorage 
    setAddresses(updatedAddresses); // מעדכן את ה-state המקומי 

    setFormData({ 
      city: "", 
      street: "", 
      houseNumber: "", 
      itemCount: "", 
      itemWeight: "", 
    }); 
    try { 
      console.log("📤 Sending data:", JSON.stringify(formData, null, 2)); 

      const response = await fetch("http://localhost:8080/AddOrder", { 
        method: "POST", 
        headers: { 
          "Content-Type": "application/json", 
        }, 
        body: JSON.stringify(formData), 
      }); 

      const data = await response.json(); 
      console.log("Response from server:", data); 

      if (response.ok) { 
        console.log("Response from server:", data.city); 
      } 
      if (data.path) { 
        drawPathOnCanvas(data.path); 
      } 
    } catch (error) { 
      console.error("Error:", error); 
    } 
  }; 

  return ( 
    <div id="getOrder"> 
      
      <div> 
        <h2>הוספת כתובת</h2> 
        <form onSubmit={handleSubmit}> 
          <input 
            type="text" 
            name="city" 
            value={formData.city} 
            onChange={handleChange} 
            required 
            placeholder="עיר" 
          /> 
          <input 
            type="text" 
            name="street" 
            value={formData.street} 
            onChange={handleChange} 
            required 
            placeholder="רחוב" 
          /> 
          <input 
            type="text" 
            name="houseNumber" 
            value={formData.houseNumber} 
            onChange={handleChange} 
            required 
            placeholder="מספר בית" 
          /> 
          <input 
            type="text" 
            name="itemCount" 
            value={formData.itemCount} 
            onChange={handleChange} 
            required 
            placeholder="מספר פריטים" 
          /> 
          <input 
            type="text" 
            name="itemWeight" 
            value={formData.itemWeight} 
            onChange={handleChange} 
            step="0.01" 
            required 
            placeholder="משקל פריטים" 
          /> 
          <br></br> 
          <button type="submit">אישור</button> 
        </form> 

        <h3>כתובות שנוספו לאחרונה</h3> 
        <div className="addresses-container"> 
          {getAdress() 
            // .sort((a, b) => a.city.localeCompare(b.city) || a.street.localeCompare(b.street)) 
            .map((address, index) => ( 
              <div key={index} className="address-card"> 
                <p> 
                  {address.city}, {address.street} {address.houseNumber},{" "} 
                  {address.itemCount} פריטים ({address.itemWeight} ק"ג) 
                </p> 
              </div> 
            ))} 
        </div> 
      </div> 
      <div> 
        <canvas 
          id="pathCanvas" 
          width="500" 
          height="500" 
          style={{ border: "1px solid black" }} 
        ></canvas> 
      </div> 
    </div> 
  ); 
}; 

export default AddOrder;
