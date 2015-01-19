CBZViewSS (.CBZ Viewer for Small Screens) 

This fork was created to enable CBZViewSS to run on the GCW0 console, the spiritual successor to the Dingoo. In doing so I have fixed a few bugs and added/improved features. 
To compile you will need to setup the GCW0 toolchain on codeblocks (http://wiki.surkow.com/Tutorials:CodeBlocks)
Then link the needed libraries -lz, -lSDL, -lSDL_ttf and -lSDL_gfx.
You also need to add the -GCWZERO flag or gcw0 options will not be present.

Original readme below:
(Note, below bugs are now fixed)

CBZViewSS
=========

CBZ (comics book zipped) Viewer for small screens

This is the initial release of Comic Book Viewer for Small Screens (CBZViewSS).

It's capable of viewing CBZ comic books with JPEG files (quite known format) on Dingux-based devices (320x240)

Supported formats
CBZ comic books with JPEG files. For other formats use Jomic - http://jomic.sourceforge.net/ - it supports conversion to CBZ with JPEG.

System requirements
Dingux device with 320x240 resolution. SWAP-enabled kernel and SWAP file for Dingoo A320 with 32mb ram. Didn't have any problems with 64mb A330, but anyway, consider using SWAP. Comix ARE HUGE.

Installing
Put CBZViewSS.dge to the desired folder of your SD card. Make a shortcut in gMenu/dMenu with ability to pick .cbz file.

Running

.cbz file name should be passed to a programm as a parameter. So, take care about correct gMenu/dMenu shortcut with ability to pick-up a target file. 
After first run you will see some debug output in terminal, then program starts.
Attention: there is a known bug - first page is rendered incorrectly (black screen), so push d-pad left-right to refresh the screen.

Keys

D-Pad - movement
B-Page down
X-Page up
Y-"Line feed" - autoscroll down for a viewport height and move left (most comics have left-to-right reading order, aren't they ? and we had spare button :-))
A+DPad - pixel scroll
R-ZOOM
L-UNZOOM
Start+Select - Exit

Known bugs

Incorrect first page render. See "Running" section for workaround. Still cannot do anything about it.
Non-jpeg files inside cbz cause segfault. Workaround - remove them manually from archive (just delete with your favourite archiving program)


Source code

It's a CodeBlocks project. 


TODO

Serialization to remember last position inside file and zoom factor. XML ?
Progress-bar and loading screens.
CBR support (the code has a basis for a painless integration, you should just inherit some classes, how it is done for zip and pick some rar-supporting library), but using jomic for conversion is less painfull :-)
Actually, I don't have any plans to update in visible future. It just works, and it's free and source included. You know, what to do.

