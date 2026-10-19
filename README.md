# waterlevelcontroler_IoT
ESP32 water-level controller with automatic pump switching and manual control through a local web portal.

Install the ESP32 Sketch Data Upload Tool:

To upload files to SPIFFS, you need to install a tool in Arduino IDE:
Download the ESP32 Sketch Data Upload Tool from here.
Follow the installation instructions provided in the repository.
After installation, you should see a new option in your Arduino IDE: Tools > ESP32 Sketch Data Upload.
Create a data Folder:

In your Arduino sketch folder (where your .ino file is located), create a folder called data.
Place the image file (e.g., logo.png) inside this data folder.
Upload the Image to SPIFFS:

After creating the data folder and placing your image inside it, open the Arduino IDE.
Select the ESP32 board you're using from Tools > Board.
Click on Tools > ESP32 Sketch Data Upload.
This will upload the contents of the data folder (including your image) to the ESP32’s SPIFFS.
Modify Your Code to Serve the Image:

Update your code to serve the image from the ESP32's file system using the /logo.png URL.
