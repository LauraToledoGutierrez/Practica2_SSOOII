
DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/
DIRBOOKS := Libros_P2/
CPPFLAGS:= -I$(DIRHEA) -c -std=c++11
LDLIBS := -pthread
CC := g++


all: dirs SSOOIIGLE 

dirs:
	mkdir -p $(DIROBJ) $(DIREXE)
        
        
SSOOIIGLE: $(DIROBJ)SSOOIIGLE.o 
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

$(DIROBJ)%.o: $(DIRSRC)%.cpp
	$(CC) $(CPPFLAGS) $^ -o $@

ejecutar:
	./$(DIREXE)SSOOIIGLE 

clean:
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRHEA)*~ 
		
