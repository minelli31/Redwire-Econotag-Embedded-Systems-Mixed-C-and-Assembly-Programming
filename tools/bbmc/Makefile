INSTALL= ../bin

LDLIBS = -lftdi

TARGET = bbmc

CFLAGS = -Wall -Wextra #-Werror

all: $(TARGET)

clean:
	-rm -f $(TARGET)

install: all $(INSTALL)
	cp $(TARGET) $(INSTALL)

$(INSTALL):
	mkdir $(INSTALL)

