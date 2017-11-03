# mesh-core-on-nrf

Welcome!

This is a Bluetooth mesh SDK, integrating on nrf52832.

Bluetooth Mesh provides a complete full-stack connectivity solution for mesh networking, where everything from radio aspects to the high-level service definition layer is fully specified.

MeshCore provides a complete stack of Bluetooth mesh from bearer layer to access layer, also integrates the founditional model.
MeshCore releases easy application interfaces for network establishment and management so that developers could focus on the application-level.

Current SDK version is v0.95, following Bluetooth Mesh Profile v1.0.
Features:
*	PB-ADV and PB-GATT
*	Relay
*	Proxy
*	Friendship
*	Low power node
*	Provision without OOB, with Input OOB, Output OOB and static OOB
*	Runtime parameter configurable

To get lastest news of MeshCore, see blog http://blog.csdn.net/blxble.
To get Bluetooth Mesh Profile specification, visit https://www.bluetooth.com.

Bluetooth qulification test for MeshCore is ongoing...

Integration on other platforms is ongoing...


Envirnment and tools for demo:
1. a NRF52832 module
2. keil v5
3. NRF5 SDK s132 v12.3

Note that:
1. some of the configuration of current released library is fixed.
2. low power settings of nrf52832 are excluded in the demo
