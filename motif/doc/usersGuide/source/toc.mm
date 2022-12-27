...\"	tocmac1.mm  --  header stuff so that we can generate a toc
...\"	from local .)T calls.   This def gets closed by the tocmac2.mm
...\"	file, which also calls the .TC macro.
...\"
.de >C	\" macro that holds .)T calls
.)T 1 0 "" "Preface" vii
.)T 2 0 "" "Are You Ready?" viii
.)T 2 0 "" "Tips for Using This Guide" ix
.)T 3 0 "" "Work the Examples" ix
.)T 3 0 "" "After the Examples" ix
.)T 3 0 "" "Pressing the Correct Mouse Button" x
.)T 3 0 "" "Typographical Conventions" x
.)T 2 0 "" "Resetting Motif Behavior" xi
.)T 2 0 "" "Other Motif Documentation" xii
.)T 1 1 "1.\ \ " "Introducing Motif" 1 1
.)T 2 2 "1.1\ \ " "Introducing the Motif Window Manager" 2 1
.)T 3 3 "1.1.1\ \ " "Starting the Window Manager" 2 1
.)T 3 3 "1.1.2\ \ " "The Default Environment" 3 1
.)T 2 2 "1.2\ \ " "Using the Mouse" 4 1
.)T 3 3 "1.2.1\ \ " "Pointer Shapes" 4 1
.)T 3 3 "1.2.2\ \ " "Clicking, Double-Clicking, and Dragging" 4 1
.)T 2 2 "1.3\ \ " "Selecting a Window" 5 1
.)T 2 2 "1.4\ \ " "Moving a Window" 6 1
.)T 3 0 "" "Step 1: Grabbing the Title Bar With the Pointer" 6 1
.)T 3 0 "" "Step 2: Dragging the Window to a New Location" 7 1
.)T 3 0 "" "Step 3: Releasing the Window" 7 1
.)T 2 2 "1.5\ \ " "Changing the Size of a Window" 8 1
.)T 3 0 "" "Step 1: Grabbing the Frame With the Pointer" 8 1
.)T 3 0 "" "Step 2: Enlarging or Shrinking the Window" 9 1
.)T 3 0 "" "Step 3: Releasing the Frame" 9 1
.)T 2 2 "1.6\ \ " "Changing a Window Into an Icon" 10 1
.)T 3 0 "" "Step 1: Locating the Minimize Button" 10 1
.)T 3 0 "" "Step 2: Pressing the Minimize Button" 10 1
.)T 3 3 "1.6.3\ \ " "Moving an Icon" 11 1
.)T 2 2 "1.7\ \ " "Restoring a Window From an Icon" 12 1
.)T 3 0 "" "Step 1: Pointing to an Icon to Restore" 12 1
.)T 3 0 "" "Step 2: Changing the Icon Into a Window" 13 1
.)T 2 2 "1.8\ \ " "Displaying a Window Menu and Making Selections" 14 1
.)T 3 0 "" "Step 1: Displaying the Window Menu" 14 1
.)T 3 0 "" "Step 2: Choosing a Function From the Menu" 14 1
.)T 3 0 "" "Step 3: Restoring the Window to its Original Size" 15 1
.)T 3 3 "1.8.4\ \ " "Summary of Window Menu Functions" 15 1
.)T 2 2 "1.9\ \ " "Raising a Window" 16 1
.)T 3 0 "" "Step 1: Positioning the Pointer" 16 1
.)T 3 0 "" "Step 2: Pressing the Select Button" 17 1
.)T 2 2 "1.10\ \ " "Quitting Motif" 18 1
.)T 3 0 "" "Step 1: Preparing to Exit" 18 1
.)T 3 0 "" "Step 2: Stopping X and the MWM" 18 1
.)T 1 1 "2.\ \ " "Using a Motif Application" 1 2
.)T 2 2 "2.1\ \ " "Getting Ready" 2 2
.)T 3 3 "2.1.1\ \ " "Copying sys.Xdefaults Into Your Home Directory" 2 2
.)T 3 3 "2.1.2\ \ " "Starting the Motif Resource Editor Program" 2 2
.)T 3 3 "2.1.3\ \ " "Recognizing Common Controls" 3 2
.)T 2 2 "2.2\ \ " "Using a List" 4 2
.)T 3 3 "2.2.1\ \ " "Scrolling the List" 4 2
.)T 3 3 "2.2.2\ \ " "Selecting an Item in the List" 5 2
.)T 2 2 "2.3\ \ " "Using Push Buttons" 6 2
.)T 3 3 "2.3.1\ \ " "Pressing a Push Button" 6 2
.)T 3 3 "2.3.2\ \ " "Using the <Return> Key: A Short Cut to the Default Push Button" 7 2
.)T 3 3 "2.3.3\ \ " "Double-Clicking: Another Short Cut to the Default Push Button" 7 2
.)T 2 2 "2.4\ \ " "Entering Text Into an Input Field" 8 2
.)T 3 0 "" "Step 1: Selecting the Input Field" 8 2
.)T 3 0 "" "Step 2: Typing the Text" 9 2
.)T 3 0 "" "Step 3: Accepting Your Input" 9 2
.)T 2 2 "2.5\ \ " "Exploring Pull-Down Menus" 10 2
.)T 2 2 "2.6\ \ " "Using a Cascade Menu" 12 2
.)T 3 0 "" "Step 1: Displaying the Menu" 12 2
.)T 3 0 "" "Step 2: Displaying a Cascade Menu" 12 2
.)T 3 0 "" "Step 3: Choosing a Function From a Cascade Menu" 13 2
.)T 2 2 "2.7\ \ " "Using Check Boxes and Radio Buttons" 14 2
.)T 3 3 "2.7.1\ \ " "Using Check Boxes" 14 2
.)T 3 3 "2.7.2\ \ " "Using Radio Buttons" 15 2
.)T 2 2 "2.8\ \ " "Saving a File" 16 2
.)T 3 3 "2.8.1\ \ " "Saving the Current File" 16 2
.)T 3 3 "2.8.2\ \ " "Saving as Another File" 17 2
.)T 3 3 "2.8.3\ \ " "Viewing the Results" 17 2
.)T 2 2 "2.9\ \ " "Reacting to Messages" 18 2
.)T 3 3 "2.9.1\ \ " "Creating an Error" 18 2
.)T 3 3 "2.9.2\ \ " "Acknowledging the Error" 19 2
.)T 2 2 "2.10\ \ " "Taking Short Cuts With Menus" 20 2
.)T 3 3 "2.10.1\ \ " "Using an Accelerator Key" 20 2
.)T 3 3 "2.10.2\ \ " "Displaying a Menu Using Its Mnemonic" 21 2
.)T 3 3 "2.10.3\ \ " "Choosing a Function Using Its Mnemonic" 21 2
.)T 3 3 "2.10.4\ \ " "Navigating Through Menus With the Cursor Keys" 21 2
.)T 2 2 "2.11\ \ " "A Summary of Menu Short Cuts" 22 2
.)T 3 3 "2.11.1\ \ " "Displaying a Menu" 22 2
.)T 3 3 "2.11.2\ \ " "Choosing a Menu Function" 22 2
.)T 1 1 "3.\ \ " "Customizing Your Motif Environment" 1 3
.)T 2 2 "3.1\ \ " "Changing Colors and Fonts Manually" 2 3
.)T 3 3 "3.1.1\ \ " "Editing Your .Xdefaults File" 2 3
.)T 3 3 "3.1.2\ \ " "Specifying Colors and Fonts on the Command Line" 3 3
.)T 2 2 "3.2\ \ " "Activating the Root Menu" 4 3
.)T 3 0 "" "Step 1: Copying system.mwmrc into Your Home Directory" 4 3
.)T 3 0 "" "Step 2: Editing Your .mwmrc File" 4 3
.)T 3 0 "" "Step 3: Restarting MWM" 5 3
.)T 3 3 "3.2.4\ \ " "Using the Root Menu" 5 3
.)T 2 2 "3.3\ \ " "Starting Clients Automatically" 6 3
.)T 3 0 "" "Step 1: Editing Your Startup File" 6 3
.)T 3 0 "" "Step 2: Viewing the Result" 7 3
.)T 1 1 "A.\ \ " "Specifying Colors" 1 A
.)T 1 1 "B.\ \ " "A List of Font Names" 1 B
.)T 1 1 "C.\ \ " "Clients That Help You Manage Windows" 1 C
.)T 2 0 "" "Resetting Environment Variables With resize" 2 C
.)T 3 0 "" "When to Use resize" 2 C
.)T 3 0 "" "Examples" 2 C
.)T 2 0 "" "Repainting the Screen With xrefresh" 3 C
.)T 3 0 "" "When to Use xrefresh" 3 C
.)T 3 0 "" "Examples" 3 C
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
