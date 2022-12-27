...\"	tocmac1.mm  --  header stuff so that we can generate a toc
...\"	from local .)T calls.   This def gets closed by the tocmac2.mm
...\"	file, which also calls the .TC macro.
...\"
.de >C	\" macro that holds .)T calls
.)T 1 1 "1.\ \ " "Introduction" 3
.)T 1 1 "2.\ \ " "Hierarchy" 4
.)T 1 1 "3.\ \ " "3D Visuals" 7
.)T 1 1 "4.\ \ " "Resolution Independence" 8
.)T 1 1 "5.\ \ " "Input Model" 9
.)T 1 1 "6.\ \ " "Scrolling Model" 10
.)T 1 1 "7.\ \ " "Compound String Functionality" 11
.)T 1 1 "8.\ \ " "Gadget Functionality" 12
.)T 1 1 "9.\ \ " "Help" 13
.)T 1 1 "10.\ \ " "Shells" 14
.)T 1 1 "11.\ \ " "Name Changes" 15
.)T 1 1 "12.\ \ " "Widget Hierarchy Differences by Class" 16
.)T 1 0 "" "Appendix A - Name Change Exceptions" 21
.)T 2 0 "" "Widget hierarchy differences by class" 21
.)T 2 0 "" "Function name changes" 22
.)T 2 0 "" "Resource name changes" 23
.)T 2 0 "" "Enumeration literal name changes" 23
.)T 2 0 "" "Callback reason name changes" 24
.)T 2 0 "" "Compound string name changes" 24
.)T 2 0 "" "FontList name changes" 25
.)T 2 0 "" "Clipboard name changes" 25
.)T 2 0 "" "Resource Manager name changes" 25
...\"	tocmac2.mm  --  generate the table of contents.  This file is
...\"	the complement to tocmac1.mm.
...\"
...\"	close the definition started in tocmac1.mm:
..
...\"
...\"	call the table of contents
.TC 1 1 3 0
.bp
