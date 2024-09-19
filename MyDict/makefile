ROOT_CFLAGS     = $(shell root-config --cflags)
ROOT_LIBS       = $(shell root-config --libs)

CC        = g++
CFLAGS    = ${ROOT_CFLAGS} -Iinclude 

DICT_DIR  = ./
DICT      = $(DICT_DIR)/MyDict
DICT_SRC  = $(DICT).cxx
DICT_OBJ  = $(DICT).o
DICT_LIB  = $(DICT).so

all : $(DICT_LIB)

$(DICT_SRC): MyDict.h LinkDef.h
	@mkdir -p $(DICT_DIR)
	rootcling -f $@ -c $^

$(DICT_OBJ): $(DICT_SRC)
	${CC} -std=c++17 -c ${CFLAGS} -fPIC $< -o $@

$(DICT_LIB): $(DICT_OBJ)
	${CC} -shared -o $@ $< ${ROOT_LIBS} 

#clean
clean : 
	rm -f $(DICT_SRC)
	rm -f $(DICT_OBJ)
	rm -f $(DICT_LIB)