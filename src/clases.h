#ifndef Resultado_Busqueda

class Resultado_Busqueda
{
    private:
        std::string palabraAnterior;
        std::string palabraPosterior;
        int lineaResultado;
    public:
        Resultado_Busqueda(){}
        Resultado_Busqueda(std::string palabraAnterior, std::string palabraPosterior, int lineaResultado){
            this->palabraAnterior = palabraAnterior;
            this->palabraPosterior = palabraPosterior;
            this->lineaResultado = lineaResultado;
        }

        void setPalabraAnterior(std::string palabra)
        {
            palabraAnterior=palabra;
        }

        void setPalabraPosterior(std::string palabra)
        {
            palabraPosterior=palabra;
        }
        void setLineaResultado(int linea)
        {
            lineaResultado=linea;
        }
        int getLineaResultado()
        {
            return lineaResultado;
        }
        std::string getPalabraAnterior()
        {
            return palabraAnterior;
        }
        std::string getPalabraPosterior()
        {
            return palabraPosterior;
        }
        void toString(Resultado_Busqueda resultado){
            std::cout << resultado.getLineaResultado() << " " << resultado.getPalabraAnterior() << " " <<resultado.getPalabraPosterior() << std::endl; 

        }
};

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
            colaResultados = resultados;
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
        void toString(Buscador buscador)
        {
            std::cout<<buscador.getId()<<" "<<buscador.getLineaInicio()<<" "<<buscador.getLineaFinal() <<" "<<std::endl;
        }
};
#endif 