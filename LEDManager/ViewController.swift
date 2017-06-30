//
//  ViewController.swift
//  LEDManager
//
//  Created by duruldalkanat on 6/24/17.
//  Copyright © 2017 duruldalkanat. All rights reserved.
//

import UIKit
import Firebase

class ViewController: UIViewController {

    //MARK: - IBOutlets
    @IBOutlet weak var titleLabel: UILabel!
    @IBOutlet weak var ledSwitch: UISwitch!
    @IBOutlet weak var statusLabel: UILabel!
    
    var ref: DatabaseReference?
    var switchRef: DatabaseReference?
    
    //MARK: - Super Methods
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        
        FirebaseApp.configure()
        ref = Database.database().reference()
        switchRef = ref?.child("ledSwitch/state")
        
        Auth.auth().signIn(withEmail: "adurul17@hotmail.com", password: "1234567") { (user, error) in
            if let error = error {
                print(error.localizedDescription)
                return
            } else {
                print("Signed in Succesfully \(String(describing: user?.email))")
                self.observerFirebaseSwitch()
            }
        }
    }
    
    @IBAction func switchValueChanged(_ sender: UISwitch) {
        switch sender.isOn {
        case true:
            switchRef?.setValue(1)
            updateStatus(value: true)
        default:
            switchRef?.setValue(0)
            updateStatus(value: false)
        }
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    func observerFirebaseSwitch() {
        switchRef?.observe(.value, with: { (data) in
            if let val = data.value as? Int {
                self.ledSwitch.setOn(Bool(val as NSNumber), animated: true)
                self.updateStatus(value: self.ledSwitch.isOn)
            }
        })
    }

    func updateStatus(value:Bool) {
        value ? (statusLabel.text = "Switch Status OFF") : (statusLabel.text = "Switch Status OFF")
    }
}

