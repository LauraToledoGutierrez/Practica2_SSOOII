#include <finder.h>

class Request{
    private:
        //int book;
        int idRequest;
        int idClient;
        std::string wordToSearch;
        bool endRequest;
    
    public:
         Request();
    
    

    Request(int idRequest, int idClient, std::string wordToSearch): idRequest(idRequest), idClient(idClient), wordToSearch(wordToSearch){}

    int getIdRequest()
    {
        return idRequest;
    }
    int getIdClient()
    {
        return idClient;
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