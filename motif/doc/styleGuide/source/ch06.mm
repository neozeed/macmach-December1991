.\"  (C) COPYRIGHT 1989, 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  (C) COPYRIGHT 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (C) COPYRIGHT 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (C) COPYRIGHT 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (C) COPYRIGHT 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.NR H1 5
.H 1 "Designing for International Markets"
.iX "base"
.iX "internationalization"
.iX "localization"
This chapter provides basic guidelines for producing applications
for international markets.
Internationalization is the 
process of generalizing programs or systems so that they can handle
a variety of languages, character sets, and national customs.
Localization is the process of providing language- or
country-specific information or support for programs.
.P
.iX "ANSI C"
.iX "ANS X3.159-1989"
.iX "POSIX 1003.1"
In general, internationalization issues are handled by tools
available to programmers on their system.  For example,
ANSI C standard (ANS X3.159-1989) and POSIX 1003.1 have
defined internationalization in terms
of locale.  The locale can then be set as part of the user's
environment, allowing the program to access locale-specific
information, such as data formats, collating sequences, and
system messages, from system- or application-specific databases.
You should use any internationalization tools available on
your system to support internationalization in your application.
.P
Following are some of the issues that are
handled by an internationalized application.  Note that
your application generally does not need to directly deal with these
issues.  In most cases, they should be addressed by the internationalization
tools available on your system.  They are provided here primarily
to increase your awareness of issues that can affect your
programming.  In a few cases you may need to adjust your program
to allow for size and layout changes of data in different
locales.
.BL
.LI
Collating sequences
.LI
Country-specific data formats
.LI
Icons, symbols and pointer shapes
.LI
Scanning direction
.LI
Modularized software
.LI
Translation of screen text
.LE
.H 2 "Collating Sequences"
.iX "internationalization" "collating sequences"
.iX "collating sequences"
.iX "sequences" "collating"
.iX "alphanumeric lists"
To produce an alphanumeric list, printable characters are sorted according to
a collating sequence.  Printable characters include letters possibly with
accents, numbers,
punctuation characters, and other symbols such as * (asterisk) or &
(ampersand).  The collating sequence defines the value and position of
a character relative to the other characters.
.P
Many applications make frequent use of collating sequences to produce
alphanumeric lists.  Examples of alphanumeric lists include the following:
.BL
.LI
A directory listing of filenames
.LI
The output from a sorting utility
.LI
An index produced by a text-processing application
.LI
The lists produced by a database application, such as lists of names or
addresses
.LE
.br
.ne 10
.H 2 "Country-Specific Data Formats"
.iX "country-specific data formats"
.iX "internationalization" "data formats"
.iX "data formats" "country-specific"
.iX "formats" "data"
.iX "internationalization" "data formats"
Country-specific data formats include the following:
.BL
.LI
Thousands separators
.LI
Decimal separators (or, radix characters)
.LI
Grouping separators
.LI
Positive and negative values
.LI
Currency
.LI
Date formats
.LI
Time formats
.LI
Time zones
.LI
Telephone numbers
.LI
Proper names and addresses
.LE
.H 3 "Thousands Separators"
.iX "thousands separators"
.iX "separators" "thousands"
.P
The comma, period, space, and apostrophe
are examples of valid separators for
units of thousands as shown in the following examples:
.DS
1234567
1 234 567
1.234.567
1'234'567
1,234,567
.DE
.H 3 "Decimal Separators"
.iX "decimal separators"
.iX "separators" "decimal"
.P
The period, comma, and the center dot are examples of
valid separators for decimal fractions as shown in
the following examples:
.DS
5
5,324
5.324
5 324
5\v'-0.25v'.\v'0.25v'324
.DE
.H 3 "Grouping Separators"
.iX "grouping" "separators"
.iX "separators" "grouping"
.P
Grouping may not be restricted to thousands separators as
shown in the following examples:
.DS
400,001.00
40,0001,00
.DE
.H 3 "Positive and Negative Values"
.iX "positive and negative values"
.iX "values" "poitive and negative"
.iX "internationalization" 
.iX "numbers" "postive and negative"
.P
Various countries indicate positive and negative values differently. 
The symbols + and - may appear either before or after 
the number.
Negative numbers may be enclosed in parentheses in 
applications such as a spreadsheet.
.br
.ne 20
.H 3 "Currency"
.P
.iX "data formats" "currency"
.iX "data formats" "country-specific"
.iX "currency"
Currency formats differ among various countries.
The comma, period, and colon are examples of valid separators for currency. 
There may be one or no
space between the currency symbol and the amount.  
The currency symbol may be up to four 
characters.  The following example shows valid currency values:
.DS
Sch3.50
SFr. 5.-
3.50FIM
25 c
3F50
760 Ptas
Esc. 3.50
kr. 3,50
.DE
.H 3 "Date Formats"
.iX "date formats"
.iX "internationalization" "date formats"
.iX "alphanumeric date formats"
.iX "numeric date formats"
.iX "formats" "dates"
.P
Most countries use the Gregorian calendar, but some do not.
Dates can be formatted differently based on the locale.
Separators can be different in different locales or left out altogether.
The hyphen, comma, 
period, space, and slash are all examples of valid separators for
the day, month, and year.
In numeric date formats the month and day fields can be 
reversed, and in some cases, the year field can come first.
For example, the 4th of August 1990 can be written
as either 4/8/90 or 8/4/90 depending on locale.
In addition, users in Sweden sometimes place the 
year first, so June 11, 1990 could be 900611 or 901106.
.H 3 "Time Formats"
.iX "formats" "time"
.iX "time formats"
.P
Time formats can change based on locale.
The colon, period, and space are examples of valid separators for hours, 
minutes, and seconds. 
The letter \fBh\fP can separate hours and 
minutes.
There is both 12- or 24-hour notation.  
For 12-hour notation, a.m. or p.m. can appear after the time, 
separated by a space.
The following example shows a number of valid time formats:
.DS
1830
18:30
04 56
08h15
11.45 a.m.
11.45 p.m.
13:07:31.30
13:07:31
.DE
.H 3 "Telephone Numbers"
.P
.iX "formats" "telephone numbers"
.iX "telephone numbers"
.iX "internationalization" "telephone numbers"
.iX "internationalization" "numbers"
.iX "national numbers"
Telephone numbers can contain
blanks,
commas,
hyphens,
periods, and
square brackets
as valid separators, for example.
Telephone numbers can be displayed in local, national, and 
international formats.  Local formats vary widely.  National formats 
may have an area code in parentheses, while the international format 
may drop the parentheses, but add a plus sign at the  beginning of the 
number to indicate the country code.
The following examples show valid telephone number formats:
.DS
(038) 473589
+44 (038) 473549
617.555.2199
(617) 555-2199
1 (617) 555-5525
(1) 617 555 5525
911
1-800-ORDERME
.DE
.H 3 "Proper Names and Addresses"
.P
.iX "proper names"
.iX "addresses"
.iX "formats" "addresses"
Addresses may vary from two to six lines long,
and can include any character used in the locale's character
set.
The post code (zip code) can be in various positions in the 
address, and can include alphabetic characters and separators
as well as numbers.
.br
.ne 8
.H 2 "Icons, Symbols, and Pointer Shapes"
.P
.iX "icons"
.iX "pointer shapes"
.iX "design" "icons"
.iX "design" "pointer shapes"
.iX "design" "graphical symbols"
.iX "language independent"
.iX "internationalization"
It may not always be possible to design an icon, pointer shape, or other
graphical symbol that adequately represents the same object or function in
different countries.  Culture is inherent even in seemingly universal
symbols.  For example, sending and receiving mail is a commonly understood
function, but representing that function with an icon of a mail box may be
inappropriate because the appearance of mail boxes varies widely among
countries.  Therefore, an envelope may be a more appropriate icon.
You should make sure that graphical symbols are localizable.
.P
When used correctly, graphical symbols offer the following advantages:
.BL
.LI
They are language independent and do not need to be translated.
In some cases, you may not be able to avoid changing an icon or symbol 
for a culture that is vastly different.  However, design icons and 
symbols with the entire user population in mind so that you can try to 
avoid redesigning.
.LI
They can be used instead of computer terms that have no national-language
equivalent.
.LI
They may have more impact when used with text as warnings than the text
alone.
.LE
.P
Here are a few guidelines to follow when creating icons, symbols, or 
pointer shapes:
.BL
.LI
Use an already existing international icon, if possible.
.LI
Make your icons, symbols, or pointer shapes represent basic, 
concrete concepts.  The more abstract the icon, the more explanatory 
documentation is needed.
.LI
Check your icons and symbols for conflicts with existing icons or 
symbols for that function.
.LI
Do not incorporate text in icons because the text will need to 
be translated.  Translated text expands, and might no longer fit the 
icon.
.LI
Test and retest your symbols and icons in context with real users.
.LE
.H 2 "Scanning Direction"
.iX "scanning direction"
.iX "reading direction"
.P
Readers of Western languages scan from
left to right across the page (or display screen)
and from top to bottom.  In other languages, particularly Eastern ones 
such as Hebrew and Arabic, this is not the case; readers scan from right to left.  
The scanning direction of the country of localization may have an
impact on the location of components in dialog boxes, the order of selections
in menus, and other areas.
.P
If your application will be used in environments other than
those that scan from left to 
right, remember that the
the scanning direction should match the input direction.
.H 2 "Designing Modularized Software"
.P
Modularizing software allows for easier localization;
that is, a properly modularized application requires that
fewer files be modified to localize the application.
Guidelines for designing modularized software are as follows:
.BL
.LI
Create separate modules for text, code, and input/output components
that need to be changed to accommodate different markets.
.LI
Separate all user interface text from the code that presents it.
.LI
Use standard (registered) data formats, such as ISO and IEEE.
.LI
Use standard processing algorithms 
for all processing, storage, and interchange.
.LE
.P
In general, you should modularize your application so that
elements that need to be translated to different languages
are in separate files, and that those files are the only files
that will need changes for localization. 
Furthermore, you should have a different set of language dependent
text files for each locale that are read in at run time using
the internationalization tools available on your system.  In this
way you can support multiple locales simultaneously.
.br
.ne 8
.H 2 "Translating Screen Text"
.iX "translating text"
.iX "text" "translation"
.iX "internationalization"
.iX "translating text"
.iX "text" "translation"
.iX "affirmative statements"
.iX "active voice"
.iX "formats" "messages"
.iX "messages"
.P
Well-written screen text makes an application easier for users to
understand.  It also makes translation easier.
.P
Use the following guidelines to write screen text for translation:
.BL
.LI
Write brief and simple sentences; they are easy to understand and translate.
.LI
Write affirmative statements; they  are easier to understand than 
negative statements.  For example, use "Would you like to continue?"
rather than "Wouldn't you like to continue?"
.LI
Use active voice; it is easier for both application users and application
translators to understand.  For example, use "Press the Help button."
rather than "The Help button should be pressed."
.LI
Use prepositions to clarify the relationship of nouns; avoid 
stringing three or more nouns together.
.LI
Use simple vocabulary; avoid using jargon unless it is a part of 
your audience's working vocabulary. 
.LI
Allow space for text expansion. Text translated from English is 
likely to expand 30% to 50%, or even more.
.LE

















