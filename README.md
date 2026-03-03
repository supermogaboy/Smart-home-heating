# Smart-home-heating
smart home heating system built on esp 32 and raspberry pi.

Overview

This system collects indoor climate data and furnace heating activity to analyze and model home thermal behavior.

Temperature Nodes: ESP32-C6 + AHT20 sensors measure temperature and humidity and transmit data via ESP-NOW.

Thermostat Detection Node: Uses a bridge rectifier and optocoupler to safely detect 24 VAC heat calls on the W line.

ESP Hub: Aggregates node data over ESP-NOW and forwards packets to a server via MQTT.

Backend Server: Stores data and injects outdoor temperature from an external source.

Data Format

Each record includes:

nodeID
temperature
humidity
outdoorTemp
timestamp
heatingState
Goal

Use collected data to model the home’s thermal dynamics and develop a predictive, ML-based thermostat optimized for the specific building.

Long term goals:

make my own thermostat using raspberry pi. 
make webapp ui that user can interface with at home.
