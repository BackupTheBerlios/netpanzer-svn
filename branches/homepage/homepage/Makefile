XMLFILES = $(wildcard *.xml)
HTMLFILES = $(addsuffix .html, $(basename $(XMLFILES)))
OUTPUTFILES = $(HTMLFILES) default.css

all : $(OUTPUTFILES)

clean :
	rm -vf $(HTMLFILES)

%.html :: %.xml default.xsl menu.xml Makefile
	xsltproc -o $*.html default.xsl $*.xml

uploadhtml: all
	scp $(OUTPUTFILES) matzebraun@cvs.berlios.de:/home/groups/netpanzer/htdocs

upload: all
	scp $(OUTPUTFILES) matzebraun@cvs.berlios.de:/home/groups/netpanzer/htdocs
	scp -r images matzebraun@cvs.berlios.de:/home/groups/netpanzer/htdocs

commit: all
	cvs commit -m "automatic webpage update"

.PHONY: all clean upload

