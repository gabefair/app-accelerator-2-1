//
//  MasterViewController.swift
//  BLEServiceBrowser
//
//  Created by Hiteshwar Vadlamudi on 11/30/15.
//  Copyright © 2015 bluetooth. All rights reserved.
//

import UIKit
import CoreBluetooth

class MainViewController: UITableViewController {

    var peripherals:NSMutableArray = []
    var bleAdapter:BLEAdapter! = sharedInstance
    var scanState:Bool! = false
    var delegate: SelectionDelegate?



    override func viewDidLoad() {
        super.viewDidLoad()
        
        scanState = false
        
    }

    @IBAction func scanButtonPressed(sender: AnyObject) {
        if(!self.scanState) {
            peripherals.removeAllObjects()
            self.tableView.reloadData()
            
            if(bleAdapter.activePeripheral != nil) {
                if(bleAdapter.activePeripheral!.state != CBPeripheralState.Disconnected){
                    bleAdapter.centralManager!.cancelPeripheralConnection(bleAdapter.activePeripheral!)
                }
            }
            bleAdapter.peripherals.removeAllObjects()
            bleAdapter.findBLEPeripherals(2.0)
        } else {
            
        }
        UIApplication.sharedApplication().networkActivityIndicatorVisible = true
        NSTimer.scheduledTimerWithTimeInterval(2.0, target: self, selector: "connectionTimer", userInfo: nil, repeats: false)
    }
   
    
    func connectionTimer() {
        if(bleAdapter.peripherals.count > 0){
            bleAdapter.printKnownPeripherals()
            insertScannedperipherals()
        }
        UIApplication.sharedApplication().networkActivityIndicatorVisible = false
    }
    
    func insertScannedperipherals () {
        for(var i=0; i<bleAdapter.peripherals.count;i++){
            peripherals.insertObject((bleAdapter.peripherals.objectAtIndex(i)), atIndex: 0)
            let indexPath:NSIndexPath =  NSIndexPath(forItem: 0, inSection: 0)
            self.tableView.insertRowsAtIndexPaths([indexPath], withRowAnimation: UITableViewRowAnimation.Fade)
        }
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    
    // MARK: - Segues

    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        if segue.identifier == "showDetail" {
            let nextDetailController = segue.destinationViewController as! DeviceViewController
            let indexPath = self.tableView.indexPathForSelectedRow! as NSIndexPath
            let peripheralObject = peripherals.objectAtIndex(indexPath.row) as! CBPeripheral
            nextDetailController.setDetailItems(peripheralObject)
        }
    }

    // MARK: - Table View

    override func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        return 1
    }

    override func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return peripherals.count
    }

    override func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCellWithIdentifier("Cell", forIndexPath: indexPath)
        let p:CBPeripheral = peripherals.objectAtIndex(indexPath.row) as! CBPeripheral
        if(p.name != nil) {
            cell.textLabel!.text = p.name
        } else {
            cell.textLabel!.text = "Unknown"
        }
        return cell
    }

   


}

