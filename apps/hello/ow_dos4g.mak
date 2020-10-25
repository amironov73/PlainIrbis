CC = wcc386
CFLAGS = -zq -bt=dos4g -IC:\irbis\include -IC:\irbis\libs\include

application = hello.exe

objects = obj\main.obj

all: $(application)

obj\main.obj: src\main.c
	$(CC) $(CFLAGS) -fo=$@ $<

$(application): $(objects)
        %write tmp.lnk name $@
        %write tmp.lnk system dos4g
        %write tmp.lnk file {$(objects)}
        %write tmp.lnk library C:\irbis\libs\magna\magna.lib, C:\irbis\libs\irbis\irbis.lib
	wlink @tmp.lnk
