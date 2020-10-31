CC = wcc
CFLAGS = -zq -bt=dos -IC:\irbis\include -IC:\irbis\libs\include

library = irbis.lib

objects = obj\address.obj  &
	obj\alphatab.obj   &
	obj\author.obj     &
	obj\bookinfo.obj   &
	obj\codes.obj      &
	obj\collecti.obj   &
	obj\connect.obj    &
	obj\dbinfo.obj     &
	obj\dll.obj        &
	obj\ean.obj        &
	obj\error.obj      &
	obj\exemplar.obj   &
	obj\field.obj      &
	obj\field203.obj   &
	obj\format.obj     &
	obj\gbl.obj        &
	obj\guard.obj      &
	obj\ilf.obj        &
	obj\impex.obj      &
	obj\ini.obj        &
	obj\iri.obj        &
	obj\isbn.obj       &
	obj\isbninfo.obj   &
	obj\iso2709.obj    &
	obj\magazine.obj   &
	obj\menu.obj       &
	obj\mst.obj        &
	obj\opt.obj        &
	obj\par.obj        &
	obj\phantom.obj    &
	obj\procinfo.obj   &
	obj\query.obj      &
	obj\rawrecor.obj   &
	obj\reader.obj     &
	obj\record.obj     &
	obj\registr.obj    &
	obj\resource.obj   &
	obj\response.obj   &
	obj\search.obj     &
	obj\serializ.obj   &
	obj\servstat.obj   &
	obj\spec.obj       &
	obj\subfield.obj   &
	obj\stw.obj        &
	obj\tabledef.obj   &
	obj\term.obj       &
	obj\title.obj      &
	obj\tree.obj       &
	obj\upc.obj        &
	obj\uppertab.obj   &
	obj\userinfo.obj   &
	obj\version.obj    &
	obj\visit.obj      &
	obj\xrf.obj

all: $(library)

obj\address.obj: src\address.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\alphatab.obj: src\alphatab.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\author.obj: src\author.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\bookinfo.obj: src\bookinfo.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\codes.obj: src\codes.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\collecti.obj: src\collecti.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\connect.obj: src\connect.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\dbinfo.obj: src\dbinfo.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\dll.obj: src\dll.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\ean.obj: src\ean.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\error.obj: src\error.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\exemplar.obj: src\exemplar.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\field.obj: src\field.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\field203.obj: src\field203.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\format.obj: src\format.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\gbl.obj: src\gbl.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\guard.obj: src\guard.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\ilf.obj: src\ilf.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\impex.obj: src\impex.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\ini.obj: src\ini.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\iri.obj: src\iri.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\isbn.obj: src\isbn.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\isbninfo.obj: src\isbninfo.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\iso2709.obj: src\iso2709.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\magazine.obj: src\magazine.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\menu.obj: src\menu.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\mst.obj: src\mst.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\opt.obj: src\opt.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\par.obj: src\par.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\phantom.obj: src\phantom.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\procinfo.obj: src\procinfo.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\query.obj: src\query.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\rawrecor.obj: src\rawrecor.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\reader.obj: src\reader.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\record.obj: src\record.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\registr.obj: src\registr.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\resource.obj: src\resource.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\response.obj: src\response.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\search.obj: src\search.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\serializ.obj: src\serializ.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\servstat.obj: src\servstat.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\spec.obj: src\spec.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\stw.obj: src\stw.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\subfield.obj: src\subfield.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\tabledef.obj: src\tabledef.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\term.obj: src\term.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\title.obj: src\title.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\tree.obj: src\tree.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\upc.obj: src\upc.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\uppertab.obj: src\uppertab.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\userinfo.obj: src\userinfo.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\version.obj: src\version.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\visit.obj: src\visit.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\xrf.obj: src\xrf.c
	$(CC) $(CFLAGS) -fo=$@ $<

$(library): $(objects)
	%write tmp.lbc + $<
	wlib -q $@ @tmp
