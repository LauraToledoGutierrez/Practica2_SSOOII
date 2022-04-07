#include <ResultadoBusqueda.h>

class Buscador
{
    private:
        std::string palabraBuscada;
        int id;
        int lineaInicio;
        int lineaFinal;
        std::queue<Resultado_Busqueda> colaResultados;

    public:
        Buscador(){}
        Buscador(std::string palabra, int Id, int lineaAnt, int lineaPost): palabraBuscada(palabra), id(Id), lineaInicio(lineaAnt), lineaFinal(lineaPost){}
        std::string getPalabraBuscada()
        {
            return palabraBuscada;
        }

        void setColaResultados(std::queue<Resultado_Busqueda> resultados)
        {
            colaResultados= resultados;
        }
        void setId(int id)
        {
            this->id = id;
        }
        int getId()
        {
            return id;
        }

        int getLineaInicio()
        {
            return lineaInicio;
        }
        void setLineaInicio(int lineaFinal)
        {
            this->lineaInicio = lineaInicio;
        }

        int getLineaFinal()
        {
            return lineaFinal;
        }
        void setLineaFinal(int lineaFinal)
        {
            this->lineaFinal = lineaFinal;
        }

        std::queue<Resultado_Busqueda> getColaResultados()
        {
            return colaResultados;
        }
};