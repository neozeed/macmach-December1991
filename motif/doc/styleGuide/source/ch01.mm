.\"  (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
.\"  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS.
.\"  (c) Copyright 1987, 1988, 1989 HEWLETT-PACKARD COMPANY
.\"  (c) Copyright 1988 MASSACHUSETTS INSTITUTE OF TECHNOLOGY 
.\"  (c) Copyright 1988 MICROSOFT CORPORATION
.\"  ALL RIGHTS RESERVED
.nr H1 0
.H 1 "User Interface Design Principles"
.iX "Design"
A user interface is simply the interface between
an application and the user of an application.
The primary goal of a user interface is to help user interface
designers easily create applications that increase
user effectiveness and satisfaction
following the guidelines presented in this
.IR "Style Guide" ,
you can create applications that are well designed and
easy to use.  These guidelines pertain to all sorts of
applications, from spreadsheets and word processors
to CAD tools.
.P
To be effective, a user interface allows a user
to interact with an application simply and naturally.
Successful user interface designers keep the user
in mind while designing an application.  Keep the user
in mind by following these two principles:
.BL
.LI
Know the user.
.LI
Empower the user.
.LE
.P
The user of an application, above all, wants to get the job done.
A user interface needs to be designed so the user can quickly
and easily complete the tasks.  Users want mastery over an
application.  Make it simple for the user to master
the basics of your application.  At the same time,
you can include advanced methods
of interaction with the application,
shortcuts for the user.
Users are curious and exploratory.
They will find these
shortcuts as they use and master the application.  Such shortcuts
need not be as intuitive as the regular methods
of interaction.
.P
This chapter discusses the following guidelines for creating
user interfaces that are consistent and easy to use.
Because of the particular nature of your client application, component, or
window manager, or in response to customers' needs,
you may not be able to apply all of these principles
all of the time.
.BL
.LI
Adopt the user's perspective. 
.LI
Give the user control. 
.LI
Use real-world metaphors. 
.LI
Keep interfaces natural. 
.LI
Keep interfaces consistent. 
.LI
Communicate application actions to the user. 
.LI
Avoid common design pitfalls. 
.LE
.H 2 "Adopt the User's Perspective"
Effective design starts with adopting the user's point
of view, which is often difficult to do.  Application
designers
tend to see an application as the implementation of
functions.  In contrast, the user sees an application in
terms of its interface.
.P
Good design is rooted in an understanding of the user's
work.  A well-designed application solves users'
problems, makes their work easier, and offers them
new capabilities.  The two most effective ways to
understand the user's work are to involve users in the
design, and to be a user yourself.
.P
Input from users can help determine both appropriate
functions and methods for presenting them.  Involve
users as early as possible in the design process
because as the design progresses and the schedule
closes in, the possibilities for design change
decrease.
.P
.iX "prototypes"
You do not need a working prototype to involve users.
In fact, you can even involve users while you are
writing specifications.  At this stage, you can watch
users work in order to understand the environment in
which your application will be used.  Talk to these
users about their work, their current tools, and their
goals for new tools.
.P
For example, if you are designing software to create
and display charts and graphs during meetings, you
might attend meetings at various customer sites, see
how charts and graphs are currently used in meetings,
and interview meeting participants to learn what they
would like to see in a new tool.
Once you have a working prototype of your application,
invite users to test it to see if your interface
meets the goals you established for it.
.P
Try to use your application in real
situations.
Using an application can provide critical insights
into user interface problems.  Acquiring experience
with the application can be difficult and time
consuming, but is a worthwhile exercise.  Before you
even create the interface for your application, you
can use similar applications, even competitive
products, to help you understand the user's tasks.
.H 2 "Give the User Control"
Users want and need to be in control of the tools they
use to perform their work.  The user can be in control
when an application is flexible, and uses progressive disclosure.
.H 3 "Keep Interfaces Flexible"
.iX "flexibility"
Providing multiple ways for users to access application functions and
accomplish their tasks increases their sense of control.
Flexibility enables users to select the best method of 
accessing a function based on the criteria they choose:
experience level, personal preference, unique situation, or simply habit.
For example, a user can access a function through a PullDown Menu,
direct manipulation of an object, a mnemonic
key press, or a keyboard accelerator.
.P
Your application should also be configurable.
Allowing users to configure settings and select personal preferences
enhances their sense of control and encourages them to take an active role
in understanding your product and how it works.  To be effective, the
configurability of your application needs to be easily accessible. 
.H 3 "Use Progressive Disclosure"
.iX "ordering" "functions"
.iX "progressive disclosure"
Design your application so that the necessary and
common functions are presented first and in a logical
order.  Make the more sophisticated and less
frequently used functions hidden from immediate view,
but still available.  For example, use a DialogBox
to hide settings that are not accessed often.
.P
Decisions about the placement of functions are not
easy to make.  From the implementation standpoint, all
functions are important.  Often, however, a relatively
small number of functions account for the majority
of use.  Make sure that these important functions
are prominently featured in the presentation of the
interface.  Also remember that they can be prominent
only if other functions are hidden.
.H 2 "Use Real-World Metaphors"
.iX "metaphors" "real world"
.iX "real world metaphors"
A good user interface allows the user to transfer skills from real-world
experiences.  For example, PushButtons push, and Scales slide.  This makes
it easier for the user to infer how to use an application.
When you design a new component, consider how a similar real-world
control performs to incorporate the metaphor into the new component.
Real-world metaphors can extend to groups of components as well, especially
when making a computer-based user interface to replace a mechanical
user interface.
.H 3 "Allow Direct Manipulation"
.iX "direct manipulation"
.iX "manipulation" 
Users need to be able to directly manipulate elements of the
user interface and their applications.
For example, the user needs to be able to directly scroll Text
with a ScrollBar, rather than using a keyboard-driven command.
Direct manipulation simulates the real world where the user employs tools to
perform tasks on physical objects.  Users control applications
by directly manipulating graphical
components similar to real-world controls, rather than
entering a command on a command line.
Direct manipulation reduces the amount of information the user
needs to memorize.
.P
Direct manipulation connects an action to an observable
response from a component.
Using direct manipulation, the user gets an
immediate visible result from each action.
.P
.iX "direct manipulation"
.iX "manipulation" "direct"
The direct manipulation model is an object-action model.
That is, you first select an object or group of objects,
then you perform an action on the selected objects.
An object-action model allows the user to see what elements
will be acted on before performing an action.  It also
allows multiple actions to be performed successively on
the selected elements.
.P
Although it is important to allow direct manipulation of
the objects in your application,
.mS
you must also support methods
for interacting with your application by keyboard-only users.
.mE
These methods can also be used by advanced users to
perform some tasks more quickly.
.H 3 "Provide Rapid Response"
.iX "immediacy of response"
.iX "response immediacy"
Make your application respond to input as rapidly
as possible.  The immediacy of the visual response is
crucial to the experience of direct manipulation.
When using components, provide the
application's response immediately and in proportion
to the component's actions.
.mS
The application must also have a
consistent speed of response.
.mE
Delays, disproportionate
responses, or inconsistent responses can render
an otherwise well-designed application unusable.
Performance problems make it difficult for the user to
concentrate on the task at hand.
.H 3 "Provide Output as Input"
Another feature of direct manipulation is that the output of one part
of an application or the output of the application itself
is also available as input.  For example, 
if one action produces a list of file names, another action can
select them for use elsewhere.
.P
The user manipulates objects by locating them and
clicking on them rather than typing in their names.
Design so that
the only time the user needs to type a name is to
create an object. A well-designed application reduces the
amount of information the user needs to memorize to perform
tasks.
.H 2 "Keep Interfaces Natural"
You can extend the concepts of giving the user
control and using real-world metaphors to arrange your
application so that tasks flow naturally.  Users need to
be able to anticipate the natural progression of each
task; through this anticipation, they are able
to complete tasks more quickly.
.P
Each screen object needs to have a distinct appearance
that the user can easily recognize and quickly
understand.  At the same time, the style of the
interface needs to graphically unify these elements
and ensure a consistent and attractive appearance at
any screen resolution.
.H 3 "Make Navigation Easy"
.iX "navigation"
Make navigation easy by providing a straightforward
presentation of the overall work area and the
mechanisms for moving through it.  Moving easily and
quickly within the work area gives the user a sense
of mastery over the application and their work.  For
example, ScrollBars are an effective way to indicate
the position of the current view in relation to
an area as a whole.  In addition to providing
positional feedback, ScrollBars allow the user to
move through the area.
.P
Arrange elements on the screen according to their use;
an optimal arrangement assists the user's decision-making
processes and reduces the possibility of errors.  The
best approach for arranging screen elements according
to use is to involve users in the arrangement process.
Present screen
objects in an orderly, simple, and uncluttered manner.
.P
Reduce mouse movement to simplify the actions of the user.
For example; place secondary DialogBoxes
near their parent DialogBox so that when the
secondary DialogBox appears, the mouse pointer is over the
default PushButton, unless the user needs to see the
contents of the original DialogBox.  Reducing mouse movement helps
make an interface natural because, from the user's
point of view, work involves a stream of thoughts,
intentions, and tasks (some predefined and some
that become apparent during the process)
that all relate to some desired outcome
or accomplishment.  This stream of thought is disrupted
when the user has to make unnecessary mouse movements,
open and close DialogBoxes, or search for commands.
.H 3 "Provide Natural Shades and Colors"
.iX "contrast" "screen objects"
.iX "colors"
Minimize the contrast between screen objects in order
to direct the user's attention.
Appropriate use of contrast helps the user distinguish
screen objects against the background of a window.
Very dark screen objects on a light background, very bright
objects on a dark background, and bright colors all
command the user's attention.  If there are many objects
with strong contrast or bright colors on the screen,
the user will have difficulty knowing where to look
first because all these objects compete equally for
attention.
.P
Use color as a redundant aspect of the interface;
that is, use it to provide additional differentiation
among screen objects.  Differentiation also comes
from the shape and size of the screen objects.  For
example, in many parts of the world, stop signs are
red octagons. You recognize the stop sign by both its
shape and color.
.H 2 "Keep Interfaces Consistent"
.iX "consistency"
The main purpose of this
.I "Style Guide"
is to ensure consistency.  
Consistency is important both among applications and within a
single application.  Consistency helps the user transfer familiar
skills to new situations.
The user can apply
the knowledge learned from one application to another
application, reducing the amount of learning and
subsequent recall.  Consistency within applications
facilitates exploration of new functions.  When
components work in a manner that is consistent with
other components, the user will be less afraid to try
new functions.  The new functions will seem familiar,
comfortable, and appropriate.
The guidelines in this
.I "Style Guide"
allow you to create applications that are consistent in a
diverse market and that help your applications succeed in the marketplace.
.P
.iX "consistency"
.iX "intraaplication consistency"
Intraapplication consistency means the following:
.BL
.LI
Similar components operate similarly and have similar uses.
.P
For example, because PullDown,
Popup, and Option Menus are similar components, their operation 
and use should be similar.
Choosing the proper component is described in Section 4.1.
Component interaction is described in the
reference section for each component and in Section 4.3.
.LI
The same action should always have the same result.  
.P
For example, pushing the top arrow in a ScrollBar
should always move the ScrollBar up.
Interaction is described in Section 4.3.
.LI
The function of components should not change based on context.
.P
For example, clicking a button should always perform the same action.
Note that even though the action is the same, the result of
the action can depend on context.  A button in a file editor can
begin editing one of a number of files.  The button need not always edit
the same file, rather its consistent action is to edit the selected
file.
Interaction is described in Section 4.3.
.LI
The position of components should not change based on context.
.P
Components should not generally be added and removed as needed.  This makes
it difficult to quickly find the desired component.  Instead,
you should make unneeded components nonfunctional and indicate
this by deemphisizing (graying out) their labels.
Component layout is described in Section 4.2.
.LI
The position of the mouse pointer should not warp.
.P
The location of the mouse pointer should be determined by 
direct manipulation and should not be positioned arbitrarily
by the application.
Positioning the mouse pointer by the application causes
the user to lose track of the pointer.  Warping the pointer
also causes problems with tablet style pointing devices 
that rely on absolute pointer positioning.
The input model is described in Chapter 2.
.LE
.P
Consistency among applications increases the user's sense of mastery.
Experience with one application can be readily applied to another
application, creating a positive transfer of knowledge.
The task at hand, rather than learning a new application,
becomes the focus of a computer session.
When applications work in a manner that is
consistent with other applications, users enjoys a feeling of immediate
confidence in their ability to master the new program.  Also, they are
pleasantly surprised when trying new functions because, although new, the
functions seem familiar.
.P
"consistency" "intraaplications"
"intraaplication" "consistency"
Interapplication consistency means the following:
.BL
.LI
Components look familiar.
.P
This does not mean that components look exactly the same,
but that the internal layout of components should be the same.
Elements of appearance such as color, size, and thickness
of beveled edges are less important to application
interoperability.  Component design and layout are described
in the reference section for each component and in Section 4.4.
.LI
Interaction is familiar.
.P
When interaction is different among applications,
it confuses the user and makes it difficult
to concentrate on the task of the application.
This applies to the behavior of components, input
methods, selection models, and keyboard navigation.
Interaction is described in Section 4.3.
.LI
Components are organized in a familiar manner.
.P
The user needs to be able to quickly find the proper
component for each task.  Organizing the components
according to consistent guidelines helps the user
do this.  Application layout guidelines are described
in Section 4.2.
.LE
.H 2 "Communicate Application Actions to the User"
Effective applications let the user know what is
happening with the application, but without
revealing implementation details.  Proper communication
between the user and the application increases user
satisfaction.  There are three guidelines for
communicating from the application to the user:
provide feedback, anticipate errors, and provide
warnings.
.H 3 "Give the User Feedback"
.iX "feedback"
Feedback lets users know that the computer has
received their input.  Give users feedback
whenever they have selected a component or Menu item by
highlighting the component or Menu item in some way.  In
addition, if certain operations take more than
a few seconds, you should let the user know that the
computer is working on that operation by providing a
message or by changing the pointer to a working pointer.
.H 3 "Anticipate Errors"
.iX "acts requiring actions"
.iX "errors"
Anticipate the errors that are likely to occur.
By anticipating errors,
you can avoid them in your design, enable the support of recovery attempts,
and provide messages informing the user of the proper corrective action.
For example, one technique for avoiding excessive error
messages is to dim interface components when they cannot
be used.
.P
.iX "help" "context-sensitive"
.iX "context-sensitive help"
Context-sensitive help aids understanding, reduces errors, and eases
recovery efforts.  Help information text needs to be clear, concise, and
written in everyday language.  Help information needs to be readily
accessible and just as readily removable. 
.P
.iX "undo"
Many users are most comfortable with learning how to use
software applications when they use a natural,
trial-and-error method.  An undo function supports 
learning by trial and error by minimizing the cost of errors.
An undo function allows the user to
retract previous actions, and fosters a spirit of exploration and
experimentation that is essential.
.H 3 "Use Explicit Destruction"
.iX "explicit destruction"
Explicit destruction means that, when an action has irreversible negative
consequences, it should require the user to take an explicit action to
perform it.  
For example, while a worksheet can be saved simply
by clicking on a Save PushButton,
erasing the worksheet should require clicking
on an Erase PushButton and answering
a warning question like
"\f(CWAre you sure you want to erase this worksheet?\fP"
with a button click in the warning DialogBox.
.P
.iX "warnings"
Warnings protect the user from inadvertent destructive operations, yet allow
them to remain in control of the application.  Warnings
also encourage the user to experiment without fear of loss.
Operations that can cause a
serious or unrecoverable loss of data should warn the user of the
consequences and request explicit confirmation.
.H 2 "Avoid Common Design Pitfalls"
The process of achieving good design presents many
challenges and potential pitfalls.  The following
guidelines can help you avoid common pitfalls.
.BL
.LI
Pay attention to details.
.P
The details of an application express the sense
of craft that you applied to the application.  The
details of an elegantly designed interface
both pleases users and facilitates their work.  For
example, aligning the PushButtons of two related
and overlapping DialogBoxes makes it easier for
the user to activate new settings in an apparently
seamless operation.  Consistent capitalization of
Menu items and DialogBox labels is a design detail
that reduces textual distractions for the user.
.LI
Do not finish prematurely.
.P
A common design pitfall is assuming too early that
a design is complete.  This tendency is aggravated
by schedule pressures and difficulty in pinpointing
the inadequacies of a design.  While it is important
to begin designing early, it is also important to
allow for redesigning for as long as possible.
The first design of an application is not a
solution but a fresh perspective from which to view
interface design problems.
.LI
Design iteratively.
.P
Interface design is best done iteratively.
The development cycle of implementation,
feedback, evaluation, and change avoids errors
by allowing for early recognition and correction of
unproductive designs.
.LI
Start with a fresh perspective.
.P
Avoid the temptation to convert existing software
by simply translating it to a new style of
interface.  Because direct manipulation changes the
way the user works, a simple one-to-one translation
is unlikely to be successful.  Command line
applications that are converted to direct
manipulation need to be extensively reconsidered and
revised.  The structure of the function hierarchy
and presentation needs to be completely redesigned.
.LI
Hide implementation details.
.P
User interfaces need to hide the underlying
software, and present a consistent interface to
the user.  A good user interface does not allow
implementation details of the application to show
through; it frees the user from focusing on the
mechanics of an application.
.LE
