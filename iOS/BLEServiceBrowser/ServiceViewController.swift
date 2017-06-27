//
//  ServiceViewController.swift
//  BLEServiceBrowser
//
//  Created by Hiteshwar Vadlamudi on 11/30/15.
//  Copyright © 2015 bluetooth. All rights reserved.
//

import Foundation
import CoreBluetooth
import UIKit


class ServiceViewController: UITableViewController, SelectionDelegate, UISplitViewControllerDelegate, BLEAdapterDelegate {
    
    var characteristicsOfAService :NSMutableArray = []
    var bleAdapter: BLEAdapter?
    var service: CBService?
    var peripheral: CBPeripheral?
    var masterPopoverController: UIPopoverController?
    var delegate: SelectionDelegate?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        
        bleAdapter = sharedInstance
        peripheral = service?.peripheral
        bleAdapter?.activePeripheral = peripheral
        self.configureView()
        bleAdapter?.getAllCharacteristicsForService(peripheral!, service: service!)
        
    }
    
    override func viewWillAppear(animated: Bool) {
        let appDelegate = UIApplication.sharedApplication().delegate as! AppDelegate
        appDelegate.setServiceViewDelegate(self)
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }
    
    func configureView() {

        bleAdapter?.bleAdapterDelegate = self

        if(service != nil)
        {
            let strTitle = bleAdapter?.CBUUIDToNSString((service?.UUID)!)
            self.title = strTitle as? String
        }
    }
    
    // MARK: - Table view data source
    
    override func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        // #warning Incomplete implementation, return the number of sections
        return 1
    }
    
    override func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        // #warning Incomplete implementation, return the number of rows
        return characteristicsOfAService.count
    }
    
    
    override func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCellWithIdentifier("Cell")! as UITableViewCell
        let aCharacteristic = characteristicsOfAService.objectAtIndex(indexPath.row) as! CBCharacteristic
        cell.textLabel!.text =  bleAdapter?.CBUUIDToNSString(aCharacteristic.UUID) as? String
        //        cell.detailTextLabel!.text = aCharacteristic.UUID.data.description
        
        return cell
    }
    
    
    
    // Override to support conditional editing of the table view.
    override func tableView(tableView: UITableView, canEditRowAtIndexPath indexPath: NSIndexPath) -> Bool {
        // Return false if you do not want the specified item to be editable.
        return false
    }
    
    override func tableView(tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath) {
        
    }
    
    // MARK: - Navigation
    
    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        if(segue.identifier == "showCharacteristic") {
            let charCtrl = segue.destinationViewController as! CharViewController
            let indexPath = self.tableView.indexPathForSelectedRow! as NSIndexPath
            let charObject = characteristicsOfAService.objectAtIndex(indexPath.row) as! CBCharacteristic
            charCtrl.characteristic = charObject
        }
    }
    
    // mark Selection Delegate
    
    func ClearUI(clear: Bool) {
        
    }
    func selectedPeripheral(peripheral: CBPeripheral) {
        
    }
    
    func selectedService(service: CBService) {
        self.service = service
        self.configureView()
    }
    
    func selectedCharacteristic(characteristic: CBCharacteristic) {
        
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
    
    //BLEAdapterDelegate Methods
    
    func AllCharacteristics(charcterArrays: NSArray)
    {
        for charcter in charcterArrays{
            characteristicsOfAService.insertObject(charcter, atIndex: 0)
            let indexPath = NSIndexPath(forRow: 0, inSection: 0)
            var indxesPath:[NSIndexPath] = [NSIndexPath]()
            indxesPath.append(indexPath)
            
            self.tableView.beginUpdates()
            self.tableView.insertRowsAtIndexPaths(indxesPath, withRowAnimation: UITableViewRowAnimation.Automatic)
            self.tableView.endUpdates()
            
            print("Characteristic found with UUID: %@", charcter.UUID)
        }
    }
    
    func OnConnected(status: Bool){
        //This is not used here
    }
    
    func OnDiscoverService(services: NSArray) {
        //This is not used here
    }
    
    
}