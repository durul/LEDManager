![Untitled.png](https://github.com/durul/LEDManager/blob/master/Untitled.png)

# LEDManager

 ![Platform](https://img.shields.io/badge/Platforms-iOS%20-lightgray.svg?style=flat)

----
- This application that uses Firebase and Node.js to provide access RaspberryPi LED with iOS App. The application is written Swift 3+ and utilizes the Firebase framework.


## Installation Pi3
- [Install Raspbian](https://www.raspberrypi.org/downloads/raspbian/)
- [Install Vnc](https://www.raspberrypi.org/blog/get-back-to-my-pi-from-anywhere-with-vnc-connect/)
- [Install Node-Red](https://nodered.org/docs/hardware/raspberrypi)

## Installation Node.js in Pi3
- **$ mkdir fireled**
- **$ cd fireled**
Use the npm init command to create a package.json file for your application. For more information on how package.json works, see Specifics of npm’s package.json handling.

- **$ npm init**
This command prompts you for a number of things, such as the name and version of your application. For now, you can simply hit RETURN to accept the defaults for most of them, with the following exception:

entry point: (index.js)
Enter app.js, or whatever you want the name of the main file to be. If you want it to be index.js, hit RETURN to accept the suggested default file name.

Now install firebase in the fireled directory and save it in the dependencies list. For example:

- **$ npm install firebase-admin --save**
- **$ npm install -g firebase-tools**
- **$ npm install firebase --save**

### This package is very important
- **$ npm install onoff**
With this package we GPIO access and interrupt detection with Node.js on Linux boards like the Raspberry Pi, C.H.I.P.

- Under the fireled directory there are two files app.js and package.json. Move to copy these files your fireled directory.
- **$ npm install**
- You have to update line 11 and line 15.


## Setup Firebase
- [Create project iOS project](https://firebase.google.com/docs/ios/setup)
- You can import database by import.json from Firebase_Database directory.
- Add Authentication USER on your Firebase project and enable SIGN-IN METHOD. Don't forget update username and password in iOS project. (in ViewController line 31)


## Requirements
- Xcode 8.3.2
- Swift 3.1+
- Deployment Target = iOS 10.3
- Base SDK = iOS 9.3
- [Raspberry Pi 3 Model B Starter Pack](https://www.adafruit.com/product/3058)
- [Firebase account](https://console.firebase.google.com)
- [Ndm](https://720kb.github.io/ndm/)


## TODO
- Android version.


## Asset Credits
Portfolio: http://durul.github.io


## Support
Twitter: https://www.twitter.com/duruldalkanat


## Contribution
- If you'd like to contribute please submit a pull request via GitHub.


## License

The MIT License (MIT)

Copyright (c) 2017 Durul Dalkanat

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
