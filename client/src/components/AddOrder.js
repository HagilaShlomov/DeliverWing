// AddOrder.js
import React, { useState, useEffect } from "react";
import { getAdress, addAdress } from "../data/AdressArray";
import { MapContainer, TileLayer, Marker, useMapEvents } from "react-leaflet";
import 'leaflet/dist/leaflet.css';
import L from 'leaflet';

// Fix for default icon issue with React-Leaflet
delete L.Icon.Default.prototype._getIconUrl;//שימוש בספריית Leaflet להצגת מפה.


L.Icon.Default.mergeOptions({
  iconRetinaUrl: 'https://unpkg.com/leaflet@1.7.1/dist/images/marker-icon-2x.png',
  iconUrl: 'https://unpkg.com/leaflet@1.7.1/dist/images/marker-icon.png',
  shadowUrl: 'https://unpkg.com/leaflet@1.7.1/dist/images/marker-shadow.png',
});

function LocationPicker({ position, setPosition }) {//קומפוננטת עזר שמאזינה ללחיצה על המפה ומציבה Marker במיקום הלחיצה. setPosition מעדכן את היעד.
  useMapEvents({
    click(e) {
      setPosition([e.latlng.lat, e.latlng.lng]);
    }
  });
  return position ? <Marker position={position} /> : null;
}

async function geocodeAddress(address) {//פונה ל-OpenStreetMap Nominatim.
  const url = `https://nominatim.openstreetmap.org/search?format=json&q=${encodeURIComponent(address)}`;
  const res = await fetch(url, {
    headers: {
      'Accept-Language': 'he'
    }
  });
  const data = await res.json();
  if (data.length === 0) {
    throw new Error("כתובת לא נמצאה. אנא ודא שהכתובת מלאה ותקינה (רחוב, מספר בית, עיר).");
  }
  return [parseFloat(data[0].lat), parseFloat(data[0].lon)];
}

const AddOrder = () => {
  const [formData, setFormData] = useState({
    city: "",
    street: "",
    houseNumber: "",
    itemCount: "",
    itemWeight: "",
  });
  const [addresses, setAddresses] = useState([]);
  const [destination, setDestination] = useState(null);

  const HARDCODED_SOURCE_ADDRESS = "הרצל 17, נתניה, ישראל";
  const [sourceCoordinates, setSourceCoordinates] = useState(null);

  useEffect(() => {
    setAddresses(getAdress());
    async function getHardcodedSourceCoords() {
      try {
        const coords = await geocodeAddress(HARDCODED_SOURCE_ADDRESS);
        setSourceCoordinates(coords);
      } catch (err) {
        console.error(`שגיאה בגיאוקוד כתובת המקור הקבועה (${HARDCODED_SOURCE_ADDRESS}):`, err.message);
        alert(`שגיאה בטעינת כתובת המקור הקבועה. אנא וודא שהיא תקינה: ${HARDCODED_SOURCE_ADDRESS}`);
      }
    }
    getHardcodedSourceCoords();
  }, []);

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

    const fullAddress = `${formData.street} ${formData.houseNumber}, ${formData.city}`;
    if (formData.city.trim() === "" || formData.street.trim() === "" || formData.houseNumber.trim() === "") {
      alert("אנא מלא כתובת יעד מלאה (עיר, רחוב, מספר בית).");
      return;
    }

    let currentDestinationCoords = null;
    try {
      currentDestinationCoords = await geocodeAddress(fullAddress);
      setDestination(currentDestinationCoords); // Update map with new destination
    } catch (err) {
      alert(`שגיאה בחיפוש כתובת היעד: ${err.message}. אנא ודא שהכתובת תקינה.`);
      return;
    }

    if (!sourceCoordinates) {
        alert("המערכת טוענת את כתובת המקור, אנא המתן או נסה שוב.");
        return;
    }

    const orderData = {
      ...formData,
      source: sourceCoordinates,
      destination: currentDestinationCoords
    };

    addAdress(formData);
    const updatedAddresses = getAdress();
    setAddresses(updatedAddresses);

    // Clear form data for next entry
    setFormData({
      city: "",
      street: "",
      houseNumber: "",
      itemCount: "",
      itemWeight: "",
    });
    // ***** שינוי: הוסר setDestination(null) כדי להשאיר את היעד מוצג במפה לאחר השליחה *****


    try {
      console.log("📤 Sending data:", JSON.stringify(orderData, null, 2));

      const response = await fetch("http://localhost:8080/AddOrder", {
        method: "POST",
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(orderData),
      });

      const data = await response.json();
      console.log("Response from server:", data);

      if (response.ok) {
        alert("ההזמנה נשלחה בהצלחה!");
      } else {
        const errorText = await response.text();
        alert(`שגיאה בשליחת ההזמנה לשרת: ${errorText}`);
      }
    } catch (error) {
      console.error("Error:", error);
      alert("אירעה שגיאה בעת שליחת ההזמנה.");
    }
  };

  return (
    <div id="getOrder" style={{ display: 'flex', gap: '20px', maxWidth: '1200px', margin: 'auto' }}>
      <div style={{ flex: 1 }}>
        <h2>הוספת הזמנה</h2>
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
          <hr/>

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
            placeholder="משקל פריטים (ק''ג)"
          />
          <br></br>
          <button type="submit">אישור הזמנה וצפייה במפה</button>
        </form>

        <h3>כתובות שנוספו לאחרונה</h3>
        <div className="addresses-container">
          {getAdress()
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

      <div style={{ flex: 1.5, display: 'flex', flexDirection: 'column' }}>
        <label>מיקומי מקור ויעד על המפה:</label>
        <MapContainer
          center={destination || sourceCoordinates || [32.0853, 34.7818]}
          zoom={destination ? 16 : (sourceCoordinates ? 14 : 10)}
          style={{ height: '500px', marginBottom: '1rem', width: '100%' }}
        >
          <TileLayer url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png" />
          {sourceCoordinates && <Marker position={sourceCoordinates} title="נקודת יציאה (קבועה)" />}
          <LocationPicker position={destination} setPosition={setDestination} />
        </MapContainer>
        {/* ***** שינוי: הוספתי בדיקה ל-destination לפני ההדפסה ***** */}
        {destination && <p>יעד נבחר: {destination[0].toFixed(5)}, {destination[1].toFixed(5)}</p>}
        {sourceCoordinates && <p>מקור קבוע: {sourceCoordinates[0].toFixed(5)}, {sourceCoordinates[1].toFixed(5)}</p>}
      </div>
    </div>
  );
};

export default AddOrder;