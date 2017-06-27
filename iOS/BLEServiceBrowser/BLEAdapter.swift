//
//  BLEAdapter.swift
//  BLEServiceBrowser
//
//  Created by Hiteshwar Vadlamudi on 11/30/15.
//  Copyright © 2015 bluetooth. All rights reserved.
//

import Foundation
import CoreBluetooth

let sharedInstance = BLEAdapter()

@objc public protocol BLEAdapterDelegate {
    func OnConnected(connectionStatus:Bool)
    func OnDiscoverService(services: NSArray)
    func AllCharacteristics(charcterArrays: NSArray)
}



@objc class BLEAdapter:NSObject, CBCentralManagerDelegate, CBPeripheralDelegate {
    
    var centralManager: CBCentralManager?
    var peripherals:NSMutableArray = NSMutableArray()
    var activePeripheral: CBPeripheral?
    var bleAdapterDelegate:BLEAdapterDelegate?


    override init() {
        super.init()
        let controlOptions:[String : AnyObject] = [CBCentralManagerOptionRestoreIdentifierKey:"BLEServiceBrowser"]
        self.centralManager = CBCentralManager(delegate: self, queue: nil, options:controlOptions)
        
    }
    
    func findBLEPeripherals(timeOut:Double)->Int {
        if(self.centralManager?.state != CBCentralManagerState.PoweredOn){
            return -1;
        }
        
        NSTimer.scheduledTimerWithTimeInterval(timeOut, target: self, selector: "connectionTimer", userInfo: nil, repeats: false)
        centralManager?.scanForPeripheralsWithServices(nil, options: nil)
        return 0
    }
    
    func connectPeripheral(peripheral: CBPeripheral, status: Bool) {
        if(status == true) {
            print("Connecting to peripheral with UUID : \(peripheral.identifier.UUIDString.utf8)\r\n" )
            activePeripheral = peripheral
            activePeripheral?.delegate = self
            centralManager?.connectPeripheral(activePeripheral!, options: nil)
        } else {
            activePeripheral = peripheral
            activePeripheral?.delegate = self
            centralManager?.cancelPeripheralConnection(self.activePeripheral!)
        }
    }
    
    func getAllCharacteristicsForService(p: CBPeripheral, service: CBService){
        p.discoverCharacteristics(nil, forService: service)
    }
    
    func connectionTimer() {
        centralManager?.stopScan()
        print("Stopped Scanning")
        print("Known Peripherals = \(self.peripherals.count)")
        self.printKnownPeripherals()
    }
    
    //----------------------------------------------------------------------------------------------------
    //
    //
    //CBCentralManagerDelegate protocol
    //
    //
    //----------------------------------------------------------------------------------------------------
    
    func centralManager(central: CBCentralManager, didDiscoverPeripheral peripheral: CBPeripheral, advertisementData: [String : AnyObject], RSSI: NSNumber) {
        
        for(var i = 0;i<self.peripherals.count; i++){
            let p:CBPeripheral = self.peripherals.objectAtIndex(i) as! CBPeripheral
            
            if(p.identifier.isEqual(peripheral.identifier)) {
                //Identifiers Match
                self.peripherals.replaceObjectAtIndex(i, withObject: peripheral)
                print("Duplicate UUID, Updating Peripherals Array")
                return;
            }else {
                if (p.name == peripheral.name){
                    self.peripherals.replaceObjectAtIndex(i, withObject: peripheral)
                    print("Duplicate UUID, Updating Peripherals Array")
                    return;
                }
            }
        }
        
        self.peripherals.addObject(peripheral)
        print("New Peripheral Adding to the List")
        
    }
    
    func centralManager(central: CBCentralManager, didConnectPeripheral peripheral: CBPeripheral) {
        print("Connection to peripheral with UUID : \(peripheral.identifier.UUIDString.utf8) successfull\r\n" );
        self.activePeripheral = peripheral;

        if(self.bleAdapterDelegate != nil){
            self.bleAdapterDelegate?.OnConnected(true)
        }
        self.activePeripheral?.discoverServices(nil)
    }
    
    func centralManager(central: CBCentralManager, didFailToConnectPeripheral peripheral: CBPeripheral, error: NSError?){
        print("Failed to connect to peripheral \(peripheral.identifier.UUIDString.utf8) \r\n");
        self.activePeripheral = peripheral;
        
        if(self.bleAdapterDelegate != nil){
            self.bleAdapterDelegate?.OnConnected(false)
        }
        self.activePeripheral?.discoverServices(nil)
    }
    
    func centralManager(central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral, error: NSError?) {
        print("Peripheral Got Disconnected \(peripheral.identifier.UUIDString.utf8) \r\n");

        if(self.bleAdapterDelegate != nil){
            self.bleAdapterDelegate?.OnConnected(false)
        }
    }
    
    //This is a REQUIRED Delagate Method of CBCentralManagerDeletage.
    
    func centralManagerDidUpdateState(central: CBCentralManager) {
        switch (central.state) {
        case CBCentralManagerState.PoweredOff:
            print("Status of CoreBluetooth Central Manager = Power Off")
            break
        case CBCentralManagerState.Unauthorized:
            print("Status of CoreBluetooth Central Manager = Does Not Support BLE")
            break
        case CBCentralManagerState.Unknown:
            print("Status of CoreBluetooth Central Manager = Unknown Wait for Another Event")
            break
        case CBCentralManagerState.PoweredOn:
            print("Status of CoreBluetooth Central Manager = Powered On")
            break
        case CBCentralManagerState.Resetting:
            print("Status of CoreBluetooth Central Manager = Resetting Mode")
            break
        case CBCentralManagerState.Unsupported:
            print("Status of CoreBluetooth Central Manager = Un Supported")
            break
        }
    }
    
    func centralManager(central: CBCentralManager, willRestoreState dict: [String : AnyObject]) {
        //
    }

    
    //----------------------------------------------------------------------------------------------------
    //
    //
    //CBPeripheralDelegate protocol methods beneth here
    //
    //
    //----------------------------------------------------------------------------------------------------
    func peripheral(peripheral: CBPeripheral, didDiscoverServices error: NSError?) {
        if (error == nil) {
            print("Services of peripheral with UUID : \(peripheral.identifier.UUIDString.utf8)found\r\n" );
            
            if(self.bleAdapterDelegate != nil){
                self.bleAdapterDelegate?.OnDiscoverService(peripheral.services!)
            }
        } else {
            print("Service discovery was unsuccessfull !\r\n");
        }
    }
    
    func peripheral(peripheral: CBPeripheral, didDiscoverCharacteristicsForService service: CBService, error: NSError?) {
        if ((error == nil)) {
            print("Characteristics of service with UUID : \(self.CBUUIDToString(service.UUID)) found\r\n");
            
            if(self.bleAdapterDelegate != nil) {
                self.bleAdapterDelegate?.AllCharacteristics(service.characteristics!)
            }
        } else {
            print("Characteristic discorvery unsuccessfull !\r\n");
        }
    }
    
    func peripheral(peripheral: CBPeripheral, didDiscoverDescriptorsForCharacteristic characteristic: CBCharacteristic, error: NSError?) {
        
    }
    
    func peripheral(peripheral: CBPeripheral, didDiscoverIncludedServicesForService service: CBService, error: NSError?) {
        
    }
    
    
    
    
    /*!
    *  @method printKnownPeripherals:
    *  @discussion printKnownPeripherals prints all curenntly known peripherals stored in the peripherals array
    */
    func printKnownPeripherals() {
        print("Prints All Known Peripherals ")
        for(var i = 0;i<self.peripherals.count; i++){
            
            let p:CBPeripheral = self.peripherals.objectAtIndex(i) as! CBPeripheral
            printPeripheralInfo(p)
            
        }
    }
    
    func GetServiceName(UUID: CBUUID)->String{
        
        let uuid = self.CBUUIDToInt(UUID) as UInt16
        switch(uuid)
        {
        case 0x1800: return "Generic Access"
        case 0x1801: return "Generic Attribute"
        case 0x1802: return "Immediate Alert"
        case 0x1803: return "Link Loss"
        case 0x1804: return "Tx Power"
        case 0x1805: return "Current Time Service"
        case 0x1806: return "Reference Time Update Service"
        case 0x1807: return "Next DST Change Service"
        case 0x1808: return "Glucose"
        case 0x1809: return "Health Thermometer"
        case 0x180A: return "Device Information"
        case 0x180B: return "Network Availability Service"
        case 0x180C: return "Watchdog"
        case 0x180D: return "Heart Rate"
        case 0x180E: return "Phone Alert Status Service"
        case 0x180F: return "Battery Service"
        case 0x1810: return "Blood Pressure"
        case 0x1811: return "Alert Notification Service"
        case 0x1812: return "Human Interface Device"
        case 0x1813: return "Scan Parameters"
        case 0x1814: return "RUNNING SPEED AND CADENCE"
        case 0x1815: return "Automation IO"
        case 0x1816: return "Cycling Speed and Cadence"
        case 0x1817: return "Pulse Oximeter"
        case 0x1818: return "Cycling Power Service"
        case 0x1819: return "Location and Navigation Service"
        case 0x181A: return "Continous Glucose Measurement Service"
        case 0x2A00: return "Device Name"
        case 0x2A01: return "Appearance"
        case 0x2A02: return "Peripheral Privacy Flag"
        case 0x2A03: return "Reconnection Address"
        case 0x2A04: return "Peripheral Preferred Connection Parameters"
        case 0x2A05: return "Service Changed"
        case 0x2A06: return "Alert Level"
        case 0x2A07: return "Tx Power Level"
        case 0x2A08: return "Date Time"
        case 0x2A09: return "Day of Week"
        case 0x2A0A: return "Day Date Time"
        case 0x2A0B: return "Exact Time 100"
        case 0x2A0C: return "Exact Time 256"
        case 0x2A0D: return "DST Offset"
        case 0x2A0E: return "Time Zone"
        case 0x2A0F: return "Local Time Information"
        case 0x2A10: return "Secondary Time Zone"
        case 0x2A11: return "Time with DST"
        case 0x2A12: return "Time Accuracy"
        case 0x2A13: return "Time Source"
        case 0x2A14: return "Reference Time Information"
        case 0x2A15: return "Time Broadcast"
        case 0x2A16: return "Time Update Control Point"
        case 0x2A17: return "Time Update State"
        case 0x2A18: return "Glucose Measurement"
        case 0x2A19: return "Battery Level"
        case 0x2A1A: return "Battery Power State"
        case 0x2A1B: return "Battery Level State"
        case 0x2A1C: return "Temperature Measurement"
        case 0x2A1D: return "Temperature Type"
        case 0x2A1E: return "Intermediate Temperature"
        case 0x2A1F: return "Temperature in Celsius"
        case 0x2A20: return "Temperature in Fahrenheit"
        case 0x2A21: return "Measurement Interval"
        case 0x2A22: return "Boot Keyboard Input Report"
        case 0x2A23: return "System ID"
        case 0x2A24: return "Model Number String"
        case 0x2A25: return "Serial Number String"
        case 0x2A26: return "Firmware Revision String"
        case 0x2A27: return "Hardware Revision String"
        case 0x2A28: return "Software Revision String"
        case 0x2A29: return "Manufacturer Name String"
        case 0x2A2A: return "IEEE 11073-20601 Regulatory Certification Data List"
        case 0x2A2B: return "Current Time"
        case 0x2A2C: return "Elevation"
        case 0x2A2D: return "Latitude"
        case 0x2A2E: return "Longitude"
        case 0x2A2F: return "Position 2D"
        case 0x2A30: return "Position 3D"
        case 0x2A31: return "Scan Refresh"
        case 0x2A32: return "Boot Keyboard Output Report"
        case 0x2A33: return "Boot Mouse Input Report"
        case 0x2A34: return "Glucose Measurement Context"
        case 0x2A35: return "Blood Pressure Measurement"
        case 0x2A36: return "Intermediate Cuff Pressure"
        case 0x2A37: return "Heart Rate Measurement"
        case 0x2A38: return "Body Sensor Location"
        case 0x2A39: return "Heart Rate Control Point"
        case 0x2A3A: return "Removable"
        case 0x2A3B: return "Service Required"
        case 0x2A3C: return "Scientific Temperature in Celsius"
        case 0x2A3D: return "String"
        case 0x2A3E: return "Network Availability"
        case 0x2A3F: return "Alert Status"
        case 0x2A40: return "Ringer Control Point"
        case 0x2A41: return "Ringer Setting"
        case 0x2A42: return "Alert Category ID Bit Mask"
        case 0x2A43: return "Alert Category ID"
        case 0x2A44: return "Alert Notification Control Point"
        case 0x2A45: return "Unread Alert Status"
        case 0x2A46: return "New Alert"
        case 0x2A47: return "Supported New Alert Category"
        case 0x2A48: return "Supported Unread Alert Category"
        case 0x2A49: return "Blood Pressure Feature"
        case 0x2A4A: return "HID Information"
        case 0x2A4B: return "Report Map"
        case 0x2A4C: return "HID Control Point"
        case 0x2A4D: return "Report"
        case 0x2A4E: return "Protocol Mode"
        case 0x2A4F: return "Scan Interval Window"
        case 0x2A50: return "PnP ID"
        case 0x2A51: return "Glucose Features"
        case 0x2A52: return "Record Access Control Point"
        case 0x2A53: return "RSC Measurement"
        case 0x2A54: return "RSC Feature"
        case 0x2A55: return "SC Control Point"
        case 0x2A56: return "Digital Input"
        case 0x2A57: return "Digital Output"
        case 0x2A58: return "Analog Input"
        case 0x2A59: return "Analog Output"
        case 0x2A5A: return "Aggregate Input"
        case 0x2A5B: return "CSC Measurement"
        case 0x2A5C: return "CSC Feature"
        case 0x2A5D: return "Sensor Location"
        case 0x2A5E: return "Pulse Oximetry Spot-check Measurement"
        case 0x2A5F: return "Pulse Oximetry Continuous Measurement"
        case 0x2A60: return "Pulse Oximetry Pulsatile Event"
        case 0x2A61: return "Pulse Oximetry Features"
        case 0x2A62: return "Pulse Oximetry Control Point"
        case 0x2A63: return "Cycling Power Measurement Characteristic"
        case 0x2A64: return "Cycling Power Vector Characteristic"
        case 0x2A65: return "Cycling Power Feature Characteristic"
        case 0x2A66: return "Cycling Power Control Point Characteristic"
        case 0x2A67: return "Location and Speed Characteristic"
        case 0x2A68: return "Navigation Characteristic"
        case 0x2A69: return "Position Quality Characteristic"
        case 0x2A6A: return "LN Feature Characteristic"
        case 0x2A6B: return "LN Control Point Characteristic"
        case 0x2A6C: return "CGM Measurement Characteristic"
        case 0x2A6D: return "CGM Features Characteristic"
        case 0x2A6E: return "CGM Status Characteristic"
        case 0x2A6F: return "CGM Session Start Time Characteristic"
        case 0x2A70: return "Application Security Point Characteristic"
        case 0x2A71: return "CGM Specific Ops Control Point Characteristic"
        default:
            return "Custom Profile";
            break;
        }
        return ""
    }

    //MARK: All Convenience Methods
    //TODO: Reconfirm the usage of CBUUID.
    func CBUUIDToInt(UUID: CBUUID)->UInt16 {
        var b1 = [UInt16](count: 16, repeatedValue: 0)
        UUID.data.getBytes(&b1, length: 16)
        let nUUID = (((UInt16)(b1[0]) << 8) | (UInt16)(b1[1]))
        return nUUID
    }
    
    func CBUUIDToString(UUID: CBUUID)->[CChar]?{
        return UUID.data.description.cStringUsingEncoding(NSUTF8StringEncoding)
    }
    
    func CBUUIDToNSString(UUID: CBUUID)->NSString {
        return UUID.data.description
    }
    
    func UUIDToString(UUID: NSUUID?)->[CChar]?{
        if(UUID == nil)
        {
            return  nil
        }
        return UUID!.UUIDString.cStringUsingEncoding(NSUTF8StringEncoding)
    }
    
    func compareCBUUID(UUID1: CBUUID, secondUUID  UUID2: CBUUID)->Int {
        var b1 = [UInt8](count: 16, repeatedValue: 0)
        UUID1.data.getBytes(&b1, length: 16)
        
        var b2 = [UInt8](count: 16, repeatedValue: 0)
        UUID2.data.getBytes(&b2, length: 16)
        
        if(memcmp(b1, b2, UUID1.data.length) == 0){
            return 1
        }else {
            return 0
        }
    }
    
    func compareCBUUIDToInt(UUID1: CBUUID, toInt UUID2: UInt16)->Int{
        let uuid1data = UUID1.data
        let uuid1count = uuid1data.length / sizeof(UInt8)
        var uuid1array = [UInt8](count: uuid1count, repeatedValue: 0)
        uuid1data.getBytes(&uuid1array, length: uuid1count * sizeof(UInt8))
        
        let b2: UInt16 = self.swap(UUID2)
        var b2Array = [b2 & 0xff, (b2 >> 8) & 0xff]
        if memcmp(&uuid1array, &b2Array, 2) == 0 {
            return 1
        }
        return 0
    }
   
    func swap(s: UInt16)->UInt16 {
        var temp = s << 8 as UInt16
        temp |= (s >> 8)
        return temp
    }

    func printPeripheralInfo(peripheral: CBPeripheral) {
        print("------------------------------------\r\n");
        print("Peripheral Info :\r\n");
        print("RSSI : \(peripheral.RSSI?.intValue)");
        print("Name : \(peripheral.name)");
        print("isConnected : \(peripheral.state)");
        print("-------------------------------------\r\n");
    }
}
