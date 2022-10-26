# ESP32 Reddit Viewer
A viewer for Subreddits. 

# Config
- Place your SSID in `const char* ssid = "???";`
- Place your password in `const char* password = "???";`
- Customise your Subreddits in `String subreddits[] = {"showerthoughts", "technology", "esp32", "pcgaming", "askreddit", "dubai"};`. 
Make sure to change `int listlength = 6;` to the length of the above list so the system knows how many Subreddits to choose from. 

This program should work on *most* displays however if you have any issues try changing the TFT_eSPI User_Setup.h file to match your display but if there are any further issues, just post them in the issues tab and I would be very happy to fix them and make sure the code is versatile for all devices. 

If you have any issues or you want to suggest new features, please put them in the issues tab and me and my community will gladly help. 
