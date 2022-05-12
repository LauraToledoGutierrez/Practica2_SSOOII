#include <finder.h>

class Request{
    private:
        //int book;
        int idRequest;
        int idClient;
        std::string wordToSearch;
        bool endRequest;
        std::vector<std::vector<Search_Result>> searchResults;
    
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

    std::vector<std::vector<Search_Result>> getSearchResults(){
        return searchResults;
    }
    void setSearchResults(std::vector<std::vector<Search_Result>> Search_Result){
        this->searchResults=searchResults;
    }
};