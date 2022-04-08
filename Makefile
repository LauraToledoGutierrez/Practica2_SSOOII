FILE := prueba.txt
WORD := david
NTHREADS := 3

DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/
DIRBOOKS := Libros_P2/
CFLAGS := -I$(DIRHEA) -c -Wall -ansi
CPPFLAGS:= -I$(DIRHEA) -c -Wall -std=c++11
LDLIBS := -pthread
CC := g++

INC := include/color.h include/

all: dirs SSOOIIGLE 

dirs:
	mkdir -p $(DIROBJ) $(DIREXE)
        
SSOOIIGLE: $(DIROBJ)SSOOIIGLE.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

$(DIROBJ)%.o: $(DIRSRC)%.cpp
	$(CC) $(CPPFLAGS) $^ -o $@

ejecutar:
	./$(DIREXE)SSOOIIGLE $(DIRBOOKS)$(FILE) $(WORD) $(NTHREADS)

clean:
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRHEA)*~ 
		
