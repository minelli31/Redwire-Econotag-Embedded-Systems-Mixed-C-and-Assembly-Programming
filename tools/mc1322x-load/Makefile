INSTALL= ../bin

TARGET = mc1322x-load

CFLAGS = -Wall -Wextra #-Werror

all: $(TARGET)

clean:
	-rm -f $(TARGET)

install: all $(INSTALL)
	cp $(TARGET) $(INSTALL)

$(INSTALL):
	mkdir $(INSTALL)

