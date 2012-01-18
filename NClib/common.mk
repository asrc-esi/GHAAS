UNIX=$(shell uname)

ifeq ($(UNIX),Darwin)
ifndef ($(CUSTOM_INC))
	CUSTOM_INC=-I/sw/include
endif
endif

export UNIXAR=ar -ru
export UNIXCC=gcc
export UNIXCCOPS=-Wall -fsigned-char -D_GNU_SOURCE $(CUSTOM_INC)
export UNIXMAKE=make
export UNIXRL=ranlib
