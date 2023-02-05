# IOT-Based-Automatic-Bottle-Filling-and-Capping-System
IOT Based Automatic Bottle Filling and Capping System - University Project

Using two mircocontroller Arduino and ESP8266, Arduino to control the process of detecting bottle through IR sensor for both filling and capping process. ESP8266 is used to read data from Arduino pins to get the status(such as bottle counts, status of filling and cappting motors) of the process and display them on the web server hosted by ESP8266 on local network 

### Refer [Project Report](https://github.com/paulbobin/IOT-Based-Automatic-Bottle-Filling-and-Capping-System/blob/main/Project%20Report.pdf) for detail information

## Bottle Filling Output
![Picture 25](https://user-images.githubusercontent.com/31591304/216818734-fd811cef-5686-4b2b-af78-cad6170ee212.jpg)
Fig. Side view of Conveyor and filling system

Here the conveyor and its support is shown along with guide rails used to guide the bottle straight to the nozzle, IR sensor is placed in perpendicular to the water nozzle. LCD Display is placed at the edge of the conveyor platform.

![FullSizeRender](https://user-images.githubusercontent.com/31591304/216818750-b5490daa-cc2d-4e3d-b1be-0c08b8b18008.jpg)
Fig. Top view of Arduino and other components

Here top view of the system shows the water pump and tube connection, Geared DC motor is connected to conveyor with a coupler. Arduino is connected with all sensor modules wires and LCD.



## Bottle Capping Output
![20210531_112134_HDR~3](https://user-images.githubusercontent.com/31591304/216818864-90e27247-37a0-4b69-b4fa-e24d0034cc42.jpg)
Fig. Side view of capping system

Here the capping motor is in resting position i.e. 45°, holding the capping DC motor. Relay is in OFF state supply by 12v. Plastic header is a mould of bottle cap in which is used to rotate bottle cap.
![Picture 30](https://user-images.githubusercontent.com/31591304/216818874-49cdf054-00bf-4cb9-9333-4905207677c9.jpg)
Fig. Side view of capping process

Here the servo motor turns down to 90°, relay is turn ON therefore the DC motor starts rotates.

## HMI Output  
![HMI](https://user-images.githubusercontent.com/31591304/216818905-119b8448-2500-4fb9-b407-92d41869e9f2.png)
Fig. Web view of HMI

Thus the output of HMI of the system controlled and monitored by web after connected to the network. It consist of monitoring of system includes bottle detection, water pump and capping motor state ,time conveyor status and counter. Control of system is to start and stop the process with START/STOP buttons. Network details are also shown there.
