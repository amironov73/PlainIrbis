.PHONY: all clean

all:
	$(MAKE) -C libs  all
	$(MAKE) -C tests all
	$(MAKE) -C apps  all

clean:
	$(MAKE) -C libs  clean
	$(MAKE) -C tests clean
	$(MAKE) -C apps  clean
