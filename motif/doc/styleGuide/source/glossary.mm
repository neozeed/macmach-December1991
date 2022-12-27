.\"  (c) Copyright 1989, OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.nr Hu 1
.HU "Glossary"
.P
.VL 2i
.LI acceleration
A temporary change in the mouse pointer gain.
.LI accelerator
A key or sequence of keys (typically a modifier key and some other key)
that provides a short-cut, immediately accessing a program function.  
.LI anchor
A position in a collection of selectable objects that marks one endpoint
of an extended selection range.
.LI autorepeat
A means of PushButton activation where a
mouse button is pressed and held on a PushButton and the PushButton
continues to activate at regular intervals until the mouse button is
released.
.LI "browse\ selection"
A selection model that allows browsing through single selection collections.
.LI button
A button on a mouse pointing device; mouse buttons can be mapped to the
keyboard.  A graphical component, on a window frame or in a DialogBox, that
works by pressing it.
.LI "button\ binding"
Association of a mouse button operation with a window manager or
application function.
.br
.ne 4
.LI "cancel"
A label given to a PushButton in some DialogBoxes, that performs the
action of closing the DialogBox without implementing any changes.
.br
.ne 5
.LI "cascading\ Menu"
A submenu that provides selections that amplify the parent selection on 
a PullDown or Popup Menu.
.LI "CheckButton"
A component used to select settings that are not mutually exclusive.  The
visual cue to the selection is frequently that the button is filled in or
checked.
.LI "click"
To press and release a mouse button.  The term comes from the fact that
pressing and releasing the buttons of most mice makes a clicking sound.
.LI "client"
An application program written specifically for the X Window System.  Some
clients make their own windows.  Other clients are utility programs.
.LI "client\ area"
The area, within the borders of a primary window's frame, that is controlled
by an application.
.LI "clipboard"
Any device used to store text or graphics during cut-and-paste operations.
.LI "close"
A label given to a PushButton in some DialogBoxes that performs the
action of closing the DialogBox.  Close is also used as a selection in
Menus to close the window associated with the Menu.
.LI "component\ navigation"
Moving the keyboard focus among controls within a window.
.LI "control\ panel"
An area of a window, similar to the control panels in real life, that is used
to hold PushButtons and other graphical components.
.br
.ne 8
.LI "cursor"
A graphical image, usually a pipe (|) or block, that shows the location
where text will appear on the screen when keys on the keyboard are pressed or
where a selection can be made.
.LI "default\ (selection)"
An object or action that is specified for selection if no other selection
is specified.
.LI "desktop"
Another term for workspace.
.LI "destination\ cursor"
The position in a component where paste operations are inserted.
.LI "DialogBox"
A secondary window that the user can display and that
contains application components.
.LI "dimmed\ selection"
A selection that is not currently available.
.LI "discontiguous\ selection"
A selection model that allows multiple discontiguous selections.
.LI "double-click"
To press and release a mouse button twice in rapid succession.
.LI "drag"
To press and hold a mouse button while moving the mouse.
.LI "expert\ activation"
A means of activation where double-clicking on a PushButton activates
a different action from clicking the PushButton.
.LI "explicit\ focus"
A keyboard focus model that sends keyboard events to the window or component
that was specified explicitly with a mouse button press or a keyboard event.
.LI gain
The ratio of the distance the pointer moves to the distance the mouse moves.
.LI "graphical\ user\ interface"
A form of communication between users and computers that uses
graphics-oriented software such as windows, Menus, and icons, to facilitate
the interaction.
.LI "graphics-like\ collection"
A 2-dimentional collection of elements where the location cursor can
be either on an element or between two elements.
.LI "grayed\ selection"
A Menu selection that is not currently available and so has been dimmed.
.LI "highlight"
A graphic technique used to provide a visual cue to the current selection
or to the current location of the input focus.  Highlighting is frequently
accomplished by reversing the video of the selection.
.LI "hotspot"
The area of a graphical image used as a pointer or cursor that is defined
as the point of the pointer or cursor.
.LI "hourglass"
A graphical image used to symbolize the passage of time and provide a
visual cue that the application is currently performing an operation.
.LI "I-beam"
A graphical image used to represent the location of the mouse pointer in a
text entry box and providing a visual cue that text can be entered in
an area. 
.LI "icon"
A small graphical image used to represent a window.  Windows can be turned
into icons or minimized to save room or unclutter the workspace.
.LI "icon\ box"
A window for organizing icons.
.LI "inactive"
A window that does not have the input focus.
.LI "insertion\ cursor"
The graphical symbol that provides the visual cue to the location of the
insertion point.
.LI "internal\ navigation"
Moving the keyboard focus within a single control.
.LI "implicit focus"
A keyboard focus model that sends keyboard events to the window or component
that the mouse pointer is over.
.LI "keyboard"
One of many input devices; the traditional method of entering text into an
application.
.LI "keyboard\ focus"
Indicates the window or component with in a window that receives keyboard
input.  It is sometimes called the input focus.
.LI "label"
The text part of an icon or graphical component.
.LI "list\ box"
A component that provides users with a scrollable list of options from which
to choose.
.LI "list-like\ collection"
A collection of elements where the location cursor is placed on an element.
.LI "location\ cursor"
A graphical symbol that marks the current location of the keyboard
input focus for selection.  Typically, this symbol is a box that surrounds
the current object.
.LI "lower"
To move a window to the bottom of the window stack on the workspace.
.LI "maximize"
To enlarge a window to its maximum size.
.LI "maximize\ button"
A control button placed on an MWM window frame and used to initiate the
maximize function.
.LI "Menu"
A list of available selections from which a user chooses.
.LI "Menu\ system"
A collection of Menus cascading from a single CascadeButton.
.LI "MenuBar"
A rectangular area at the top of the client area of a window that contains
the titles of the standard PullDown Menus for that application.
.LI "MenuBar\ system"
A collection of Menus assciated with a MenuBar and the MenuBar itself.
.LI "message\ box"
The generic name for any DialogBox that provides information, gives the
current state of a work in progress, asks a question, issues a warning, or
draws attention to an error.
.LI "minimize"
To turn a window into an icon.  The term iconify is sometimes used
instead of minimize.
.LI "minimize\ button"
A control button placed on an MWM window frame and used to initiate the
minimize function.
.LI "mnemonic"
A single character (frequently the initial character) of a Menu selection
which, when the Menu is displayed and the character is pressed on the
keyboard, initiates the selection.
.br
.ne 4
.LI "modifier\ key"
A key that, when pressed with another key, changes the meaning of the other
key.  
.BR <Ctrl> ,
.BR <Alt> ,
and
.B <Shift>
are modifier keys.
.LI "mouse"
A pointing device commonly used in conjunction with a keyboard in
point-and-click, object-oriented user interfaces.
.LI "mouse\ button"
A button on a mouse pointing device.  Mouse buttons can be
pressed, released, dragged, clicked, and double-clicked.
.LI "multiclick"
To click a mouse button multiple times without moving the pointer.
.LI "multiple\ selection"
A selection model that allows multiple single selections.
.LI "multipress"
To press a mouse button multiple times without moving the pointer.
.LI "multidrag"
To press a mouse button multiple times without moving the pointer
and then drag the mouse pointer.
.LI "open"
To start an action or begin working with a text, data, or graphics file.
.LI "paste"
Inserting data into an area.  Pasting is commonly used in reference to text
files where a block of text is cut from one area and pasted into
another area.
.LI "pointer"
The graphical image that appears on the workspace and represents the
current location of a mouse or other pointing device. 
.LI "pointing\ device"
A device such as a mouse, trackball, or graphics tablet that allows users
to move a pointer about on the workspace and point to graphical objects.
.LI "Popup\ Menu"
A Menu that provides no visual cue to its presence, but simply pops up
when users perform a particular action.  Popup Menus associated with
a particular area of the workspace, such as the client area of an
application, and users must memorize where these areas are.
.LI "press"
To hold down a mouse button or a key.  Note that to hold down a mouse
button and move the mouse is called dragging. 
.LI "previewing"
A means of PushButton activation where information about the
impending action of a button release is displayed on the button
press action.
.LI "primary\ window"
A top-level window of an application.  Primary windows can be minimized.
.LI "PullDown\ Menu"
A Menu that is pulled down from a client application's title bar.
.br
.ne 4
.LI "PushButton"
A graphic component that simulates a real-life PushButton.  People use the
pointer and mouse to push the button and start some action.
.br
.ne 5
.LI "RadioButton"
A graphic component that simulates the buttons on a real-life car radio.
Each button represents a mutually exclusive selection.  RadioButtons are
typically used for setting states or modes.
.LI "range\ selection"
A selection model that allows selection of a range of elements.
.LI "release"
To let up on a mouse button or key that has been pressed.  Sometimes it is
the press that initiates the action; sometimes it is the release.
.LI "resize"
To change the height or width of a window.
.LI "resize\ border"
The MWM window frame part that surrounds the client area of an application and
that is used to change the height or width of the window.
.LI "restore"
To return an icon or maximized window to its normal size.
.LI "root\ window"
See workspace.
.LI "save"
To write changes to a data file to a storage device for safekeeping.
.LI "screen"
The physical Cathode Ray Tube (CRT) that displays information from the 
computer.  In the OSF/Motif environment, in most cases screen and
workspace are synonymous.
.LI "scroll\ bar"
A graphical device used to change a user's viewpoint of a list or data
file.  A scroll bar consists of a slider, scroll area, and scroll arrows.
A user changes the view by sliding the slider up or down in the scroll
area or by pressing one of the scroll arrows.  This causes the view to
scroll up or down in the window adjacent to the scroll bar.
.LI "secondary\ window"
A child window of a primary window.
.LI "select"
To choose an object to be acted upon or an action to be performed.
.br
.ne 5
.LI "selection"
The object or action that is selected.  Menus are composed of selection
items.  Dialog boxes contain components, each of which represents a selection.
.LI "Select\ button"
The mouse button used to make a selection.
.LI "selection\ cursor"
See location cursor.
.LI "single\ selection"
A selection model that allows selection of a single element.
.LI "slider"
One of the graphical components of a scroll bar or scale.  The slider is the
object that is dragged along the scroll area to cause a change.
.LI "state"
A generic term used to describe the condition or mode of an object or action.
.LI "submenu"
A cascading Menu.
.LI "system\ Menu"
See window Menu.
.LI "text\ cursor"
See insertion cursor.
.LI "text-like\ collection"
A collection of elements where the location cursor is
placed between two elements.
.LI "title\ area"
The area at the top of the window frame immediately beneath the resize
border.  The title bar has two functions: it contains a title or name that
identifies the window and it can be grabbed and dragged to relocate the
window. 
.LI "title\ bar"
The bar across the top of an MWM window that consists of the
window Menu button, the title area, and the window-control buttons.
.LI "transient\ window"
A window of short duration such as a DialogBox.  The window is
displayed for only a short time, usually just long enough to convey some
information or get some operational directions. 
.LI "virtual binding"
A vendor independent notation for representing keyboard keys and mouse
buttons.
.LI "window"
A data structure that represents all or part of the CRT display screen.
Visually, a window is represented as a rectangular subset of the display
screen. 
.LI "window\ decoration"
The frame and window-control buttons that surround windows managed by the
a window manager such as the OSF/Motif Window Manager.
.LI "window\ frame"
The area surrounding a window.  A window frame can consist of a resize
border, a window Menu button, a title bar, and window-control buttons.
.br
.ne 6
.LI "window\ manager"
A program that controls the size, placement, and operation of windows
on the workspace.  The window manager includes the functional window
frames that surround each window object and may include a separate Menu for
the workspace.  
.br
.ne 7
.LI "window\ Menu"
The Menu that appears when the window Menu button is pressed.  The window
Menu typically contains selections for restoring, moving, sizing,
minimizing, maximizing, and closing the window.
.LI "window\ Menu\ button"
The graphical control button that appears at the left side of the title bar
in the window frame.
.LI "window\ navigation"
Moving the keyboard focus among windows.
.LI "workspace"
The CRT screen.  The area on which the windows of a user's environment
appear.  The workspace is sometimes called the desk, desktop, or root
window.  
.LE
