/******************************
 * Class: Request
 * Description: This is the Request Class where all the attributes, methods, getters and setters are implemented
 *
 ******************************/
#pragma once
#include "definitions.h"
#include "search_result.h"

class Request
{
private:
    int idRequest;            // Unique ID that identifies every Request object
    int idClient;             // ID corresponding to the Client that created this Request
    int typeClient;           // Type of the associated Client with this request
                              // 0: Premium plus.      1: Premium with Balance.        2: Free client
    std::string wordToSearch; // Contains the word to be searched by the Finders
    int finishedThreads;      // Holds the amount of finder threads that have already finished
    bool endRequest;          // Boolean to warn if the request has already finished or not

public:
    Request();

    std::vector<Search_Result> searchResults; // This vector will store all the results found for this Request

    // Constructor for the Request Object
    Request(int idRequest, int idClient, std::string wordToSearch, int typeClient) : idRequest(idRequest), idClient(idClient), wordToSearch(wordToSearch), typeClient(typeClient) {}

    /*********
     *Method: getIdRequest
     *Description: Returns the idRequest for this Request
     *Return type: int
     */
    int getIdRequest()
    {
        return idRequest;
    }

    /*********
     *Method: getIdClient
     *Description: Returns the id of the client associated with this Request
     *Return type: int
     */
    int getIdClient()
    {
        return idClient;
    }

    /*********
     *Method: getwordToSearch
     *Description: Returns the word to be searched by the program
     *Return type: std::string
     */
    std::string getwordToSearch()
    {
        return wordToSearch;
    }

    /*********
     *Method: getEndRequest
     *Description: Returns if the request has been finalized or not
     *Return type: boolean
     */
    bool getEndRequest()
    {
        return endRequest;
    }

    /*********
     *Method: end_Request
     *Description: Sets the endRequest variable to True
     */
    void end_Request()
    {
        endRequest = true;
    }

    /*********
     *Method: getSearchResults
     *Description: Returns the vector with all the results found for this Request
     *Return type: std::vector<Search_Result>
     */
    std::vector<Search_Result> getSearchResults()
    {
        return searchResults;
    }

    /*********
     *Method: setSearchResults
     *Description: Updates the vector holding the results by the given one by parameter
     *Parameters: std::vector<Search_Result> Search_Result
     */
    void setSearchResults(std::vector<Search_Result> Search_Result)
    {
        this->searchResults = searchResults;
    }

    /*********
     *Method: getFinishedThreads
     *Description: Returns the amount of threads that have already finished their search
     *Return type: int
     */
    int getFinishedThreads()
    {
        return finishedThreads;
    }

    /*********
     *Method: setFinishedThreads
     *Description: Updates the amount of threads that have already finished by the given one by parameter
     *Parameters: int finishedThreads
     */
    void setFinishedThreads(int finishedThreads)
    {
        this->finishedThreads = finishedThreads;
    }

    /*********
     *Method: getTypeClient
     *Description: Returns the type of client that has created this Request. It is returned as a String for more understandability while debugging
     *Return type: std::string
     */
    std::string getTypeClient()
    {
        switch (typeClient)
        {
        case 0:
            return "PREMIUM PLUS";
            break;
        case 1:
            return "PREMIUM CON SALDO";
            break;
        case 2:
            return "GRATUITO";
            break;
        default:
            return "ERROR: IMPOSSIBLE TYPE OF CLIENT";
            break;
        }
    }
};
