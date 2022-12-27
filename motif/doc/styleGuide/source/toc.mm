...\"	tocmac1.mm  --  header stuff so that we can generate a toc
...\"	from local .)T calls.   This def gets closed by the tocmac2.mm
...\"	file, which also calls the .TC macro.
...\"
.de >C	\" macro that holds .)T calls
.)T 1 0 "" "Preface" vii
.)T 2 0 "" "Audience" vii
.)T 2 0 "" "Contents" vii
.)T 2 0 "" "Typographic Conventions" viii
.)T 2 0 "" "Compliance Conventions" ix
.)T 1 1 "1.\ \ " "User Interface Design Principles" 1 1
.)T 2 2 "1.1\ \ " "Adopt the User's Perspective" 1 1
.)T 2 2 "1.2\ \ " "Give the User Control" 2 1
.)T 3 3 "1.2.1\ \ " "Keep Interfaces Flexible" 2 1
.)T 3 3 "1.2.2\ \ " "Use Progressive Disclosure" 2 1
.)T 2 2 "1.3\ \ " "Use Real-World Metaphors" 2 1
.)T 3 3 "1.3.1\ \ " "Allow Direct Manipulation" 2 1
.)T 3 3 "1.3.2\ \ " "Provide Rapid Response" 3 1
.)T 3 3 "1.3.3\ \ " "Provide Output as Input" 3 1
.)T 2 2 "1.4\ \ " "Keep Interfaces Natural" 3 1
.)T 3 3 "1.4.1\ \ " "Make Navigation Easy" 3 1
.)T 3 3 "1.4.2\ \ " "Provide Natural Shades and Colors" 4 1
.)T 2 2 "1.5\ \ " "Keep Interfaces Consistent" 4 1
.)T 2 2 "1.6\ \ " "Communicate Application Actions to the User" 5 1
.)T 3 3 "1.6.1\ \ " "Give the User Feedback" 5 1
.)T 3 3 "1.6.2\ \ " "Anticipate Errors" 5 1
.)T 3 3 "1.6.3\ \ " "Use Explicit Destruction" 5 1
.)T 2 2 "1.7\ \ " "Avoid Common Design Pitfalls" 6 1
.)T 1 1 "2.\ \ " "Input and Navigation Models" 1 2
.)T 2 2 "2.1\ \ " "The Keyboard Focus Model" 1 2
.)T 3 3 "2.1.1\ \ " "Implicit Focus" 1 2
.)T 3 3 "2.1.2\ \ " "Explicit Focus" 2 2
.)T 2 2 "2.2\ \ " "The Input Device Model" 3 2
.)T 3 3 "2.2.1\ \ " "Pointing Devices" 3 2
.)T 3 3 "2.2.2\ \ " "Pointer Shapes" 5 2
.)T 3 3 "2.2.3\ \ " "Warp Pointer Only If Explicitly Enabled" 7 2
.)T 2 2 "2.3\ \ " "The Navigation Model" 7 2
.)T 3 3 "2.3.1\ \ " "Mouse-Based Navigation" 7 2
.)T 3 3 "2.3.2\ \ " "Keyboard-Based Navigation" 8 2
.)T 3 3 "2.3.3\ \ " "Menu Traversal" 14 2
.)T 3 3 "2.3.4\ \ " "Scrollable Component Navigation" 16 2
.)T 1 1 "3.\ \ " "Selection and Component Activation" 1 3
.)T 2 2 "3.1\ \ " "Selection Models" 1 3
.)T 3 3 "3.1.1\ \ " "Mouse-Based Single Selection" 2 3
.)T 3 3 "3.1.2\ \ " "Mouse-Based Browse Selection" 3 3
.)T 3 3 "3.1.3\ \ " "Mouse-Based Multiple Selection" 3 3
.)T 3 3 "3.1.4\ \ " "Mouse-Based Range Selection" 3 3
.)T 3 3 "3.1.5\ \ " "Mouse-Based Discontiguous Selection" 4 3
.)T 3 3 "3.1.6\ \ " "Keyboard Selection" 5 3
.)T 3 3 "3.1.7\ \ " "Canceling a Selection" 6 3
.)T 3 3 "3.1.8\ \ " "Selecting and Deselecting All Elements" 6 3
.)T 3 3 "3.1.9\ \ " "Using Mnemonics for Elements" 7 3
.)T 2 2 "3.2\ \ " "Selection Actions" 7 3
.)T 3 3 "3.2.1\ \ " "The Drag-and-Drop Model" 7 3
.)T 3 3 "3.2.2\ \ " "Using Primary Selection" 8 3
.)T 3 3 "3.2.3\ \ " "Using Quick Transfer" 9 3
.)T 3 3 "3.2.4\ \ " "Using Keyboard Clipboard Selection Actions and Deletion" 11 3
.)T 2 2 "3.3\ \ " "Component Activation" 11 3
.)T 3 3 "3.3.1\ \ " "Basic Activation" 12 3
.)T 3 3 "3.3.2\ \ " "Accelerators" 12 3
.)T 3 3 "3.3.3\ \ " "Mnemonics" 13 3
.)T 3 3 "3.3.4\ \ " "Help Activation" 13 3
.)T 3 3 "3.3.5\ \ " "Default Activation" 13 3
.)T 3 3 "3.3.6\ \ " "Expert Activation" 14 3
.)T 3 3 "3.3.7\ \ " "Previewing and Autorepeat" 14 3
.)T 1 1 "4.\ \ " "Application Design Principles" 1 4
.)T 2 2 "4.1\ \ " "Choosing Components" 1 4
.)T 3 3 "4.1.1\ \ " "Guidelines for Choosing a Main Component Group" 3 4
.)T 3 3 "4.1.2\ \ " "Guidelines for Choosing Interactive Methods" 4 4
.)T 2 2 "4.2\ \ " "Layout" 6 4
.)T 3 3 "4.2.1\ \ " "Common Client Areas" 6 4
.)T 3 3 "4.2.2\ \ " "Grouping Components" 18 4
.)T 3 3 "4.2.3\ \ " "Menu Design" 19 4
.)T 3 3 "4.2.4\ \ " "DialogBox Design" 21 4
.)T 2 2 "4.3\ \ " "Interaction" 24 4
.)T 3 3 "4.3.1\ \ " "Supplying Indications of Actions" 24 4
.)T 3 3 "4.3.2\ \ " "Providing Feedback" 27 4
.)T 3 3 "4.3.3\ \ " "Allowing User Flexibility" 27 4
.)T 2 2 "4.4\ \ " "Component Design" 28 4
.)T 1 1 "5.\ \ " "Window Manager Design Principles" 1 5
.)T 2 2 "5.1\ \ " "Configurability" 1 5
.)T 2 2 "5.2\ \ " "Window Support" 1 5
.)T 3 3 "5.2.1\ \ " "Primary Window" 2 5
.)T 3 3 "5.2.2\ \ " "Secondary Windows (Dialog)" 3 5
.)T 3 3 "5.2.3\ \ " "Menu Windows" 3 5
.)T 2 2 "5.3\ \ " "Window Decorations" 3 5
.)T 3 3 "5.3.1\ \ " "Client Area" 4 5
.)T 3 3 "5.3.2\ \ " "Title Area" 5 5
.)T 3 3 "5.3.3\ \ " "Maximize Button" 5 5
.)T 3 3 "5.3.4\ \ " "Minimize Button" 5 5
.)T 3 3 "5.3.5\ \ " "Other Buttons" 5 5
.)T 3 3 "5.3.6\ \ " "Resize Borders" 5 5
.)T 3 3 "5.3.7\ \ " "Window Menu" 6 5
.)T 2 2 "5.4\ \ " "Window Navigation" 7 5
.)T 2 2 "5.5\ \ " "Icons" 8 5
.)T 3 3 "5.5.1\ \ " "Icon Decoration" 8 5
.)T 3 3 "5.5.2\ \ " "Icon Menu" 8 5
.)T 3 3 "5.5.3\ \ " "Icon Box" 9 5
.)T 1 1 "6.\ \ " "Designing for International Markets" 1 6
.)T 2 2 "6.1\ \ " "Collating Sequences" 1 6
.)T 2 2 "6.2\ \ " "Country-Specific Data Formats" 2 6
.)T 3 3 "6.2.1\ \ " "Thousands Separators" 2 6
.)T 3 3 "6.2.2\ \ " "Decimal Separators" 2 6
.)T 3 3 "6.2.3\ \ " "Grouping Separators" 2 6
.)T 3 3 "6.2.4\ \ " "Positive and Negative Values" 2 6
.)T 3 3 "6.2.5\ \ " "Currency" 3 6
.)T 3 3 "6.2.6\ \ " "Date Formats" 3 6
.)T 3 3 "6.2.7\ \ " "Time Formats" 3 6
.)T 3 3 "6.2.8\ \ " "Telephone Numbers" 3 6
.)T 3 3 "6.2.9\ \ " "Proper Names and Addresses" 3 6
.)T 2 2 "6.3\ \ " "Icons, Symbols, and Pointer Shapes" 4 6
.)T 2 2 "6.4\ \ " "Scanning Direction" 4 6
.)T 2 2 "6.5\ \ " "Designing Modularized Software" 4 6
.)T 2 2 "6.6\ \ " "Translating Screen Text" 5 6
.)T 1 1 "7.\ \ " "Controls, Groups, and Models Reference Pages" 1 7
.)T 2 0 "" "Accelerators" 3 7
.)T 2 0 "" "Activation" 4 7
.)T 2 0 "" "Basic Controls" 6 7
.)T 2 0 "" "Basic Groups" 7 7
.)T 2 0 "" "Canvas" 8 7
.)T 2 0 "" "CascadeButton" 9 7
.)T 2 0 "" "CheckButton" 10 7
.)T 2 0 "" "CommandBox" 12 7
.)T 2 0 "" "CommandDialog" 13 7
.)T 2 0 "" "Composition" 14 7
.)T 2 0 "" "Default Activation" 15 7
.)T 2 0 "" "DialogBox" 16 7
.)T 2 0 "" "Drag and Drop" 18 7
.)T 2 0 "" "Edit Menu" 19 7
.)T 2 0 "" "ErrorDialog" 21 7
.)T 2 0 "" "Field Controls" 22 7
.)T 2 0 "" "File Menu" 23 7
.)T 2 0 "" "FileSelectionBox" 25 7
.)T 2 0 "" "FileSelectionDialog" 27 7
.)T 2 0 "" "Focus" 28 7
.)T 2 0 "" "Frame" 29 7
.)T 2 0 "" "Framing Groups" 30 7
.)T 2 0 "" "Help Menu" 31 7
.)T 2 0 "" "Icon Menu" 32 7
.)T 2 0 "" "IconBox" 33 7
.)T 2 0 "" "Icons" 34 7
.)T 2 0 "" "InformationDialog" 35 7
.)T 2 0 "" "Input Devices" 36 7
.)T 2 0 "" "Label" 37 7
.)T 2 0 "" "Layout Groups" 38 7
.)T 2 0 "" "List" 39 7
.)T 2 0 "" "MainWindow" 41 7
.)T 2 0 "" "MenuBar" 42 7
.)T 2 0 "" "Menus" 44 7
.)T 2 0 "" "MessageDialogs" 45 7
.)T 2 0 "" "Mnemonics" 46 7
.)T 2 0 "" "Navigation" 47 7
.)T 2 0 "" "OptionButton" 49 7
.)T 2 0 "" "PanedWindow" 51 7
.)T 2 0 "" "Panel" 52 7
.)T 2 0 "" "Pointer Shapes" 53 7
.)T 2 0 "" "Primary Selection" 55 7
.)T 2 0 "" "PromptDialog" 56 7
.)T 2 0 "" "PushButton" 57 7
.)T 2 0 "" "QuestionDialog" 58 7
.)T 2 0 "" "Quick Transfer" 59 7
.)T 2 0 "" "RadioButton" 60 7
.)T 2 0 "" "Sash" 62 7
.)T 2 0 "" "Scale" 64 7
.)T 2 0 "" "ScrollBar" 66 7
.)T 2 0 "" "ScrolledWindow" 68 7
.)T 2 0 "" "Selection" 69 7
.)T 2 0 "" "SelectionBox" 70 7
.)T 2 0 "" "SelectionDialog" 71 7
.)T 2 0 "" "Separator" 72 7
.)T 2 0 "" "Text" 73 7
.)T 2 0 "" "ToggleButton" 77 7
.)T 2 0 "" "WarningDialog" 78 7
.)T 2 0 "" "Window Menu" 79 7
.)T 2 0 "" "WorkingDialog" 81 7
.)T 1 1 "A.\ \ " "Common Bindings for Virtual Buttons and Keys" 1 A
.)T 1 1 "B.\ \ " "OSF/Motif Widgets and Components Correspondence" 1 B
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
