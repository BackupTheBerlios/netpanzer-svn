AUTOMAKE_OPTIONS = foreign
SUBDIRS = src

EXTRA_DIST = autogen.sh ChangeLog TODO 

picsdir = @datadir@/netpanzer/pics
pics_DATA = pics/chars8x8.raw pics/chars5x5.raw

all-local: src/netpanzer
	ln -sf src/netpanzer

