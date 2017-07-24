// GPIO requirements and config
// Button is attached to pin 17, led is attached to pin 18

var GPIO = require('onoff').Gpio,
	led = new GPIO(18, 'out'),
	button = new GPIO(17, 'in', 'both');

// Firebase requirements and config
var admin = require("firebase-admin");

var serviceAccount = require("YOUR JSON FILE");

admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
  databaseURL: "YOUR DATABASE URL"
});

console.log("Begin LED button code");

// Firebase database references
var db = admin.database();
var switchRef = db.ref('ledSwitch/state');

// define the callback function for firebase switch
function firebaseSwitch(state) {
	// check the switch value in firebase
	// 1 == on, 0 == off
	if (state == 1) {
		led.writeSync(1);
		console.log("Led ON FIREBASE");
	} else {
		led.writeSync(0)
		console.log("Led OFF FIREBASE");
	}
}


// define the callback function for button press
function buttonPress(err, state) {
	// check the state of the button
	// 1 == pressed, 0 == not pressed
		if (state == 1) {
		led.writeSync(1);
		console.log("Led ON BUTTON");
	} else {
		led.writeSync(0)
		console.log("Led OFF BUTTON");
	}
}

// Firebase key observer
switchRef.on('value', function(snapshot){
	firebaseSwitch(snapshot.val())
});


// Button observer
// pass the callback function
// as the first argument
button.watch(buttonPress);
