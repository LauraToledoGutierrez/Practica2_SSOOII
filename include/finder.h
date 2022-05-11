
#include <iostream>
#include <queue>
#include "search_result.h"
class Finder
{
    private:
        std::string searchedWord;
        int id;
        int initialLine;
        int finalLine;
        std::queue<Search_Result> queueResults;
        std::vector<std::string> books = {"17-LEYES-DEL-TRABJO-EN-EQUIPO.txt", "21-LEYES-DEL-LIDERAZGO.txt", "25-MANERAS-DE-GANARSE-A-LA-GENTE.txt",
                        "ACTITUD-DE-VENDEDOR.txt", "El-oro-y-la-ceniza.txt", "La-última-sirena.txt", "prueba.txt", 
                        "SEAMOS-PERSONAS-DE-INFLUENCIA.txt", "VIVE-TU-SUEÑO.txt"};
        std::atomic<int> id_request()
    public:

    void operator() () const{
        //!NEcesitamos exclusion mutua para sacar de la cola de peticiones
        cvClient.wait(uniLockQRequests, [] {return !clientRequestFree.empty() || !clientRequestPremium.empty();});
        int random= 1+ rand()%(100-1);
        Request req;
        if(random<=79 & !clientRequestPremium.empty()){
            req=std::move(clientRequestPremium.front());
            clientRequestPremium.pop();
        }else if(random>79 & !clientRequestFree.empty()){
            req=std::move(clientRequestFree.front());
            clientRequestFree.pop();
        }else{
            std::cout<<"No hay peticiones disponibles"<<std::endl;
        }




    }
        Finder(){}
        Finder(std::string palabra, int Id, int firstLine, int lastLine): searchedWord(palabra), id(Id), initialLine(firstLine), finalLine(lastLine){}
        
        std::string getSearchedWord()
        {
            return searchedWord;
        }

        void setId(int id)
        {
            this->id = id;
        }
        int getId()
        {
            return id;
        }

        int getInitialLine()
        {
            return initialLine;
        }
        void setInitialLine(int initialLine)
        {
            this->initialLine = initialLine;
        }

        int getFinalLine()
        {
            return finalLine;
        }
        void setFinalLine(int finalLine)
        {
            this->finalLine = finalLine;
        }

        std::queue<Search_Result> getQueueResults()
        {
            return queueResults;
        }

        void setQueueResults(std::queue<Search_Result> resultados)
        {
            queueResults = resultados;
        }
};