************************************************
 ABOUT 
************************************************
* Addon: ofxGriddedBackground

Description:
* We came across a scenario where we wanted to have a very large (30mb jpg) image work as a background
   across multiple screens/computers.  We were working in a memory constrained environment so loading
   the entire image into memory was not a realistic option.  We created a tool chain so we could easily
   turn large images into multiple gridded images, and then load/unload and display these gridded images
   depending on what view we had on each screen.  That is what this tool does.  It also can scroll the
   image left or right over time, and it repeats it so that there is no white space.



************************************************
 SETUP
************************************************
Hardware Requirements:
  * Computer

Local:
  1. Make sure you have the ofxThreadedImageLoader addon present.  This ships by default.
  2. Get a good source image (http://upload.wikimedia.org/wikipedia/commons/e/e6/Clocktower_Panorama_20080622_20mb.jpg)
  3. Grid up the image using the backgroundSlider tool (drop image into the window)
  4. Copy/move the images from backgroundSlider/bin/data to example/bin/data (or whatever project)
  5. Run the example (or your project).

NOTE: The example has a background that draws in 2 places.  It mimicks having 2 monitors right next to each other.
       additionally it shows a shaded white window over what is on screen.  This lets you understand how the loading works.

************************************************
 RUN
************************************************

  * The setup describe this

************************************************
 RUN FROM SRC 
************************************************

************************************************
 MORE INFO 
************************************************

* Troubleshooting
  * Turn on debug, and see if the images are loading/unloading as expected.
  * Try a very large image (http://upload.wikimedia.org/wikipedia/commons/e/e6/Clocktower_Panorama_20080622_20mb.jpg)
  * Try a very small image (anything scaled down).

* Licensing
  * TSPS is MIT + LGPL

* Notes
  *

