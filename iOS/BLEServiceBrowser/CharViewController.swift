//
//  CharViewController.swift
//  BLEServiceBrowser
//
//  Created by Hiteshwar Vadlamudi on 12/1/15.
//  Copyright © 2015 bluetooth. All rights reserved.
//

import Foundation
import UIKit
import CoreBluetooth

class CharViewController: UIViewController, UITableViewDataSource, UITableViewDelegate, SelectionDelegate, UISplitViewControllerDelegate {
    
    var peripheral: CBPeripheral?
    var adapter: BLEAdapter?
    var characteristic: CBCharacteristic?
    var masterPopoverController: UIPopoverController?
    
    @IBOutlet weak var tableView: UITableView!
    @IBOutlet weak var WriteCommandButton: UIButton!
    @IBOutlet weak var WriteButton: UIButton!
    
    @IBOutlet weak var ReadButton: UIButton!
    @IBOutlet weak var NotificationSwitch: UISwitch!
    @IBOutlet weak var IndicationSwitch: UISwitch!
    @IBOutlet weak var textBoxHex: UITextField!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        let appDelegate = UIApplication.sharedApplication().delegate as! AppDelegate
        appDelegate.setCharViewDelegate(self)
        
        adapter = sharedInstance
        peripheral = adapter?.activePeripheral
        let properties = characteristic?.properties
        
        self.ReadButton.enabled = ((properties?.rawValue)! & CBCharacteristicProperties.Read.rawValue) == 1 ? true: false
        self.WriteButton.enabled = ((properties?.rawValue)! & CBCharacteristicProperties.Write.rawValue) == 1 ? true: false
        self.WriteCommandButton.enabled = ((properties?.rawValue)! & CBCharacteristicProperties.WriteWithoutResponse.rawValue) == 1 ? true: false
        self.NotificationSwitch.enabled = ((properties?.rawValue)! & CBCharacteristicProperties.Notify.rawValue) == 1 ? true: false
        self.IndicationSwitch.enabled = ((properties?.rawValue)! & CBCharacteristicProperties.Indicate.rawValue) == 1 ? true: false
        self.NotificationSwitch.on = false
        self.IndicationSwitch.on = false
        
    }
    
    override func viewDidAppear(animated: Bool) {
        self.configureView()
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func configureView()
    {
        if(peripheral != nil)
        {
            peripheral?.discoverDescriptorsForCharacteristic(characteristic!)
        }
    }
    
    func OnReadChar(characteristic: CBCharacteristic)
    {
        print("KeyfobViewController didUpdateValueForCharacteristic %@", characteristic)
        self.textBoxHex.text = characteristic.value?.description
        self.tableView.reloadData()
    }
    
    func OnWriteChar(characteristic: CBCharacteristic)
    {
        
    }
    
    func dataFromHexString(string: NSString)->NSData?
    {
        let strLower = string.lowercaseString
        let length = strLower.characters.count
        
        
        let rawData = UnsafeMutablePointer<CUnsignedChar>.alloc(length/2)
        var rawIndex = 0
        
        for var index = 0; index < length; index+=2{
            let single = NSMutableString()
            single.appendString(strLower.substringWithRange(Range(start:strLower.startIndex.advancedBy(index), end:strLower.startIndex.advancedBy(index+2))))
            rawData[rawIndex] = UInt8(single as String, radix:16)!
            rawIndex++
        }
        
        let data:NSData = NSData(bytes: rawData, length: length/2)
        rawData.dealloc(length/2)
        
        return data
    }
    
    @IBAction func ReadButtonPressed(sender: AnyObject) {
        if(peripheral != nil)
        {
            if(characteristic != nil)
            {
                peripheral?.readValueForCharacteristic(self.characteristic!)
            }
        }
    }
    
    @IBAction func WriteButtonPressed(sender: AnyObject) {
        
        if(peripheral != nil)
        {
            let data = self.dataFromHexString(self.textBoxHex.text!)
            NSLog("%@", data!);
            
            if(characteristic != nil)
            {
                peripheral?.writeValue(data!, forCharacteristic: self.characteristic!, type: CBCharacteristicWriteType.WithResponse)
            }
            
            
        }
    }
    
    @IBAction func WriteCommandButtonPressed(sender: AnyObject) {
        
        if(peripheral != nil)
        {
            let data = self.dataFromHexString(self.textBoxHex.text!)
            NSLog("%@", data!);
            
            if(characteristic != nil)
            {
                peripheral?.writeValue(data!, forCharacteristic: characteristic!, type: CBCharacteristicWriteType.WithoutResponse)
            }
        }
    }
    
    @IBAction func NotificationSwitchChanged(sender: AnyObject) {
        
        if(self.NotificationSwitch.on == true)
        {
            peripheral?.setNotifyValue(true, forCharacteristic: characteristic!)
        }
        else
        {
            peripheral?.setNotifyValue(false, forCharacteristic: characteristic!)
        }
    }
    
    @IBAction func InidicationSwitchChanged(sender: AnyObject) {
    }
    /*
    // MARK: - Navigation
    
    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
    // Get the new view controller using segue.destinationViewController.
    // Pass the selected object to the new view controller.
    }
    */
    
    var ROWS_PER_SECTION = 3
    
    func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return ROWS_PER_SECTION
    }
    
    func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCellWithIdentifier("Cell")! as UITableViewCell
        
        let char = self.characteristic
        cell.textLabel?.text = ""
        cell.detailTextLabel?.text = ""
        
        switch indexPath.row
        {
        case 0:
            cell.textLabel?.text = "Characteristic Property"
            let properties = char?.properties
            
            cell.detailTextLabel?.text = String(format: "B:%d R:%d w:%d W:%d N:%d I:%d A:%d E:%d",
                (properties?.rawValue)! & CBCharacteristicProperties.Broadcast.rawValue == 1 ? 1 : 0,
                (properties?.rawValue)! & CBCharacteristicProperties.Read.rawValue == 1 ? 1 : 0,
                (properties?.rawValue)! & CBCharacteristicProperties.WriteWithoutResponse.rawValue == 1 ? 1 : 0,
                
                
                (properties?.rawValue)! & CBCharacteristicProperties.Write.rawValue == 1 ? 1 : 0,
                
                (properties?.rawValue)! & CBCharacteristicProperties.Notify.rawValue == 1 ? 1 : 0,
                
                (properties?.rawValue)! & CBCharacteristicProperties.Indicate.rawValue == 1 ? 1 : 0,
                (properties?.rawValue)! & CBCharacteristicProperties.AuthenticatedSignedWrites.rawValue == 1 ? 1 : 0,
                (properties?.rawValue)! & CBCharacteristicProperties.ExtendedProperties.rawValue == 1 ? 1 : 0)
            
            break;
        case 1:
            cell.textLabel?.text = "Vlaue"
            if(char?.value != nil)
            {
                cell.detailTextLabel?.text = "0x".stringByAppendingString((characteristic!.value?.description)!)
            }
            else
            {
                if((characteristic?.properties.rawValue)! & CBCharacteristicProperties.Read.rawValue == 0)
                {
                    cell.detailTextLabel?.text = "Not Readable"
                }
            }
            break;
        case 2:
            cell.textLabel?.text = "Descriptors"
            cell.detailTextLabel?.text = ""
            break;
        default:
            break;
        }
        
        return cell
    }
    
    // mark Selection Delegate
    
    func ClearUI(clear: Bool)
    {
        
    }
    func selectedPeripheral(peripheral: CBPeripheral)
    {
        
    }
    
    func selectedService(service: CBService)
    {
        
    }
    func selectedCharacteristic(characteristic: CBCharacteristic)
    {
        self.characteristic = characteristic
        
        self.configureView()
    }
    
    func splitViewController(svc: UISplitViewController, willHideViewController aViewController: UIViewController, withBarButtonItem barButtonItem: UIBarButtonItem, forPopoverController pc: UIPopoverController) {
        
        barButtonItem.title = "Scan"
        self.navigationItem.setLeftBarButtonItem(barButtonItem, animated: true)
        self.navigationItem.leftItemsSupplementBackButton = true
        self.masterPopoverController = pc
    }
    
    func splitViewController(svc: UISplitViewController, willShowViewController aViewController: UIViewController, invalidatingBarButtonItem barButtonItem: UIBarButtonItem) {
        self.navigationItem.setLeftBarButtonItem(nil, animated: true)
        self.masterPopoverController = nil
    }
    
}