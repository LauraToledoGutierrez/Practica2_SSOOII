#include <finder.h>

class Request{
    private:
        //int book;
        int idRequest;
        std::string wordToSearch;
        bool endRequest;
    
    public:
         Request();

    Request(int idRequest, std::string wordToSearch): idRequest(idRequest), wordToSearch(wordToSearch){}

    int getIdRequest()
    {
        return idRequest;
    }

    /*int getBook()
    {
        return book;
    }

    void nextBook()
    {
        book++;
    }*/

    std::string getwordToSearch()
    {
        return wordToSearch;
    }

    bool getEndRequest()
    {
        return endRequest;
    }
    void end_Request()
    {
        endRequest = true;
    }

};