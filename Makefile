FILE := El-oro-y-la-ceniza.txt
WORD := superviviente
NTHREADS := 20

DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/
DIRBOOKS := Libros_P2/
CPPFLAGS:= -I$(DIRHEA) -c -std=c++11
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
		
