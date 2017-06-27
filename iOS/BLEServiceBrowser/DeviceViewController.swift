////
//  DetailViewController.swift
//  BLEServiceBrowser
//
//  Created by Hiteshwar Vadlamudi on 11/30/15.
//  Copyright © 2015 bluetooth. All rights reserved.
//

import UIKit
import CoreBluetooth

class DeviceViewController: UIViewController, UITableViewDataSource, UITableViewDelegate, SelectionDelegate, UISplitViewControllerDelegate, BLEAdapterDelegate {

    @IBOutlet weak var detailDescriptionLabel: UILabel!
    @IBOutlet weak var peripheralConnectionStatusLabel: UILabel!
    @IBOutlet weak var servicesLabel: UILabel!
    @IBOutlet weak var statusLabel: UILabel!
    @IBOutlet weak var connectButton: UIBarButtonItem!
    @IBOutlet weak var serviceTableViewOutlet: UITableView!

    var peripheral: CBPeripheral!
    var bleAdapter:BLEAdapter! = sharedInstance
    var pheripheralServices: NSMutableArray = []
    var delegate:SelectionDelegate?
    var masterPopoverController: UIPopoverController?


    func setDetailItems(peripheralDetailItem: AnyObject?) {
        peripheral = peripheralDetailItem as! CBPeripheral
    }
    
    func configureView() {
        bleAdapter.bleAdapterDelegate = self
        
        adjustUI(false)
        // Update the user interface for the detail item.
        if let peripheral = self.peripheral {
            if let peripheralDescription = self.detailDescriptionLabel {
                peripheralDescription.text = peripheral.description
                peripheralDescription.layer.borderColor = UIColor.redColor().CGColor
                peripheralDescription.layer.borderWidth = 2.0
            }
            
        }
    }


    func adjustUI(clear: Bool){
        if(clear == true){
            self.serviceTableViewOutlet.hidden = true
            self.navigationItem.rightBarButtonItem = nil
            self.detailDescriptionLabel.hidden = true
            self.peripheralConnectionStatusLabel.hidden = true
            self.statusLabel.hidden = true
            self.servicesLabel.hidden = true
            pheripheralServices.removeAllObjects()
            self.serviceTableViewOutlet.reloadData()
        }
        else
        {
            self.serviceTableViewOutlet.hidden = false
            self.navigationItem.rightBarButtonItem = connectButton
            self.detailDescriptionLabel.hidden = false
            self.peripheralConnectionStatusLabel.hidden = false
            self.statusLabel.hidden = false
            self.servicesLabel.hidden = false
            pheripheralServices.removeAllObjects()
            self.serviceTableViewOutlet.reloadData()
        }
    }

    @IBAction func connectButtonPressed(sender: AnyObject) {
        if(peripheral.state == CBPeripheralState.Disconnected)
        {
            pheripheralServices.removeAllObjects()
            serviceTableViewOutlet.reloadData()
            bleAdapter.connectPeripheral(peripheral, status: true)
        }
        else
        {
            bleAdapter.connectPeripheral(peripheral, status: false)
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()

        let appDelegate = UIApplication.sharedApplication().delegate as! AppDelegate
        appDelegate.setMainViewDelegate(self)
        
        detailDescriptionLabel.text = ""
        peripheralConnectionStatusLabel.text = "Not Connected"
        
        serviceTableViewOutlet.dataSource = self
        serviceTableViewOutlet.delegate = self
        bleAdapter.bleAdapterDelegate = self
        
        self.configureView()
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        if(segue.identifier == "showService") {
            let serviceCtrl = segue.destinationViewController as! ServiceViewController
            let indexPath = self.serviceTableViewOutlet.indexPathForSelectedRow! as NSIndexPath
            let serviceObject = pheripheralServices.objectAtIndex(indexPath.row) as! CBService
            serviceCtrl.service = serviceObject
        }
    }

    //Mark: Functions from Split View Delegate
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
    
    //Mark: Table View
    func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return pheripheralServices.count
    }
    
    func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCellWithIdentifier("Cell")! as UITableViewCell
        let aService = pheripheralServices.objectAtIndex(indexPath.row) as! CBService
        
        cell.textLabel?.text = bleAdapter.GetServiceName(aService.UUID)
        cell.detailTextLabel?.text = aService.UUID.data.description
        
        return cell
    }
    
    func tableView(tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath) {
        if(UIDevice.currentDevice().userInterfaceIdiom == UIUserInterfaceIdiom.Pad)
        {
            let object = pheripheralServices.objectAtIndex(indexPath.row) as! CBService
            
        }
    }

    
    //Mark: Functions from Selection Delegate
    func ClearUI(clear: Bool)
    {
            //Not Implementing
    }
    
    func selectedPeripheral(peripheral: CBPeripheral)
    {
        self.peripheral = peripheral
        self.configureView()
    }
    
    func selectedService(service: CBService)
    {
        //Not Implementing
    }
    
    func selectedCharacteristic(characteristic: CBCharacteristic)
    {
        //Not Implementing
    }
    
    
    //BLE Adapter Delegate
    //This function will get called whenever a Device is getting connected.
    func OnConnected(status: Bool){
        if(status == true) {
            peripheralConnectionStatusLabel.text = "Connected"
            connectButton = UIBarButtonItem(title: "Disconnect", style: UIBarButtonItemStyle.Plain, target: self, action: "connectButtonPressed:")
        } else {
            peripheralConnectionStatusLabel.text = "Not Connected";
            connectButton = UIBarButtonItem(title: "Connect", style: UIBarButtonItemStyle.Plain, target: self, action: "connectButtonPressed:")
        }
    }
    
    func OnDiscoverService(services: NSArray) {
        for service in services {
            pheripheralServices.insertObject(service, atIndex: 0)
            let indexPath = NSIndexPath(forRow: 0, inSection: 0)
            
            serviceTableViewOutlet.beginUpdates()
            self.serviceTableViewOutlet.insertRowsAtIndexPaths([indexPath], withRowAnimation: UITableViewRowAnimation.Automatic)
            serviceTableViewOutlet.endUpdates()
        }
    }
    
    func AllCharacteristics(charcterArrays: NSArray)
    {
        //This is Optional. This will  be used on Service View COntroller
    }

}

