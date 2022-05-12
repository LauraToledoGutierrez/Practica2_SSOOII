#include "definitions.h"
#include "client.h"
#include "colors.h"
#include "finder.h"
#include "search_result.h"

class Request{
    private:
        //int book;
        int idRequest;
        int idClient;
        int typeClient;
        std::string wordToSearch;
        int finishedThreads; //Amount of finder threads that have already finished
        bool endRequest;
    
    public:
         Request();
    
    std::vector<std::vector<Search_Result>> searchResults;


    Request(int idRequest, int idClient, std::string wordToSearch, int typeClient): idRequest(idRequest), idClient(idClient), wordToSearch(wordToSearch), typeClient(typeClient){}

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
    int getFinishedThreads(){
        return finishedThreads;
    }
    void setFinishedThreads(int finishedThreads){
        this->finishedThreads=finishedThreads;
    }
};