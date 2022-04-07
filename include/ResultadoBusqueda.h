#ifndef FRAG

class Resultado_Busqueda
{
    private:
        std::string palabraAnterior;
        std::string palabraPosterior;
        int lineaResultado;
    public:
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
};
#endif 