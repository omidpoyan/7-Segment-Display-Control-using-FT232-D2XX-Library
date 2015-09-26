##Code for Controlling a 7 segment display using FT232 USB to Serial Converter chip and D2XX library.
<img src = "http://www.xanthium.in/sites/default/files/site-images/ft232-7-segment-led-interface-d2xx/Lt543-7-segment-interfaced-ft232.jpg" alt = "image of FT232 usb to serial converter chip controlling a 7 segment display using D2XX library"/>

The control codes for 7 Segment display are send by a C program running on Windows PC serially into the FT232, which is then used to control the 7 segment display without using any microcontroller.

<a href ="http://xanthium.in/interfacing-7-segment-led-display-with-ft232-and-d2xx-library">Full Explanation of the hardware circuit and code can be found here</a>. 

A short video of FT232 controlling the 7 segment display is <a href ="https://www.youtube.com/watch?v=SaTPUVlcq1g">available on youtube </a>

The code runs on a Windows (Windows7) PC and uses the **D2XX library** from FTDI.It is compiled using **GCC**.

D2XX library is used to put the FT232 chip into **asynchronous Bitbang mode**.This will effectively convert the USB to Serial converter to a USB to Parallel converter with 8 IO channels.The 8 IO's are then used to control a common cathode 7 segment display.<a href ="http://xanthium.in/interfacing-7-segment-led-display-with-ft232-and-d2xx-library">Full explanation can be found here...</a> 

More info about the  <a href = "http://xanthium.in/USB-to-Serial-RS232-RS485-Converter">USB to Serial/RS232/RS485 Converter used in the above tutorial can be found here</a>

<img src = "http://s25.postimg.org/ucb73bztr/usb_to_rs485_converter_250px.jpg"  href="http://xanthium.in/USB-to-Serial-RS232-RS485-Converter"/>

