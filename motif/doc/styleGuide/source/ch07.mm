.\"  (c) Copyright 1989, 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.nr H1 6
.H 1 "Controls, Groups, and Models Reference Pages"
.iX "controls"
.iX "group reference"
.iX "models"
.iX "reference pages"
This chapter presents detailed information about
components, user interface models, and concepts in
reference format.  Each topic starts on a new
page and is organized alphabetically.  Details
on user interface models and concepts are provided
in the earlier chapters of this
.IR "Style Guide" .
The model and concepts reference pages are provided here only
as a quick reference to
information provided earlier, and are not complete
in every detail.  Therefore, they should not be used as the definitive
source for
information about user interface models and concepts.
This chapter includes the following model and concepts reference pages.
.TS
tab(@);
l l l.
Accelerators@File Menu@Layout Groups
Activation@Focus@Mnemonics
Basic Controls@Framing Groups@Navigation
Basic Groups@Help Menu@Pointer Shapes
DefaultActivation@Icon Menu@Primary Selection
Drag and Drop@IconBox@Quick Transfer
Edit Menu@Icons@Selection
Field Controls@Input Devices@Window Menu
.TE
.P
The reference pages for components are provided for
designers to use when implementing the components described
in this guide or when creating new components.
Remember when
designing new components, you should follow the
same rules that application designers follow.
As such, you should be familiar with
all the chapters of this
.IR "Style Guide" ,
not just this reference-page section.
.mS
New components must follow the guidelines for designing
applications.
.mE
This chapter contains the following control and group reference pages.
.TS
tab(@);
l l l.
Canvas@Label@RadioButton
CascadeButton@List@Sash
CheckButton@MainWindow@Scale
CommandBox@MenuBar@ScrollBar
CommandDialog@Menus@ScrolledWindow
Composition@MessageDialog@SelectionBox
DialogBox@OptionButton@SelectionDialog
ErrorDialog@PanedWindow@Separator
FileSelectionBox@Panel@Text
FileSelectionDialog@PromptDialog@ToggleButton
Frame@PushButton@WarningDialog
InformationDialog@QuestionDialog@WorkingDialogBox
.TE
.P
.iX "components" "design"
.iX "design" "components"
When designing a new component, compare
its features with those of other components.
.mS
If the
new component has a feature that is the same as another
component, the mechanisms for using the feature,
layout, key bindings, graphics, and so on,
must be similar to the existing component.
.mE
.br
.ne 23
.P
Each component reference page contains the following
information about the component.
.VL 18n
.LI "\fBDescription\fP"
A description of the use and appearance of each component,
group, or model.
.LI "\fBIllustration\fP"
An illustration of a typical component.  The illustrations in this
.I "Style Guide"
use the OSF/Motif reference appearance
with 3-dimensional beveled edges.
Although it is important to be consistent in the placement
of the elements in a component, the appearance, or rendering,
of the component is not an issue of
.I "Style Guide"
compliance.
.LI "\fBNavigation\fP"
A description of the methods for navigating within a component.
.LI "\fBOther Operations\fP"
A description of other operations available within the component.
.LI "\fBCommon Bindings\fP"
A list of the virtual keys used by this component, and the
common substitutions for each virtual key.
.LI "\fBRelated Information\fP"
A list of sections
and related reference pages
in this
.IR "Style Guide"
with additional information.
.LE
.nr 'C \\n(Cl
.nr Cl 2
.de hR
.EH "'\\$1''\\\\*(cT'"
.OH "'\\\\*(cT''\\$1'"
.fl
.nr Hu 2
.bp
.HU "\\$1"
..
.de hS
.nr Hu 3
.fl
.HU "\\$1"
..
.so AcceleratMR.mm
.so ActivatioMR.mm
.so BasicCR.mm
.so BasicGR.mm
.so CanvasR.mm
.so CascadeButR.mm
.so CheckButtoR.mm
.so CommandBoxR.mm
.so CommandDBR.mm
.so CompositioR.mm
.so DefaultAcMR.mm
.so DialogBoxR.mm
.so DragAndDrMR.mm
.so EditMenuMR.mm
.so ErrorDBR.mm
.so FieldCR.mm
.so FileMenuMR.mm
.so FileSelectR.mm
.so FileSeleDBR.mm
.so FocusMR.mm
.so FrameR.mm
.so FramingGR.mm
.so HelpMenuMR.mm
.so IconMenuMR.mm
.so IconBoxMR.mm
.so IconsMR.mm
.so InformatDBR.mm
.so InputDeviMR.mm
.so LabelR.mm
.so LayoutGR.mm
.so ListR.mm
.so MainWindowR.mm
.so MenuBarR.mm
.so MenuR.mm
.so MessageDBR.mm
.so MnemonicsMR.mm
.so NavigatioMR.mm
.so OptionButtR.mm
.so PanedWindoR.mm
.so PanelR.mm
.so PointerShMR.mm
.so PrimarySeMR.mm
.so PromptDBR.mm
.so PushButtonR.mm
.so QuestionDBR.mm
.so QuickTranMR.mm
.so RadioButtoR.mm
.so SashR.mm
.so ScaleR.mm
.so ScrollBarR.mm
.so ScrolledWiR.mm
.so SelectionMR.mm
.so SelectionR.mm
.so SelectioDBR.mm
.so SeparatorR.mm
.so TextR.mm
.so ToggleButtR.mm
.so WarningDBR.mm
.so WindowMenMR.mm
.so WorkingDBR.mm
.nr Cl \\n('C
