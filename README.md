WindowsDesktopMouseControllerFromAndroid
========================================

This is my first Android project. The goal of it is to control the mouse of a computer on Windows with the Android phone.
This was a useful project in order to comprehend a bit about network and multithreading on Android. And of course basic GUI.

The connection between Windows computer and Android phone is via wifi.
I send UDP requests from Android to Windows to get the commands to run on the desktop.

### How to build

#### The Windows server

I use the json minimalistic library [Jsmn](https://bitbucket.org/zserge/jsmn/wiki/Home).
You just need to run cmake with no additional options. The server has been coded in C.
Then compile it and run it normally. The cursor cannot be moved within windows that have been started with higher privileges. Thus it is adivsed to run it as administrator.

Since I use UDP protocol, the cursor moving may be not optimal.

#### The Android client

I use Android Studio to build it.

### Run

Just run the server (as admin if possible) and the client.
Be sure you have wifi enabled on both platforms.
