import React, { useState } from 'react';
import 'leaflet/dist/leaflet.css';
import { MapContainer, TileLayer, FeatureGroup, Rectangle, Popup } from 'react-leaflet';

export default function Map() {
    const rectangle = [
        [11.271572, 77.604363],
        [11.271293, 77.604834],
    ];
    const [paymentLog, setPaymentLog] = useState([]);

    const handleMouseDown = (amount, imgSrc, carNumber) => {
        if (paymentLog.length >= 3) {
            // If payment log has 3 or more entries, remove the oldest one (the first item)
            setPaymentLog(prevLog => prevLog.slice(1).concat({ time: getCurrentTime(), amount, imgSrc, carNumber }));
        } else {
            // Otherwise, simply add a new entry
            setPaymentLog(prevLog => [...prevLog, { time: getCurrentTime(), amount, imgSrc, carNumber }]);
        }
    };

    const clearPaymentLog = () => {
        setPaymentLog([]);
    };

    const purpleOptions = { color: 'purple' };
    const getCurrentTime = () => {
        const now = new Date();
        const hours = now.getHours().toString().padStart(2, '0');
        const minutes = now.getMinutes().toString().padStart(2, '0');
        const seconds = now.getSeconds().toString().padStart(2, '0');
        return `${hours}:${minutes}:${seconds}`;
    };

    // Generate random values for demonstration
    const getRandomCarNumber = () => {
        return `TN${Math.floor(Math.random() * 100)} DX${Math.floor(Math.random() * 10000)}`;
    };

    const getRandomGPSStatus = () => {
        return  'Online' ;
    };

    const getRandomTollFee = () => {
        return `${Math.floor(Math.random() * 70)}rs`;
    };

    // Calculate total amount
    const totalAmount = paymentLog.reduce((total, entry) => total + parseInt(entry.amount), 0);

    return (
        <div>
            <div className='m-2 select-none bg-black rounded-md text-white'>
                <p className='text-[70px] text-center font-bold select-none' onMouseDown={() => handleMouseDown(getRandomTollFee(), 'https://i.pinimg.com/564x/cb/db/e2/cbdbe2838e914d52ae6f41392cdfdb70.jpg', getRandomCarNumber())}>
                    GPS BASED TOLL SYSTEM
                </p>
            </div>
            <div className='flex'>
                <div className='m-2 w-1/2  h-[320px] bg-black rounded-lg  text-white'>
                    <MapContainer center={[11.271115, 77.604569]} zoom={15} scrollWheelZoom={true} className='rounded-lg h-[320px]' >
                        <TileLayer
                            attribution='&copy; <a href="https://raghavan2005.netlify.app/">420 Nithin</a> '
                            url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
                        />
                        <FeatureGroup pathOptions={purpleOptions}>
                            <Popup>Toll Area</Popup>
                            <Rectangle bounds={rectangle} />
                        </FeatureGroup>
                    </MapContainer>
                </div>
                <div className='m-2 p-3 w-1/2 h-[320px] bg-black rounded-md text-white'>
                    <div>
                        <div className=''>
                            <div className='bg-gray-500 flex rounded-lg text-center font-bold p-2 w-full text-[20px]'>
                                <div>Payment Log</div>
                                <div className='mx-auto mr-10 text-[15px] pt-1  bg-red-500 p-2 rounded-md hover:bg-red-600' onClick={clearPaymentLog}>
                                    Clear all
                                </div>
                            </div>
                        </div>
                        {paymentLog.map((entry, index) => (
                            <div key={index} className='bg-green-500 m-2 rounded-lg text-center font-bold p-1 text-[15px]'>
                                <div>Time: {entry.time}</div>
                                <div>Amount: {entry.amount}</div>
                               
                                <div>Car No: {entry.carNumber}</div>
                            </div>
                        ))}
                    </div>
                </div>
            </div>
            <div className='m-2 h-[250px] bg-black rounded-md text-white flex'>
                <img src='https://i.pinimg.com/564x/cb/db/e2/cbdbe2838e914d52ae6f41392cdfdb70.jpg' className='w-[220px] h-[220px] rounded-lg m-5 '/>
                <div className='p-5 text-[30px] text-center pt-14'>
                  <div className='bg-white text-black text-[25px] p-3 rounded-md hover:bg-slate-200 select-none' onClick={()=>{console.log("hello")}}> Click Me</div>
                </div>
                </div>
        </div>
    );
}
