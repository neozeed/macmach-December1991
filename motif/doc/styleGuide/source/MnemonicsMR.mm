.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  ALL RIGHTS RESERVED
.hR "Mnemonics"
.hS "Description"
.iX "mnemonics"
A mnemonic is a single character that can be associated with
any component that contains a text label.
.mS
The label must contain
the character, and the character
must be underlined within the label.
.mE
The mnemonic should be the first character of the label.
.mS
If a label does not naturally
contain the character of the mnemonic, the mnemonic must be placed
in parentheses following the label.
.mE
Labels can also be sequentially numbered, and the number can serve
as the mnemonic.  Labels that are duplicated
within an application should be given the same mnemonic.
.mS
Mnemonics must be case insensitive for activation.
.mE
Either an uppercase or lowercase letter can be underlined in the
label.
.P
.mS
When the location cursor is on a component within the same field
as a component with a mnemonic, a Menu, or a MenuBar,
typing the
mnemonic character must move the location cursor to the component
and activate it.
If a mnemonic is used for 
an OptionButton or for a CascadeButton in a MenuBar, 
or optionally for a PushButton that is not in a basic group, typing
.B MAlt
and the mnemonic anywhere in the window or its Menus must
move the cursor to the component with that mnemonic
and must activate it.
.mE
.P
Collections can also support mnemonics associated
with its elements.
.mS
Typing a mnemonic while
the collection has the keyboard focus must
be equivalent to moving the location cursor to the
element and typing
.BR KSelect .
.mE
