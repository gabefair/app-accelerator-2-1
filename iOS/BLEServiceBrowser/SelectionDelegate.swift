//
//  SelectionDelegate.swift
//  BLEServiceBrowser
//
//  Created by Hiteshwar Vadlamudi on 11/30/15.
//  Copyright © 2015 bluetooth. All rights reserved.
//

import Foundation
import CoreBluetooth

protocol SelectionDelegate
{
    func ClearUI(clear: Bool)
    func selectedPeripheral(peripheral: CBPeripheral)
    
    func selectedService(service: CBService)
    func selectedCharacteristic(characteristic: CBCharacteristic)
    
    
}