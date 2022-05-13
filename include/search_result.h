#pragma once

#include "./definitions.h"


class Search_Result
{
    /***********************************
     * Class: Search_Result
     * Description: In this class we are going to save the attributes of a search (the previous and the next word to the one we want to find 
     * and the line where the word is)
     * Date: 13/05/2022
     ***********************************/
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
