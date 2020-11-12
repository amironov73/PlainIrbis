CC = wcc
CFLAGS = -zq -bt=dos -IC:\irbis\include -IC:\irbis\libs\include

library = magna.lib

objects = obj\array.obj     &
	obj\beep.obj        &
	obj\buffer.obj      &
	obj\chain.obj       &
	obj\chunked.obj     &
	obj\cp1251.obj      &
	obj\cp866.obj       &
	obj\date.obj        &
	obj\delphi.obj      &
	obj\dump.obj        &
	obj\encoding.obj    &
	obj\enumertr.obj    &
	obj\error.obj       &
	obj\fastbuf.obj     &
	obj\file.obj        &
	obj\format.obj      &
	obj\handle.obj      &
	obj\intarray.obj    &
	obj\io.obj          &
	obj\keyboard.obj    &
	obj\koi8r.obj       &
	obj\list.obj        &
	obj\logging.obj     &
	obj\magna.obj       &
	obj\map.obj         &
	obj\mapfile.obj     &
	obj\memory.obj      &
	obj\navigatr.obj    &
	obj\number.obj      &
	obj\object.obj      &
	obj\path.obj        &
	obj\process.obj     &
	obj\retry.obj       &
	obj\screen.obj      &
	obj\sleep.obj       &
	obj\span.obj        &
	obj\spanarry.obj    &
	obj\stream.obj      &
	obj\string.obj      &
	obj\tcp4.obj        &
	obj\thread.obj      &
	obj\utf7.obj        &
	obj\utf8.obj        &
	obj\utils.obj       &
	obj\vector.obj

all: $(library)

obj\array.obj: src\array.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\beep.obj: src\beep.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\buffer.obj: src\buffer.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\chain.obj: src\chain.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\chunked.obj: src\chunked.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\cp1251.obj: src\cp1251.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\cp866.obj: src\cp866.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\date.obj: src\date.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\delphi.obj: src\delphi.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\dump.obj: src\dump.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\encoding.obj: src\encoding.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\enumertr.obj: src\enumertr.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\error.obj: src\error.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\fastbuf.obj: src\fastbuf.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\file.obj: src\file.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\format.obj: src\format.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\handle.obj: src\handle.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\intarray.obj: src\intarray.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\io.obj: src\io.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\keyboard.obj: src\keyboard.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\koi8r.obj: src\koi8r.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\list.obj: src\list.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\logging.obj: src\logging.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\magna.obj: src\magna.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\map.obj: src\map.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\mapfile.obj: src\mapfile.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\memory.obj: src\memory.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\navigatr.obj: src\navigatr.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\number.obj: src\number.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\object.obj: src\object.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\path.obj: src\path.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\process.obj: src\process.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\retry.obj: src\retry.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\screen.obj: src\screen.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\sleep.obj: src\sleep.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\span.obj: src\span.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\spanarry.obj: src\spanarry.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\stream.obj: src\stream.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\string.obj: src\string.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\tcp4.obj: src\tcp4.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\thread.obj: src\thread.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\utf8.obj: src\utf7.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\utf8.obj: src\utf8.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\utils.obj: src\utils.c
	$(CC) $(CFLAGS) -fo=$@ $<

obj\vector.obj: src\vector.c
	$(CC) $(CFLAGS) -fo=$@ $<

$(library): $(objects)
        %write tmp.lbc + $<
	wlib -q $@ @tmp
