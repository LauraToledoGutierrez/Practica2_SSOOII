#include <finder.h>

class Request{
    private:
        //int book;
        int idRequest;
        std::string wordToSearch;
        bool endRequest;
    
    public:
         Request();
    
    std::promise<std::queue<Search_Result>> prom;
    std::future<std::queue<Search_Result>> fut= prom.get_future();

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