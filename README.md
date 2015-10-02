# jts-0304-drop-tower
Camera control program for JTS drop towers written in c++

# Installation instructions

**Above and beyond kiosk setup**

Unfortunately the install process for this program is not super simple. The complication arises from the fact that the application is written using primarily 32-bit libraries, but it is running on 64-bit architecture. 

Normally, this wouldn't be a problem, except that the runtime driver for the camera is installed in the same package as the runtime DLLs, and we have to have the 64-bit driver installed, but use the 32-bit dlls. If you follow the steps below, this shouldn't be a problem, but it is obnoxious.

*1. Install the filter driver*

Grab the 64-bit VwFilter file from the Project Server. It is located at "Journey to Space (SPACE)/Multimedia/0302 ME-01 Drop Tower/00 Application/Drivers/64-bit". On the exhibit computer, run the .exe, and follow through each of the dialog boxes. Once it is installed, open the windows menu, type "appwiz.cpl" into the search box, and press enter to bring up the "Add or Remove Programs" window. Scroll down to find "Vieworks Imaging Solution", select it, and click uninstall. It will ask you if you wish to remove the filter driver, to which you should answer "No". Continue through the following dialogs to finish removing the 64-bit library.

*2. Install the 32-bit libraries*

Grab the 32-bit VwFilter file from the Project Server. It is located at "Journey to Space (SPACE)/Multimedia/0302 ME-01 Drop Tower/00 Application/Drivers/32-bit". On the exhibit computer, run the .exe, and follow through each of the dialog boxes. Once it finished installing, you should be good to move on to the next step of installation.

*3. Install the dropTower application*

From the project server, download the "dropTower" application folder. It should be located at: "Journey to Space (SPACE)/Multimedia/0302 ME-01 Drop Tower/00 Application/". Place this folder on the desktop of the exhibit computer. In the "dropTower" folder that is now on the desktop, make a shortcut to the "startup.bat" file, and place this shortcut into the "Windows Menu/All Programs/startup" folder. Then, you should be good to go! 
