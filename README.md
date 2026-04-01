# Intelligent Vehicle Dashboard Controller with Real-Time IoT Monitoring and Safety Alert

## Overview

This project focuses on developing an intelligent vehicle dashboard system using a dual-microcontroller architecture. It integrates embedded control and IoT to enhance vehicle safety, provide automation, and enable real-time monitoring of critical parameters.

## System Architecture

The system is built using two microcontrollers:

* PIC microcontroller (used in Master–Slave configuration)
* ESP32 (used for IoT and cloud communication)

The complete system is divided into two main sections:

1. Dashboard Control System
2. Cabin Control System

## Dashboard Control System

Multiple vehicle parameters are continuously monitored using sensors, including:

* Engine temperature
* Seatbelt status
* Door status (all four doors)
* Battery percentage
* Fuel level
* Vehicle speed

The master controller collects all sensor data and transmits it to the slave controller using SPI communication. The slave acts as the processing and decision-making unit, where all received data is compared with predefined threshold values.

Based on the conditions:

* Low fuel condition triggers a buzzer alert and dashboard indication
* Seatbelt not worn generates a warning alert
* Door status is monitored for safety
* Battery level and vehicle speed are tracked continuously

All safety-related actions and alerts are handled by the slave controller.

## Communication System

* SPI communication is used between master and slave for reliable and fast data transfer
* UART communication is used to send important filtered data from the slave controller to the ESP32

The ESP32 uploads this data to the cloud, allowing real-time monitoring of vehicle status from anywhere. Any changes in parameters can be tracked instantly.

## Cabin Control System

The system also includes an automatic cabin temperature control mechanism. The master controller continuously monitors cabin temperature:

* If the temperature exceeds the set threshold → AC turns ON
* If the temperature falls below the threshold → AC turns OFF

This ensures a comfortable cabin environment without manual intervention.

## Features

* Dual microcontroller architecture (PIC + ESP32)
* Real-time monitoring of vehicle parameters
* Automatic safety alerts and dashboard indications
* SPI-based master-slave communication
* UART communication with ESP32 for IoT integration
* Cloud-based real-time vehicle tracking
* Automatic cabin temperature control

## Technologies Used

* Embedded C
* SPI Communication
* UART Communication
* ESP32 (IoT integration)
* Sensors and actuators

## Applications

* Smart vehicle dashboard systems
* Automotive safety and monitoring systems
* IoT-based vehicle tracking solutions

## Conclusion

This project demonstrates the practical implementation of an intelligent vehicle dashboard system by combining embedded systems and IoT. The use of a dual-microcontroller architecture improves system reliability, enables automation, and provides real-time monitoring, making it suitable for modern automotive applications.

## Circuit Diagram
![Circuit](Circuit/circuit.png)

## Output
![Output](Circuit/output.png)
