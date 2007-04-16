XMLFILES = $(wildcard *.xml)
HTMLFILES = $(addsuffix .html, $(basename $(XMLFILES)))
OUTPUTFILES = $(HTMLFILES) default.css

all : $(OUTPUTFILES)

clean :
	rm -vf $(HTMLFILES)

%.html :: %.xml default.xsl menu.xml Makefile
	xsltproc -o $*.html default.xsl $*.xml

kromupload: all
	scp $(OUTPUTFILES) kromxp@shell.berlios.de:/home/groups/netpanzer/htdocs

uploadhtml: all
	scp $(OUTPUTFILES) matzebraun@shell.berlios.de:/home/groups/netpanzer/htdocs

upload: all
	scp $(OUTPUTFILES) matzebraun@shell.berlios.de:/home/groups/netpanzer/htdocs
	scp -r images matzebraun@shell.berlios.de:/home/groups/netpanzer/htdocs

fenio: all
	scp $(OUTPUTFILES) fenio@shell.berlios.de:/home/groups/netpanzer/htdocs
	scp -r images fenio@shell.berlios.de:/home/groups/netpanzer/htdocs

commit: all
	cvs commit -m "automatic webpage update"

.PHONY: all clean upload

