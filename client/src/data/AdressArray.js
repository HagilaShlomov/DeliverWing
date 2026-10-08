const ADRESS_ORDER = "AdressArray";

export const getAdress = () => {
  const address = localStorage.getItem(ADRESS_ORDER);
  return address ? JSON.parse(address) : []; 
};

export const addAdress = (newAdress) => {
  const adress = getAdress();
  adress.push(newAdress);
  localStorage.setItem(ADRESS_ORDER, JSON.stringify(adress));
};