#include <buscador.h>

class Peticion{
    private:
        int libro;
        int idPeticion;
        std::string palabra;
        bool finPeticion;
    
    public:
         Peticion();

    Peticion(int, std::string);

    int getidPeticion()
    {
        return idPeticion;
    }

    int getLibro()
    {
        return libro;
    }

    void siguienteLibro()
    {
        libro++;
    }

    std::string getPalabra()
    {
        return palabra;
    }

    bool getFinPeticion()
    {
        return finPeticion;
    }
    void fin_Peticion()
    {
        finPeticion = true;
    }

};