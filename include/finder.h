
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

    public:
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