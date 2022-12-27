.\"  (c) Copyright 1989, 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.nr H1 1
.H 1 "Using a Motif Application"
Application programs written for Motif are intended to follow a particular
style of appearance and behavior.  This common style is designed to
directly benefit you so that your interaction with
all applications is the same and you don't have to learn new techniques before
putting a new application program to work.
.iX "mre"
.P
In this chapter you will use the Motif Resource Editor program
(\fBmre\fR) to change some of the colors and fonts displayed on your screen.
In doing so, you will learn the following techniques for using a Motif
application:
.BL
.LI
Using a list box with scroll bars
.LI
Using pull-down menus (including some shortcuts)
.LI
Entering text into an input box
.LI
Using push buttons
.LI
Reacting to dialogs
.LI
Using check boxes and radio buttons
.LE
.sp
.nS
The examples in this chapter use \fBmre\fR to demonstrate general techniques
for using a Motif application.
.nE
.SK

.H 2 "Getting Ready"
Before working the examples in this chapter, you must start the X Window
System and the Motif Window Manager just as you did in Chapter 1.
.iX "sys.Xdefaults file"
.H 3 "Copying sys.Xdefaults Into Your Home Directory"
The file \fB/usr/lib/X11/sys.Xdefaults\fR is read each time you start a
program written for X.  In this chapter you will work on
a copy of this file (named \fB.Xdefaults\fR) in your home directory.
.P
To copy \fBsys.Xdefaults\fR into your home directory, renaming it to
\&\fB.Xdefaults\fR, execute these commands:
.DS
\fBcd\ <Return>\fB
.sp
cp\ /usr/lib/X11/sys.Xdefaults\ .Xdefaults\ <Return>\fB\fR
.DE
.iX "mre" "starting"
.H 3 "Starting the Motif Resource Editor Program"
Next, start the \fBmre\fR program.  Point to the terminal window and click
the select button (to make the window active).  Then type this command:
.DS
\fBmre\ &\ <Return>\fB\fR
.DE
If you have correctly entered the command, a few moments later you will
see the application's window appear on the screen.
.P
If you have trouble starting \fBmre\fR, check to see that you entered the
command properly.  Remember, if the terminal window is not the active
window, everything you type is lost.  If you are still having problems,
consult your system administrator to be sure everything has been installed
properly (including the \fBmre\fR program).
.SK

.H 3 "Recognizing Common Controls"
.iX "mre" "controls"
.iX "controls" "common"
The \fBmre\fR program has many of the controls that you might use in other
Motif applications as shown in Figure 2-1.
.iX "menu bar"
.iX "list box"
.iX "push buttons"
.iX "scroll bar"
.br
.ne 17P
\0
.FG "Typical Controls Used by a Motif Application"
.sp 14P
.in +.69i
.P! graphics/TypCntrl.ps 
.in -.69i
.P
.P
As you work through the examples in the rest of this chapter, you will learn
how to use each of these controls.  After completing this chapter, you
should be able to immediately use another Motif application quickly
and easily.
.SK

.iX "list" "using"
.H 2 "Using a List"
The list box near the center of the \fBmre\fR window contains all the
resources specified in your \fB.Xdefaults\fR file.  (The \fBmre\fR program may
be used to work with other resource files; \fB.Xdefaults\fR was loaded by default 
because you didn't specify a file.)
.iX "scrolling"
.iX "list" "scrolling"
.iX "scroll bar"
.H 3 "Scrolling the List"
Whenever the list of resources is larger than the list box, \fBmre\fR
displays scroll bars along the right and bottom edges of the box.  A
scroll bar is a control that allows you to control what portion of a larger
object is displayed within the box.
.P
Scroll bars have an arrow at each end and a rectangular slider in the
middle.  The slider represents the current relative position in the list.
There are two ways to move a slider:
.BL
.iX "arrow buttons"
.LI
Using the arrow buttons
.iX "slider" "dragging"
.iX "dragging the slider"
.LI
Dragging the slider
.LE
.iX "arrow buttons"
.H 4 "Using the Arrow Buttons"
Point to the down arrow (\(da) in the vertical scroll bar.  Press and
hold the select button until the list stops scrolling\*(EMit may take a
moment to get to the end of the list\*(EMthen release the button.
.br
.ne 9P
\0
.FG "Using an Arrow Button"
.sp 6P
.in +2.2i
.P! graphics/ScrollDn.ps 
.in -2.2i
.SK

.iX "slider" "dragging"
.iX "dragging the slider"
.H 4 "Using the Slider"
Now, point to the slider (which should now be at the bottom of the scroll
bar), press and hold the select button to grab the slider as shown in Figure 2-3.  
While still holding the button, move the pointer up and down and watch the list scroll
to keep up with your movements.
.br
.ne 9P
\0
.FG "Using a Slider"
.sp 6P
.in +2.56i
.P! graphics/Slider.ps 
.in -2.56i
.P
On your own, play with the scroll bar controls until you feel comfortable
with moving the list up and down.  If the list has any long lines, \fBmre\fR
displays a horizontal scroll bar for scrolling side-to-side.  Try that one
too.
.iX "list" "selecting and item"
.H 3 "Selecting an Item in the List"
When a list item is selected, it is highlighted (displayed in inverse
video) and shows up in the Selection box just below the list.  There are two
ways to select an item in the list:
.BL
.LI
Point to the item you want to select and click the select button.
.LI
Or, use the <\(ua>\fR and <\(da>\fR keys to move the highlight to the
item you want.
.LE
.P
In the next section, you will use the second method to select a particular
resource.
.SK

.iX "push buttons"
.H 2 "Using Push Buttons"
A push button is a control that causes an immediate action.  To press
a push button, point to it and then click the select button.
.H 3 "Pressing a Push Button"
When you first start \fBmre\fR, the list box contains all of the resources in
your \fB.Xdefaults\fR file.  The push buttons to the left of the list box
allow you to quickly redisplay the list of resources showing only a
particular type of resource.
.P
To display only the resources associated with color, click the push button
that looks like a painter's palette.  See Figure 2-4.
.iX "palette button"
.br
.ne 7P
\0
.FG "Press the Palette Button to Display Color Resources Only"
.sp 3.5P
.in +2.56i
.P! graphics/PaletBtn.ps 
.in -2.56i
.P
.P
.sp
The resource list is redisplayed, this time showing only color resources.
(The other resources have not been deleted, \fBmre\fR is
simply not showing them.)
.P
Use the <\(ua>\fR and <\(da>\fR keys on your keyboard to move up and down
the list of resources until this line is highlighted:
.DS
\fBxterm*background:   white\fR
.DE
.SK

.H 3 "Using the <Return> Key: A Short Cut to the Default Push Button"
The frame around the Edit push button means that pressing <Return>\fR is
equivalent to clicking Edit. Often, the most frequently used push button
has a frame like this, indicating that it is the default selection.
.P
Press <Return>\fR now to edit the color resource that you selected above
(\fBxterm*background\fR).  The \fBmre\fR program displays a new window
specifically for editing a color resource.
.P
In the next section, Entering Text into an Input Field, you will select a
new color for this resource.
.iX "double-clicking"
.H 3 "Double-Clicking: Another Short Cut to the Default Push Button"
Double-clicking on an item in a list is even faster than using <Return>\fR
or clicking Edit.  Remember, in order 
to double-click, first point to the item, then
rapidly press and release the select button twice.
.SK

.iX "text" "entering"
.iX "entering text"
.iX "typing text"
.iX "input"
.H 2 "Entering Text Into an Input Field"
So far, most of your interaction with the \fBmre\fR program has been with the
mouse.  In this section, you will use your keyboard to enter some text (a
color name) into an input field.
.nr Hu 3
.HU "Step 1: Selecting the Input Field"
Since some application may display more than one input field at a time, you
may have to select the field you want to type into.  To select the input
field, point to it and then click the select button.
.P
For this example, point to the box just above the push buttons in the Color
Selection Window and click.  An I-shaped cursor begins blinking in the box.
This cursor marks your typing position.
.SK
.nr Hu 3
.HU "Step 2: Typing the Text"
Use <Backspace>\fR and <Del>\fR to delete unwanted characters, then 
type \fBorange\fR into the field as shown in Figure 2-5.  Use 
the <\(<->\fR and <\(->>\fR keys to move the cursor left and right.
.br
.ne 10P
\0
.FG "The \fRI\fP Cursor is a Cue to Type Text"
.sp 7P
.in +.69i
.P! graphics/TypnText.ps 
.in -.69i
.nr Hu 3
.HU "Step 3: Accepting Your Input"
When you are happy with the text you have entered, click the OK button.
This action accepts the text you entered and closes the Color Selection
window.  Notice that the resource you changed is now displayed as
\fBxterm*background:orange\fR in the list.
.SK

.iX "pull-down menus"
.H 2 "Exploring Pull-Down Menus"
Most Motif applications have a menu bar across the top of the window (just
below the title bar) as shown in Figure 2-6.  Each word in the menu bar represents 
a pull-down menu, which is displayed by pulling down on the menu bar 
with the mouse.
.br
.ne 9P
\0
.FG "A Typical Menu Bar"
.sp 5.5P
.in +.56i
.P! graphics/MenuBar.ps 
.in -.56i
.P
.sp
There are two ways to choose a function from a pull-down menu using the
mouse:
.BL
.LI
\fIDrag method\fR: Point to the menu you want to display, press and
hold the select button, drag the pointer to highlight the desired
function, and then release the button.
.LI
\fIClick method\fR: Point to the menu you want to display, click the select
button once to display the menu, point to the desired function, and then
click again.
.LE
.P
If you decide you don't want to execute a function after the menu is
displayed, point away from the menu (so that no function is highlighted),
and then release or click the button.
.P
The menu bar is the primary way to access an application's functions.
Therefore, browsing through the menus and their functions often serves as
an excellent overview of a program's features.
.P
The File menu typically contains functions for opening, closing, deleting,
and printing files as shown in Figure 2-7.  To display the File menu, point 
to \fBFile\fP in the menu bar, and then press and hold the 
select button.
.iX "File menu"
.SK
.br
.ne 16P
\0
.FG "Displaying the File Menu"
.sp 12P
.in +.8i
.P! graphics/FileMenu.ps 
.in -.8i
.P
.P
.sp
While still holding the button down, drag the pointer down the File
menu.  Notice that each function is highlighted as the pointer passes over it.
Don't let go of the button yet.
.P
Now point to \fBEdit\fP in the menu bar as shown in Figure 2-8.  The File 
menu disappears and the Edit menu appears.
.iX "Edit menu"
.br
.ne 13P
\0
.FG "Displaying the Edit Menu"
.sp 9P
.in +.8i
.P! graphics/EditMenu.ps 
.in -.8i
.P
.P
.sp
On your own\*(EMwhile still holding the button down\*(EMexplore each of the
pull-down menus in the \fBmre\fR menu bar.  When you are done, move the mouse
completely away from the menu (so that no menu function is highlighted)
before releasing the button.
.SK

.iX "cascade menu"
.iX "menu" "cascade"
.H 2 "Using a Cascade Menu"
Earlier you used the palette button to cause \fBmre\fR to display only color
resources.  In this section you will choose a function from a cascade menu
that will resort the color resources.
.nr Hu 3
.HU "Step 1: Displaying the Menu"
Display the View menu by pointing to View in the menu bar and holding
down the select button.  There are two distinguishing features of the View
menu that you may not have seen before:
.BL
.LI
Cascade menus (also called submenus), which are indicated with an
arrow next to the function name.  Cascade menus are used to
group similar functions together.
.LI
Check boxes that represent a group of settings that may be set in any
combination.  Check boxes, which are frequently used outside of menus too, 
are demonstrated in the next section.
.LE
.nr Hu 3
.HU "Step 2: Displaying a Cascade Menu"
While still holding the select button, drag the pointer slowly down the
menu stopping briefly on each of the selections that has an arrow next to
it.  Notice that each one has a cascade menu that appears as you point to
it.  The cascade menus in the View menu all contain the same two functions:
ascending and descending as shown in Figure 2-9.
.SK
.br
.ne 21P
\0
.FG "Displaying a Cascade Menu"
.sp 18P
.in +.3i
.P! graphics/ViewCasc.ps 
.in -.3i
.nr Hu 3
.HU "Step 3: Choosing a Function From a Cascade Menu"
While pointing to the By Resource menu item, carefully move the pointer
to the right until ascending is also highlighted.  Then release the
button.  This action, which  might be read as view by resource in
ascending order, causes the resources in the list to be sorted and
redisplayed in ascending (alphabetical) order according to their resource
names.
.SK

.iX "check boxes"
.iX "radio buttons"
.iX "toggle buttons"
.H 2 "Using Check Boxes and Radio Buttons"
When an application has a finite list of options or settings, they are
often presented as a list of toggle buttons.  Like a light switch, a
toggle button can be on or off.  There are two ways that a group of
toggle buttons may be presented:
.BL
.LI
Check boxes are used for a group of settings that can be set in any
combination.  Check boxes are usually displayed as small square buttons.
.LI
Radio buttons are used to group related settings that allow only
one selection at a time.  Radio buttons are usually displayed as small
diamond-shaped buttons.
.LE
.H 3 "Using Check Boxes"
The \fBmre\fR program's View By Resource menu contains a set of check boxes; 
one for each type of resource.  When you first start \fBmre\fR all the boxes are
selected (on), so that all resources are displayed.
.P
The three push buttons along the left edge of the \fBmre\fR window are
provided as shortcuts that unselect all but one type of resource (colors,
fonts, and true/false types, respectively).  Since the resource button you 
pressed most recently was the palette button, \fBmre\fR is currently displaying 
only color resources.  To also display the font and file resources, follow these 
two steps:
.AL 1
.LI
Choose \fBShow font resources\fP from the View menu.
.LI
Choose \fBShow file resources\fP from the View menu.
.LE
.SK

.H 3 "Using Radio Buttons"
The \fBmre\fR program uses radio buttons to let you specify font
characteristics.  To see how they work, create a new entry for your
\fBxterm\fR font and open the Font Selection window by typing this into the
Selection box of the \fBmre\fR main window:
.DS
\fBxterm*font: <Return>\fB\fR
.DE
At the top of the Font Selection window, there are four sets of radio
buttons as shown in Figure 2-10.  Initially, none of the buttons is selected.  
Click one button under each of the four categories.  Now go through and
click a different button in each category.  Notice that only
one button at a time can be selected in each group.
.br
.ne 16P
\0
.FG "Radio Buttons List Your Options and Current Selections"
.sp 12P
.in +.31i
.P! graphics/RadioBtn.ps 
.in -.31i
.P
.sp
Now click the \fBFilter to criteria\fP button to list the fonts on your system
that match the criteria you have just set with the radio buttons.
.P
Using the \fBDisplay all matches\fP and \fBDisplay selection\fP push buttons, you
can display samples of the fonts.  When you find a font that you like,
select it (by clicking its name in the list) and then click OK.
.nS
If your system does not support this method of specifying fonts, you may have
to type a font name into the Selection window (just above the push
buttons).  If you are really stuck, the font named \fBfixed\fR is available
on most systems.
.nE
.SK

.iX "File menu"
.iX "saving a file"
.iX "file" "saving"
.H 2 "Saving a File"
There are two functions in the File menu for saving your file.  The Save
function simply saves the current file using the same name.  The Save As
function allows you to enter or select a different file name.
.P
Throughout this chapter, you have been working on the \fB.Xdefaults\fR file
in your home directory.  If you want to overwrite your \fB.Xdefaults\fR file,
then work through "Saving the Current File" below.  If you want to save
your work under a different file name, work through "Saving as Another
File" in Section 2.8.2.
.H 3 "Saving the Current File"
If you want to overwrite your current \fB.Xdefaults\fR file with the changes
you've made in this chapter, use one of the following methods to choose
Save from the File menu:
.BL
.LI
Use the mouse
.LI
Use the accelerator key:  <CTRL>\fR <S>\fR
.LI
Use the mnemonics: <Alt>\fR <F>\fR <S>\fR
.LE
.P
Accelerators and mnemonics are explained later in this chapter.
.SK

.H 3 "Saving as Another File"
If you do not want to overwrite your current \fB.Xdefaults\fR file with the
changes you've made in this chapter, follow these steps to create a new file:
.AL 1
.LI
Choose Save As from the File menu.
.LI
Type a new file name into the Selection box of the \fBSave file as \&.\|.\|.\fP
window.  Be sure to pick a name that you have not already used for something
else.
.LI
Click OK or press <Return>\fR.
.LE
.H 3 "Viewing the Results"
Whenever you change your \fB.Xdefaults\fR file (or any other configuration
file), the changes are not read until you restart the
programs that are affected by the changes.
.SK

.iX "error messages" "reacting to"
.iX "messages" "reacting to"
.H 2 "Reacting to Messages"
Motif applications are designed to display helpful messages to alert you to
important conditions or errors.
.H 3 "Creating an Error"
In this example, you will purposely cause an error by attempting to load a
file that doesn't exist.  To create the error, follow these steps:
.AL 1
.LI
Choose Open\&.\|.\|. from the File menu.
.LI
Type \fBbogus\fR into the Selection box in the Open File window.
.LI
Press <Return>\fR (or click on the OK button).
.LE
.P
Since the file you specified doesn't exist, \fBmre\fR is confused and
displays the message shown in Figure 2-11.
.br
.ne 13P
\0
.FG "A Typical Error Message"
.sp 10P
.in +1.56i
.P! graphics/ErrorMsg.ps 
.in -1.56i
.SK

.H 3 "Acknowledging the Error"
Many applications require you to acknowledge a message before the program
will continue to run.  If the message is confusing, a Help button
is often included to provide you with more information about the message.
.P
To acknowledge the error, click the OK button.  The dialog box disappears
and you may continue working.
.SK

.iX "short cuts" "menus"
.iX "menu" "short cuts"
.H 2 "Taking Short Cuts With Menus"
Earlier in this chapter you learned how to choose menu functions using the
mouse.  Two additional features allow you to quickly execute
a function in a menu without using the mouse.
.BL
.iX "accelerator" "menu"
.iX "menu" "accelerator"
.LI
An accelerator key is a key on your keyboard that the application has
assigned to directly execute a function in a menu.  Accelerators are listed
in the menu next to the function names.
.iX "mnemonic" "menu"
.iX "menu" "mnemonic"
.LI
A mnemonic provides a similar capability by allowing you to select from a
menu by typing significant letters within the menu.  Mnemonics are
indicated as underlined characters in the menu.
.LE
.br
.ne 10P
\0
.FG "Many Menus Have Accelerators and Mnemonics"
.sp 7P
.in +.8i
.P! graphics/MenuShor.ps 
.in -.8i
.P
.sp
The important difference is that accelerator keys allow you to immediately
execute a function without displaying the menu.  Mnemonics, on the other
hand, are simply a way to display menus and choose functions without using
the mouse.
.H 3 "Using an Accelerator Key"
Notice that the Help menu has an accelerator listed in the menu bar.  At
any time while you are using \fBmre\fR, you can get help by pressing <F1>\fR.
Try it.
.P
When you are done with the Help information, click OK in the Help window.
.SK

.H 3 "Displaying a Menu Using Its Mnemonic"
To use a mnemonic to display a menu, hold down the <Alt>\fR key and then
press the letter that is underlined in the menu's name.
.P
Try this:  Display the Help menu by typing <Alt>\fR <H>\fR (because the H
is underlined).
.H 3 "Choosing a Function Using Its Mnemonic"
Once a menu is displayed, you can use its mnemonic to execute a function by
typing the letter that is underlined in the function's name.
.P
If the Help menu is still displayed, press <C>\fR to display helpful
information about colors.
.iX "menu" "navigating with cursor keys"
.H 3 "Navigating Through Menus With the Cursor Keys"
After displaying a menu by clicking the mouse or using a mnemonic, you can
use the cursor keys (<\(<->\fR, <\(->>\fR, <\(ua>\fR, and <\(da>\fR) to
move the highlight.
.P
When the function you want to execute is highlighted, press <Return>\fR.  To
cancel the menu without choosing a function, press <ESC>\fR.
.P
Try this:  Display the Help menu again by pressing <Alt>\fR <H>\fR.  Now use
the <\(ua>\fR and <\(da>\fR keys to move the highlight.  When the Fonts
selection is highlighted, press <Return>\fR.  Just as before, when you are
done reading the Help information, click OK.
.SK

.iX "short cuts" "menus"
.H 2 "A Summary of Menu Short Cuts"
.sp -10p
.H 3 "Displaying a Menu"
As you've seen, there are two general ways to display a pull-down menu:
.BL
.LI
Using the mouse, point to the menu name in the menu bar and then \&.\|.\|.
.DL
.LI
Click the select button
.LI
Or, press and hold the select button
.LE
.LI
Using the menu's mnemonic (press <Alt>\fR plus the underlined character).
For example, press <Alt>\fR <F>\fR to display the File menu.
.LE
.H 3 "Choosing a Menu Function"
Once a menu is displayed, you can choose a function like this:
.BL
.LI
If you are still holding the mouse button, drag the pointer down the menu
until the desired function is highlighted, then release the button.
.LI
If you clicked the select button or used the menu's mnemonic, you can
choose a function using any of these techniques:
.DL
.LI
Point to the desired function and click the select button
.LI
Type the function's mnemonic (underlined character)
.LI
Use the cursor keys (<\(ua>\fR, <\(da>\fR, <\(<->\fR, and <\(->>\fR) to
move the highlight to the desired function, and then press <Return>\fR
.LE  
.LE
.P
You can also use accelerators to execute menu functions without displaying
the menu.  For example, the Cut function in the Edit menu can be executed
by typing <Shift>\fR <Del>\fR.
