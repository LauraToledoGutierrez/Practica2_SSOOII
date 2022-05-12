#include "definitions.h"


class Search_Result
{
    private:
        std::string previousWord;
        std::string nextWord;
        int resultLine;
    public:
        void setPreviousWord(std::string word)
        {
            previousWord=word;
        }

        void setNextWord(std::string word)
        {
            nextWord=word;
        }
        void setresultLine(int linea)
        {
            resultLine=linea;
        }
        int getresultLine()
        {
            return resultLine;
        }
        std::string getPreviousWord()
        {
            return previousWord;
        }
        std::string getNextWord()
        {
            return nextWord;
        }
};