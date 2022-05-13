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
        std::string previousWord; //The previous word than the finded word
        std::string nextWord; //The next word than the finded word
        int resultLine; //The line where we find the word
    public:
        /*********
        *Method: setPreviousWord
        *Description: Updates previous word in the book
        *Parameters: string previousWord
        */
        void setPreviousWord(std::string word)
        {
            previousWord=word;
        }
        /*********
        *Method: setNextWord
        *Description: Updates next word in the book
        *Parameters: string nextWord
        */
        void setNextWord(std::string word)
        {
            nextWord=word;
        }
        /*********
        *Method: seteesultLine
        *Description: Updates the line that we find the word in the book
        *Parameters: int resultLine
        */
        void setresultLine(int linea)
        {
            resultLine=linea;
        }
        /*********
        *Method: getResultLine
        *Description: Returns the line where we find the word
        *Return type: int
        */
        int getresultLine()
        {
            return resultLine;
        }
        /*********
        *Method: getPreviousWord
        *Description: Returns the previous word
        *Return type: string
        */
        std::string getPreviousWord()
        {
            return previousWord;
        }
        /*********
         *Method: getNextWord
        *Description: Returns the next word
        *Return type: string
        */
        std::string getNextWord()
        {
            return nextWord;
        }
};
