CFLAGS=		/DEBUG/NOLIST

hfiles=		ebind.h-
		edef.h-
		efunc.h-
		epath.h-
		estruct.h-
		evar.h


ofiles=		ansi.obj-
		basic.obj-
		bind.obj-
		buffer.obj-
		crypt.obj-
		dg10.obj-
		display.obj-
		eval.obj-
		exec.obj-
		file.obj-
		fileio.obj-
		hp110.obj-
		hp150.obj-
		ibmpc.obj-
		input.obj-
		isearch.obj-
		line.obj-
		lock.obj-
		main.obj-
		mouse.obj-
		random.obj-
		region.obj-
		search.obj-
		st520.obj-
		vms.obj-
		tcap.obj-
		tipc.obj-
		vmsvt.obj-
		vt52.obj-
		window.obj-
		word.obj-
		z309.obj

me.exe : $(ofiles)
	LINK  me/option/share

ansi.obj : ansi.c,$(HFILES)
    CC $(CFLAGS) ansi
basic.obj : basic.c,$(HFILES)
    CC $(CFLAGS) basic
bind.obj : bind.c,$(HFILES)
    CC $(CFLAGS) bind
buffer.obj : buffer.c,$(HFILES)
    CC $(CFLAGS) buffer
crypt.obj : crypt.c,$(HFILES)
    CC $(CFLAGS) crypt
dg10.obj : dg10.c,$(HFILES)
    CC $(CFLAGS) dg10
display.obj : display.c,$(HFILES)
    CC $(CFLAGS) display
eval.obj : eval.c,$(HFILES)
    CC $(CFLAGS) eval
exec.obj : exec.c,$(HFILES)
    CC $(CFLAGS) exec
file.obj : file.c,$(HFILES)
    CC $(CFLAGS) file
fileio.obj : fileio.c,$(HFILES)
    CC $(CFLAGS) fileio
hp110.obj : hp110.c,$(HFILES)
    CC $(CFLAGS) hp110
hp150.obj : hp150.c,$(HFILES)
    CC $(CFLAGS) hp150
ibmpc.obj : ibmpc.c,$(HFILES)
    CC $(CFLAGS) ibmpc
input.obj : input.c,$(HFILES)
    CC $(CFLAGS) input
isearch.obj : isearch.c,$(HFILES)
    CC $(CFLAGS) isearch
line.obj : line.c,$(HFILES)
    CC $(CFLAGS) line
lock.obj : lock.c,$(HFILES)
    CC $(CFLAGS) lock
main.obj : main.c,$(HFILES)
    CC $(CFLAGS) main
mouse.obj : mouse.c,$(HFILES)
    CC $(CFLAGS) mouse
random.obj : random.c,$(HFILES)
    CC $(CFLAGS) random
region.obj : region.c,$(HFILES)
    CC $(CFLAGS) region
search.obj : search.c,$(HFILES)
    CC $(CFLAGS) search
st520.obj : st520.c,$(HFILES)
    CC $(CFLAGS) st520
tcap.obj : tcap.c,$(HFILES)
    CC $(CFLAGS) tcap
vms.obj : vms.c,$(HFILES)
    CC $(CFLAGS) vms
tipc.obj : tipc.c,$(HFILES)
    CC $(CFLAGS) tipc
vmsvt.obj : vmsvt.c,$(HFILES)
    CC $(CFLAGS) vmsvt
vt52.obj : vt52.c,$(HFILES)
    CC $(CFLAGS) vt52
window.obj : window.c,$(HFILES)
    CC $(CFLAGS) window
word.obj : word.c,$(HFILES)
    CC $(CFLAGS) word
z309.obj : z309.c,$(HFILES)
    CC $(CFLAGS) z309
