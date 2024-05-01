import { useState } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import Map from './Map'

function App() {
  const [count, setCount] = useState(0)

  return (
    <div className=' mt-8'>
    <div className=' m-5 '>
      <Map/>
      <div className='text-black text-end m-5 opacity-35'>RG</div>
    </div>
    </div>
  )
}

export default App
