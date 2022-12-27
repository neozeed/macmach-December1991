...\"	tocmac1.mm  --  header stuff so that we can generate a toc
...\"	from local .)T calls.   This def gets closed by the tocmac2.mm
...\"	file, which also calls the .TC macro.
...\"
.de >C	\" macro that holds .)T calls
.)T 1 0 "" "Preface" xv
.)T 2 0 "" "Audience" xv
.)T 2 0 "" "Contents" xv
.)T 2 0 "" "Typographical Conventions" xvi
.)T 1 1 "1.\ \ " "Introduction to the OSF/Motif Toolkit" 1 1
.)T 2 2 "1.1\ \ " "The OSF/Motif Toolkit and the X Window System" 1 1
.)T 2 2 "1.2\ \ " "Widget Classes and Hierarchy" 1 1
.)T 2 2 "1.3\ \ " "Compiling Sample Programs" 8 1
.)T 1 1 "2.\ \ " "Widgets, Gadgets, and Convenience Functions" 1 2
.)T 2 2 "2.1\ \ " "Widgets" 1 2
.)T 3 3 "2.1.1\ \ " "Shell Widgets" 3 2
.)T 3 3 "2.1.2\ \ " "Display Widgets" 4 2
.)T 3 3 "2.1.3\ \ " "Container Widgets" 8 2
.)T 3 3 "2.1.4\ \ " "Dialog Widgets" 14 2
.)T 3 3 "2.1.5\ \ " "Dialog Widget Descriptions" 15 2
.)T 3 3 "2.1.6\ \ " "Convenience Dialogs" 15 2
.)T 3 3 "2.1.7\ \ " "Menu Widgets" 17 2
.)T 2 2 "2.2\ \ " "Gadgets" 17 2
.)T 2 2 "2.3\ \ " "Convenience Functions" 19 2
.)T 1 1 "3.\ \ " "Using Motif Widgets in Programs" 1 3
.)T 2 2 "3.1\ \ " "Introduction to the xmbutton Program" 1 3
.)T 3 3 "3.1.1\ \ " "Widget Tree Diagram for the xmbutton Program" 2 3
.)T 3 3 "3.1.2\ \ " "Program Listing for xmbutton.c" 2 3
.)T 3 3 "3.1.3\ \ " "Defaults File XMdemos Partial Listing" 3 3
.)T 3 3 "3.1.4\ \ " "Output Produced by the xmbutton Program" 3 3
.)T 2 2 "3.2\ \ " "Including Header Files" 4 3
.)T 2 2 "3.3\ \ " "Initializing the Xt Intrinsics" 4 3
.)T 2 2 "3.4\ \ " "Creating Argument Lists for Widgets" 6 3
.)T 2 2 "3.5\ \ " "Adding Callback Procedures" 6 3
.)T 3 3 "3.5.1\ \ " "Writing a Callback Procedure" 7 3
.)T 3 3 "3.5.2\ \ " "Adding Callbacks" 7 3
.)T 3 3 "3.5.3\ \ " "Setting Widgets' Callback Resources" 8 3
.)T 2 2 "3.6\ \ " "Creating the Widget" 9 3
.)T 2 2 "3.7\ \ " "Making the Widget Visible" 9 3
.)T 2 2 "3.8\ \ " "Linking Libraries" 10 3
.)T 2 2 "3.9\ \ " "Creating Defaults Files" 10 3
.)T 3 3 "3.9.1\ \ " "Application Defaults Files" 11 3
.)T 3 3 "3.9.2\ \ " "User Defaults Files" 11 3
.)T 3 3 "3.9.3\ \ " "Defaults File Example" 11 3
.)T 2 2 "3.10\ \ " "Using Color" 12 3
.)T 3 3 "3.10.1\ \ " "Visual Capabilities and Attributes" 12 3
.)T 3 3 "3.10.2\ \ " "Using the Capabilities" 13 3
.)T 2 2 "3.11\ \ " "Advanced Programming Techniques" 13 3
.)T 3 3 "3.11.1\ \ " "Setting Argument Values" 13 3
.)T 3 3 "3.11.2\ \ " "Manipulating Created Widgets" 15 3
.)T 2 2 "3.12\ \ " "An Advanced Sample Program" 15 3
.)T 3 3 "3.12.1\ \ " "Windows Used in xmfonts" 16 3
.)T 3 3 "3.12.2\ \ " "Widget Hierarchy" 17 3
.)T 3 3 "3.12.3\ \ " "Source Code" 19 3
.)T 1 1 "4.\ \ " "Shell Widgets" 1 4
.)T 2 2 "4.1\ \ " "Descriptions of Shell Widgets" 1 4
.)T 2 2 "4.2\ \ " "Shell Widget Appearance" 2 4
.)T 1 1 "5.\ \ " "Dialog Widgets and Functions" 1 5
.)T 2 2 "5.1\ \ " "Dialog Widgets and Menus" 1 5
.)T 2 2 "5.2\ \ " "A List of the Dialog Widgets" 1 5
.)T 2 2 "5.3\ \ " "Convenience Dialogs" 2 5
.)T 2 2 "5.4\ \ " "Using Dialogs and Convenience Functions" 3 5
.)T 3 3 "5.4.1\ \ " "XmDialogShell" 3 5
.)T 3 3 "5.4.2\ \ " "XmBulletinBoard" 4 5
.)T 3 3 "5.4.3\ \ " "XmCommand" 5 5
.)T 3 3 "5.4.4\ \ " "XmFileSelectionBox" 8 5
.)T 3 3 "5.4.5\ \ " "XmForm" 9 5
.)T 3 3 "5.4.6\ \ " "XmMessageBox" 12 5
.)T 3 3 "5.4.7\ \ " "XmSelectionBox" 14 5
.)T 1 1 "6.\ \ " "Menus" 1 6
.)T 2 2 "6.1\ \ " "Overview of the Motif Menu System" 1 6
.)T 3 3 "6.1.1\ \ " "Convenience Functions and Widgets Used to Create Menus" 1 6
.)T 3 3 "6.1.2\ \ " "Introducing the Three Menu Types" 3 6
.)T 2 2 "6.2\ \ " "Creating Popup Menu Systems" 5 6
.)T 3 3 "6.2.1\ \ " "Popup MenuPane Convenience Function" 6 6
.)T 3 3 "6.2.2\ \ " "Event Handlers for Popup Menu Systems" 6 6
.)T 3 3 "6.2.3\ \ " "Procedure for Creating a Popup Menu" 6 6
.)T 3 3 "6.2.4\ \ " "Interacting with Popup Menus" 8 6
.)T 3 3 "6.2.5\ \ " "Sample Program" 9 6
.)T 2 2 "6.3\ \ " "Creating a Pulldown Menu System" 10 6
.)T 3 3 "6.3.1\ \ " "MenuBar Create Function" 10 6
.)T 3 3 "6.3.2\ \ " "Pulldown MenuPane Create Function" 11 6
.)T 3 3 "6.3.3\ \ " "Creating a Help Button" 11 6
.)T 3 3 "6.3.4\ \ " "Procedure for Creating a Pulldown Menu" 11 6
.)T 3 3 "6.3.5\ \ " "Interacting with Pulldown Menus" 13 6
.)T 3 3 "6.3.6\ \ " "Sample Program" 13 6
.)T 2 2 "6.4\ \ " "Creating Submenus" 16 6
.)T 3 3 "6.4.1\ \ " "Procedure for Creating Submenus" 16 6
.)T 3 3 "6.4.2\ \ " "Interacting with Submenus" 17 6
.)T 3 3 "6.4.3\ \ " "Sample Program" 17 6
.)T 2 2 "6.5\ \ " "Creating Option Menu Systems" 19 6
.)T 3 3 "6.5.1\ \ " "Option MenuPane Create Function" 20 6
.)T 3 3 "6.5.2\ \ " "Procedure for Creating an Option Menu" 20 6
.)T 3 3 "6.5.3\ \ " "Interacting with Option Menus" 21 6
.)T 3 3 "6.5.4\ \ " "Sample Program" 21 6
.)T 2 2 "6.6\ \ " "Selecting a Menu Cursor" 23 6
.)T 2 2 "6.7\ \ " "Creating Menus Without Convenience Functions" 25 6
.)T 3 3 "6.7.1\ \ " "Functions for Creating Menus" 25 6
.)T 3 3 "6.7.2\ \ " "Parenting Relationships" 25 6
.)T 3 3 "6.7.3\ \ " "Sample Program" 27 6
.)T 1 1 "7.\ \ " "Specialized Widgets" 1 7
.)T 2 2 "7.1\ \ " "List Widget" 1 7
.)T 3 3 "7.1.1\ \ " "List Functions" 1 7
.)T 3 3 "7.1.2\ \ " "Using the List Widget" 3 7
.)T 2 2 "7.2\ \ " "RowColumn Widget" 10 7
.)T 3 3 "7.2.1\ \ " "RowColumn Types" 10 7
.)T 3 3 "7.2.2\ \ " "RowColumn Functions" 10 7
.)T 3 3 "7.2.3\ \ " "Layout" 11 7
.)T 2 2 "7.3\ \ " "Text Widget" 14 7
.)T 3 3 "7.3.1\ \ " "Text Functions" 14 7
.)T 3 3 "7.3.2\ \ " "Using the Text Widget in a Program" 15 7
.)T 1 1 "8.\ \ " "Additional Functionality" 1 8
.)T 2 2 "8.1\ \ " "Compound Strings" 1 8
.)T 3 3 "8.1.1\ \ " "Components of a Compound String" 2 8
.)T 3 3 "8.1.2\ \ " "Compound String Functions" 3 8
.)T 3 3 "8.1.3\ \ " "A Sample Program" 12 8
.)T 2 2 "8.2\ \ " " Cut and Paste Functions" 13 8
.)T 3 3 "8.2.1\ \ " "Clipboard Copy Functions" 13 8
.)T 3 3 "8.2.2\ \ " "Clipboard Inquire Functions" 18 8
.)T 3 3 "8.2.3\ \ " "Clipboard Retrieve Functions" 21 8
.)T 3 3 "8.2.4\ \ " "Miscellaneous Clipboard Functions" 23 8
.)T 2 2 "8.3\ \ " "Dynamic Resource Defaulting" 25 8
.)T 2 2 "8.4\ \ " "Localization" 25 8
.)T 2 2 "8.5\ \ " "Pixmap Caching Functions" 27 8
.)T 2 2 "8.6\ \ " "Resolution Independence" 29 8
.)T 3 3 "8.6.1\ \ " "The Resolution Independence Mechanism" 29 8
.)T 3 3 "8.6.2\ \ " "Setting the Font Units" 30 8
.)T 3 3 "8.6.3\ \ " "Converting Between Unit Types" 30 8
.)T 2 2 "8.7\ \ " "Interacting With the Motif Window Manager" 31 8
.)T 3 3 "8.7.1\ \ " "Protocol Management" 31 8
.)T 3 3 "8.7.2\ \ " "Protocol Manager Functions" 32 8
.)T 3 3 "8.7.3\ \ " "Atom Management" 34 8
.)T 2 2 "8.8\ \ " "Motif Version Number" 35 8
.)T 2 2 "8.9\ \ " "Motif Window Manager Presence" 35 8
.)T 1 1 "9.\ \ " "Keyboard Interface" 1 9
.)T 2 2 "9.1\ \ " "Keyboard Focus Models" 1 9
.)T 2 2 "9.2\ \ " "Grouping Widgets Into Tab Groups" 1 9
.)T 2 2 "9.3\ \ " "Traversal Within and Between Tab Groups" 2 9
.)T 2 2 "9.4\ \ " "Keyboard Input Processing to a Widget" 3 9
.)T 1 1 "10.\ \ " "Introduction to the OSF/Motif Window Manager" 1 10
.)T 2 2 "10.1\ \ " "Conventions" 1 10
.)T 2 2 "10.2\ \ " "Some Window Manager Precautions" 2 10
.)T 1 1 "11.\ \ " "Understanding the Principles of Window Management" 1 11
.)T 2 2 "11.1\ \ " "Getting User Input" 1 11
.)T 3 3 "11.1.1\ \ " "Using a Pointer" 1 11
.)T 3 3 "11.1.2\ \ " "Using a Keyboard" 2 11
.)T 3 3 "11.1.3\ \ " "Modifying Default Behavior" 2 11
.)T 2 2 "11.2\ \ " "Using Focus Policies:  Keyboard Input and Colormaps" 3 11
.)T 3 3 "11.2.1\ \ " "Setting the Colormap Focus Policy " 4 11
.)T 2 2 "11.3\ \ " "Distinguishing Windows by Type" 4 11
.)T 3 3 "11.3.1\ \ " "Choosing the Right Type of Client Window" 4 11
.)T 3 3 "11.3.2\ \ " "Input Modes" 5 11
.)T 3 3 "11.3.3\ \ " "Understanding Window Manager Windows" 5 11
.)T 2 2 "11.4\ \ " "Understanding the Parts of the MWM Window Frame" 7 11
.)T 3 3 "11.4.1\ \ " "Coloring Frame Components" 7 11
.)T 3 3 "11.4.2\ \ " "Understanding Frame Components and Their Behavior" 8 11
.)T 3 3 "11.4.3\ \ " "Selecting From the Window Menu" 12 11
.)T 2 2 "11.5\ \ " "Placing and Sizing Windows" 14 11
.)T 3 3 "11.5.1\ \ " "Stacking Windows" 15 11
.)T 2 2 "11.6\ \ " "Introduction to Icons" 15 11
.)T 1 1 "12.\ \ " "Communicating Between MWM and Clients: MWM Properties" 1 12
.)T 2 2 "12.1\ \ " "MWM Programmatic Interface Standards" 1 12
.)T 2 2 "12.2\ \ " "Inter-Client Communication Conventions" 1 12
.)T 3 3 "12.2.1\ \ " "Programming Client Actions" 1 12
.)T 3 3 "12.2.2\ \ " "Client Responses to MWM Actions" 6 12
.)T 2 2 "12.3\ \ " "MWM Specific Information" 7 12
.)T 3 3 "12.3.1\ \ " "The _MOTIF_WM_HINTS Property" 7 12
.)T 3 3 "12.3.2\ \ " "Window Management Calls" 10 12
.)T 1 1 "13.\ \ " "Managing Windows With MWM" 1 13
.)T 2 2 "13.1\ \ " "Starting Up MWM" 1 13
.)T 2 2 "13.2\ \ " "Using Resources to Configure MWM" 2 13
.)T 3 3 "13.2.1\ \ " "The Hierarchy of Resource Configuration Files" 2 13
.)T 3 3 "13.2.2\ \ " "Classification of Resources" 2 13
.)T 3 3 "13.2.3\ \ " "Order of Precedence" 3 13
.)T 3 3 "13.2.4\ \ " "The Syntax for Declaring Resources" 3 13
.)T 2 2 "13.3\ \ " "Managing Screens" 4 13
.)T 2 2 "13.4\ \ " "Setting Focus Policies for Active Windows" 4 13
.)T 3 3 "13.4.1\ \ " "Colormap Focus Policy" 4 13
.)T 3 3 "13.4.2\ \ " "Keyboard Focus Policy" 4 13
.)T 3 3 "13.4.3\ \ " "The Syntax of Focus Policy Resources" 5 13
.)T 2 2 "13.5\ \ " "Managing Clients" 6 13
.)T 2 2 "13.6\ \ " "Managing the Appearance and Behavior of Window Frames and Mattes" 6 13
.)T 2 2 "13.7\ \ " "Specifying a Different Font for the Window Manager" 7 13
.)T 2 2 "13.8\ \ " "Coloring Window Frames" 7 13
.)T 3 3 "13.8.1\ \ " "Coloring Individual Frame Elements" 9 13
.)T 3 3 "13.8.2\ \ " "Syntax for Resources Controlling Frame Appearance Element" 9 13
.)T 2 2 "13.9\ \ " "Making Window Frame Pixmaps" 10 13
.)T 2 2 "13.10\ \ " "Specifying a Matte for Client Windows" 11 13
.)T 3 3 "13.10.1\ \ " "Coloring Individual Matte Elements" 12 13
.)T 3 3 "13.10.2\ \ " "Changing the Pixmap of Mattes" 12 13
.)T 3 3 "13.10.3\ \ " "The Syntax for Matte Resources" 12 13
.)T 2 2 "13.11\ \ " "Using Frameless or Reduced-Element Windows" 13 13
.)T 3 3 "13.11.1\ \ " "Adding or Removing Elements" 14 13
.)T 3 3 "13.11.2\ \ " "The Syntax for clientDecoration and transientDecoration Resources" 14 13
.)T 2 2 "13.12\ \ " "Controlling Window Size and Position" 15 13
.)T 3 3 "13.12.1\ \ " "Window Size Resources" 15 13
.)T 3 3 "13.12.2\ \ " "Window Position Resources" 16 13
.)T 3 3 "13.12.3\ \ " "Other Resources" 17 13
.)T 3 3 "13.12.4\ \ " "The Syntax for Size and Position Refinement Resources" 17 13
.)T 1 1 "14.\ \ " "Managing Menus, Mouse Buttons, and Keyboard Bindings" 1 14
.)T 2 2 "14.1\ \ " "MWM Resource Description File" 1 14
.)T 2 2 "14.2\ \ " "Modifying Menus and Default Menus" 2 14
.)T 2 2 "14.3\ \ " "Making New Menus" 3 14
.)T 3 3 "14.3.1\ \ " "Menu Titles" 3 14
.)T 3 3 "14.3.2\ \ " "Menu Items" 3 14
.)T 3 3 "14.3.3\ \ " "Mnemonics and Accelerators" 4 14
.)T 3 3 "14.3.4\ \ " "MWM Functions" 4 14
.)T 3 3 "14.3.5\ \ " "Size of Menu Window" 8 14
.)T 3 3 "14.3.6\ \ " "Binding a Menu to a Mouse Button or Key" 9 14
.)T 2 2 "14.4\ \ " "Changing the Menu Associated with the Window Menu Button" 9 14
.)T 3 3 "14.4.1\ \ " "Size of Menu Window" 11 14
.)T 3 3 "14.4.2\ \ " "Binding a Menu to a Mouse Button or Key" 11 14
.)T 2 2 "14.5\ \ " "Changing the Menu Associated with the Window Menu Button" 11 14
.)T 2 2 "14.6\ \ " "Mouse Button Bindings" 12 14
.)T 3 3 "14.6.1\ \ " "Default Button Bindings" 12 14
.)T 3 3 "14.6.2\ \ " "Button Binding Syntax and Modifying Button Bindings" 13 14
.)T 3 3 "14.6.3\ \ " "Making a New Button Binding Set" 15 14
.)T 3 3 "14.6.4\ \ " "Modifying Button Click Timing" 16 14
.)T 2 2 "14.7\ \ " "Keyboard Bindings" 16 14
.)T 3 3 "14.7.1\ \ " "Default Keyboard Bindings" 16 14
.)T 3 3 "14.7.2\ \ " "Keyboard Binding Syntax and Modifying Keyboard Bindings" 16 14
.)T 3 3 "14.7.3\ \ " "Making a New Button Binding Set" 18 14
.)T 3 3 "14.7.4\ \ " "Modifying Button Click Timing" 19 14
.)T 2 2 "14.8\ \ " "Keyboard Bindings" 19 14
.)T 3 3 "14.8.1\ \ " "Default Keyboard Bindings" 19 14
.)T 3 3 "14.8.2\ \ " "Keyboard Binding Syntax and Modifying Keyboard Bindings" 19 14
.)T 3 3 "14.8.3\ \ " "Making a New Keyboard Binding Set" 20 14
.)T 1 1 "15.\ \ " "Working with Icons" 1 15
.)T 2 2 "15.1\ \ " "Studying Icon Anatomy" 1 15
.)T 3 3 "15.1.1\ \ " "The Image" 1 15
.)T 3 3 "15.1.2\ \ " "The Label" 2 15
.)T 2 2 "15.2\ \ " "Manipulating Icons" 2 15
.)T 3 3 "15.2.1\ \ " "Controlling Icon Placement" 4 15
.)T 3 3 "15.2.2\ \ " "The Syntax for Icon Placement Resources" 5 15
.)T 2 2 "15.3\ \ " "Controlling Icon Appearance and Behavior" 5 15
.)T 3 3 "15.3.1\ \ " "Selecting Icon Decoration" 5 15
.)T 3 3 "15.3.2\ \ " "Sizing Icons" 5 15
.)T 3 3 "15.3.3\ \ " "The Syntax for Resources that Control Icon Appearance" 6 15
.)T 2 2 "15.4\ \ " "Coloring Icons" 6 15
.)T 3 3 "15.4.1\ \ " "The Syntax for Icon Coloring Resources" 7 15
.)T 2 2 "15.5\ \ " "Using the Icon Box to Hold Icons" 8 15
.)T 3 3 "15.5.1\ \ " "Controlling Icon Box Appearance and Behavior" 8 15
.)T 3 3 "15.5.2\ \ " "Controlling Icons in the Icon Box" 9 15
.)T 1 1 "16.\ \ " "Quick Reference Tables" 1 16
.)T 2 2 "16.1\ \ " "Syntax for Resources" 1 16
.)T 2 2 "16.2\ \ " "Resource Tables" 1 16
.)T 3 3 "16.2.1\ \ " "Coloring Windows, Icons, Menus, and Mattes" 2 16
.)T 3 3 "16.2.2\ \ " "Shading" 2 16
.)T 3 3 "16.2.3\ \ " "Window Decorations" 3 16
.)T 3 3 "16.2.4\ \ " "Focus Policies" 4 16
.)T 3 3 "16.2.5\ \ " "Controlling Windows" 5 16
.)T 3 3 "16.2.6\ \ " "Controlling Window Size and Position" 6 16
.)T 3 3 "16.2.7\ \ " "Configuring Icons" 7 16
.)T 3 3 "16.2.8\ \ " "Configuring the Icon Box" 7 16
.)T 2 2 "16.3\ \ " "MWM Functions" 8 16
.)T 1 1 "17.\ \ " "Introduction to the User Interface Language" 1 17
.)T 2 2 "17.1\ \ " "Overview of UIL and MRM" 1 17
.)T 2 2 "17.2\ \ " "Benefits of Using UIL and MRM" 1 17
.)T 3 3 "17.2.1\ \ " "Easier Coding" 1 17
.)T 3 3 "17.2.2\ \ " "Earlier Error Detection" 2 17
.)T 3 3 "17.2.3\ \ " "Separation of Form and Function" 2 17
.)T 3 3 "17.2.4\ \ " "Faster Prototype Development" 2 17
.)T 3 3 "17.2.5\ \ " "Interface Customization" 2 17
.)T 2 2 "17.3\ \ " "Features of UIL" 2 17
.)T 3 3 "17.3.1\ \ " "Named Values" 3 17
.)T 3 3 "17.3.2\ \ " "Compile-Time Expressions" 3 17
.)T 3 3 "17.3.3\ \ " "Identifiers" 3 17
.)T 3 3 "17.3.4\ \ " "Lists" 3 17
.)T 3 3 "17.3.5\ \ " "Support for Compound Strings" 3 17
.)T 1 1 "18.\ \ " "UIL Language Syntax" 1 18
.)T 2 2 "18.1\ \ " "Character Set" 1 18
.)T 3 3 "18.1.1\ \ " "Punctuation Characters" 2 18
.)T 2 2 "18.2\ \ " "Names" 2 18
.)T 2 2 "18.3\ \ " "Keywords" 3 18
.)T 2 2 "18.4\ \ " "Literals " 6 18
.)T 3 3 "18.4.1\ \ " "String Literals" 6 18
.)T 3 3 "18.4.2\ \ " "Concatenated String Literals" 8 18
.)T 3 3 "18.4.3\ \ " "Integer Literals" 11 18
.)T 3 3 "18.4.4\ \ " "Boolean Literals" 12 18
.)T 3 3 "18.4.5\ \ " "Floating-Point Literals" 12 18
.)T 2 2 "18.5\ \ " "Value-Generating Functions" 12 18
.)T 3 3 "18.5.1\ \ " "The CHARACTER_SET Function" 13 18
.)T 3 3 "18.5.2\ \ " "The KEYSYM Function" 13 18
.)T 3 3 "18.5.3\ \ " "Functions for Specifying Colors" 13 18
.)T 3 3 "18.5.4\ \ " "Functions for Specifying Pixmaps" 15 18
.)T 3 3 "18.5.5\ \ " "The SINGLE_FLOAT Function" 17 18
.)T 3 3 "18.5.6\ \ " "The FLOAT Function" 17 18
.)T 3 3 "18.5.7\ \ " "The FONT Function" 17 18
.)T 3 3 "18.5.8\ \ " "The FONT_TABLE Function" 18 18
.)T 3 3 "18.5.9\ \ " "The COMPOUND_STRING Function" 18 18
.)T 3 3 "18.5.10\ \ " "The COMPOUND_STRING_TABLE Function" 19 18
.)T 3 3 "18.5.11\ \ " "The ASCIZ_STRING_TABLE Function" 19 18
.)T 3 3 "18.5.12\ \ " "The INTEGER_TABLE Function" 19 18
.)T 3 3 "18.5.13\ \ " "The ARGUMENT Function" 20 18
.)T 3 3 "18.5.14\ \ " "The REASON Function" 22 18
.)T 3 3 "18.5.15\ \ " "The TRANSLATION_TABLE Function" 22 18
.)T 2 2 "18.6\ \ " "The any Data Type" 23 18
.)T 2 2 "18.7\ \ " "Compile-Time Value Expressions" 24 18
.)T 1 1 "19.\ \ " "UIL Module Structure" 1 19
.)T 2 2 "19.1\ \ " "Structure of a UIL Module" 1 19
.)T 3 3 "19.1.1\ \ " "Version Clause" 2 19
.)T 3 3 "19.1.2\ \ " "Case-Sensitivity Clause" 2 19
.)T 3 3 "19.1.3\ \ " "Default Character Set Clause" 2 19
.)T 3 3 "19.1.4\ \ " "Default Object Variant Clause" 3 19
.)T 2 2 "19.2\ \ " "Scope of References to Values and Objects" 3 19
.)T 2 2 "19.3\ \ " "Structure of a Value Section" 4 19
.)T 2 2 "19.4\ \ " "Structure of a Procedure Section" 5 19
.)T 2 2 "19.5\ \ " "Structure of a List Section" 7 19
.)T 3 3 "19.5.1\ \ " "Arguments List Structure" 7 19
.)T 3 3 "19.5.2\ \ " "Callbacks List Structure" 8 19
.)T 3 3 "19.5.3\ \ " "Controls List Structure" 9 19
.)T 3 3 "19.5.4\ \ " "Procedures List Structure" 9 19
.)T 2 2 "19.6\ \ " "Structure of an Object Section" 10 19
.)T 3 3 "19.6.1\ \ " "Specifying the Object Variant in the Module Header" 11 19
.)T 3 3 "19.6.2\ \ " "Specifying the Object Variant in the Object Declaration" 12 19
.)T 2 2 "19.7\ \ " "Structure of an Identifier Section" 13 19
.)T 2 2 "19.8\ \ " "Include Directive" 14 19
.)T 2 2 "19.9\ \ " "Definitions for Constraint Arguments" 14 19
.)T 2 2 "19.10\ \ " "Symbolic Referencing of Widget IDs" 15 19
.)T 1 1 "20.\ \ " "Using the UIL Compiler " 1 20
.)T 2 2 "20.1\ \ " "Invoking the Compiler" 1 20
.)T 3 3 "20.1.1\ \ " "Invoking the Compiler by Command" 1 20
.)T 3 3 "20.1.2\ \ " "Invoking the Compiler by Function" 2 20
.)T 2 2 "20.2\ \ " "Default Character Set for String Literals" 5 20
.)T 2 2 "20.3\ \ " "Interpreting Diagnostics Issued by the Compiler" 6 20
.)T 2 2 "20.4\ \ " "Reading the Compiler Listing" 6 20
.)T 3 3 "20.4.1\ \ " "Title" 7 20
.)T 3 3 "20.4.2\ \ " "Source Line" 7 20
.)T 3 3 "20.4.3\ \ " "Diagnostics" 7 20
.)T 3 3 "20.4.4\ \ " "Summaries" 8 20
.)T 1 1 "21.\ \ " "Motif Resource Manager Functions" 1 21
.)T 2 2 "21.1\ \ " "Setting Up Storage and Data Structures" 1 21
.)T 2 2 "21.2\ \ " "Obtaining UID Database File IDs" 1 21
.)T 2 2 "21.3\ \ " "Closing a MRM Search Hierarchy" 3 21
.)T 2 2 "21.4\ \ " "Registering MRM Information and Callbacks" 4 21
.)T 3 3 "21.4.1\ \ " "Registering MRM Information" 4 21
.)T 3 3 "21.4.2\ \ " "Registering a Vector of Callback Functions" 4 21
.)T 2 2 "21.5\ \ " "Fetching Widgets" 6 21
.)T 3 3 "21.5.1\ \ " "Fetching Values Stored in UID Files" 6 21
.)T 3 3 "21.5.2\ \ " "Fetching Indexed Application Widgets" 7 21
.)T 3 3 "21.5.3\ \ " "Overriding MrmFetchWidget Arguments" 8 21
.)T 2 2 "21.6\ \ " "Fetching Literals" 9 21
.)T 3 3 "21.6.1\ \ " "Fetching a Named Color Literal" 9 21
.)T 3 3 "21.6.2\ \ " "Fetching an Icon Literal" 10 21
.)T 3 3 "21.6.3\ \ " "Fetching a Literal Value" 11 21
.)T 1 1 "22.\ \ " "Creating User Interfaces with UIL and MRM" 1 22
.)T 2 2 "22.1\ \ " "Specifying a User Interface Using UIL" 1 22
.)T 3 3 "22.1.1\ \ " "Creating a UIL Specification File" 1 22
.)T 3 3 "22.1.2\ \ " "Declaring the UIL Module" 2 22
.)T 3 3 "22.1.3\ \ " "Using Constants Defined in the Motif Toolkit" 3 22
.)T 3 3 "22.1.4\ \ " "Declaring Procedures" 3 22
.)T 3 3 "22.1.5\ \ " "Declaring Values" 5 22
.)T 3 3 "22.1.6\ \ " "Declaring Interface Objects in a UIL Module" 9 22
.)T 3 3 "22.1.7\ \ " "Using an Icon as a Widget Label" 11 22
.)T 3 3 "22.1.8\ \ " "Recommended Coding Techniques" 13 22
.)T 2 2 "22.2\ \ " "Creating a User Interface at Run Time with \s-1MRM\s0" 15 22
.)T 3 3 "22.2.1\ \ " "Accessing the UID File at Run Time" 18 22
.)T 3 3 "22.2.2\ \ " "Deferring Fetching" 22 22
.)T 3 3 "22.2.3\ \ " "Getting Literal Values from UID Files" 22 22
.)T 3 3 "22.2.4\ \ " "Setting Values at Run Time Using UID Resources" 23 22
.)T 3 3 "22.2.5\ \ " "Using an Object Definition as a Template" 27 22
.)T 2 2 "22.3\ \ " "Customizing a Motif Interface Using UIL and MRM" 28 22
.)T 2 2 "22.4\ \ " "Using UIL on Large Projects" 29 22
.)T 2 2 "22.5\ \ " "Working with User-Defined Widgets in UIL" 30 22
.)T 3 3 "22.5.1\ \ " "Defining Arguments and Reasons for a User-Defined Widget" 30 22
.)T 3 3 "22.5.2\ \ " "Using a User-Defined Widget in an Interface Specification" 31 22
.)T 3 3 "22.5.3\ \ " "Accessing a User-Defined Widget at Run Time" 33 22
.)T 1 1 "23.\ \ " "The Widget Meta-Language Facility" 1 23
.)T 2 2 "23.1\ \ " "Using WML" 1 23
.)T 3 3 "23.1.1\ \ " "Building WML" 2 23
.)T 3 3 "23.1.2\ \ " "Running WML" 2 23
.)T 3 3 "23.1.3\ \ " "Installing UIL" 3 23
.)T 3 3 "23.1.4\ \ " "Building UIL" 3 23
.)T 2 2 "23.2\ \ " "Modifying WML files" 4 23
.)T 3 3 "23.2.1\ \ " "WML Syntax" 4 23
.)T 1 1 "A.\ \ " "Constraint Arguments" 1 A
.)T 1 1 "B.\ \ " "UIL Built-In Tables" 1 B
.)T 1 1 "C.\ \ " "UIL Arguments" 1 C
.)T 1 1 "D.\ \ " "UIL Diagnostic Messages" 1 D
.)T 1 0 "" "Glossary" 1 GL
.)T 1 0 "" "Index" 1 Index
...\"	tocmac2.mm  --  generate the table of contents.  This file is
...\"	the complement to tocmac1.mm.
...\"
...\"	close the definition started in tocmac1.mm:
..
...\"
...\"	call the table of contents
.TC 1 1 3 0
...\".EH ""
...\".OH ""
...\".PH ""
...\".bp
...\".af `p \\gP
...\".af P 1
...\".nr `p \\nP
...\".af P \\g(`p
...\".af `p 1
...\".if t \{\ .if !\\n(`p%2 \{\
...\".rs
...\".EF ""
...\".OF ""
...\".PF ""
...\"\ \" eject blank page
...\".bp
...\"'br\}
