# Heat-Wave-Mechanix
--------------------
List of components used-
1. DHT 11 sensor (for temp & humidity measuring)
2. Esp-wroom-32 module microcontroller
3. A 16 pin LCD display
4. Dual Chanel Relay (as an electric switch to turn on and off the heating element)
5. Thermoelectric Peltier Module with heat Sink (Hating Element)
--------------------
Main Logic Meaning-
- The Dht sensor is connected with esp32 and Heating element is also connected with esp32 with the help of 12v Relay. When the temperature of the goes below X°C then esp32 will tell relay to start switch and this will start heating element.
- When the temperature exceeds Y°C then the heating element will be stopped with the help of same above logic.
- With the help of LCD display temperature and humidity will be displayed.
- As esp32 is used remote monitoring and controlling is possible.
--------------------
Prototype video link(Youtube)- https://youtu.be/5nkeaPN5dII
--------------------
