#pragma once

#include "definitions.h"
#include "request.h"
#include "search_result.h"

extern std::vector<Request> clientRequestFree;
extern std::vector<Request> requestsDone;
extern std::vector<Request> clientRequestPremium;
extern void printResults(Request req);
extern std::vector<std::string> readFile(std::string bookPath);
extern void findWord(int nbook, int iteration, std::vector<std::string> vector, int lowerLimit, int uppwerLimit, Request requestCurrent);
extern void systemPay();

class Finder
{
    /***********************************
     * Class: Finder
     * Description: This class is the finder that searches the word in the method findWord in the main class. Creates the "Master" Finder threads
     * which funcionality is to find the word in all of books 
     * Date: 13/05/2022
     ***********************************/
private:
    std::string searchedWord;       //Word that the client want to search in all books
    int id; 
    int initialLine;                // First line of the book
    int finalLine;                  //The last line of the book

public:
    std::vector<std::string> books = {"./Libros_P2/17-LEYES-DEL-TRABJO-EN-EQUIPO.txt", "./Libros_P2/21-LEYES-DEL-LIDERAZGO.txt", "./Libros_P2/25-MANERAS-DE-GANARSE-A-LA-GENTE.txt",
"./Libros_P2/ACTITUD-DE-VENDEDOR.txt", "./Libros_P2/El-oro-y-la-ceniza.txt", "./Libros_P2/La-última-sirena.txt", "./Libros_P2/prueba.txt",
"./Libros_P2/SEAMOS-PERSONAS-DE-INFLUENCIA.txt", "./Libros_P2/VIVE-TU-SUEÑO.txt"};  //Vector of all the available books/sources it can be searched into
    std::vector<std::thread> vFinderThreads; //Vector of Finder threads

    /***********************************
     * Method: operator()
     * Description: The finder takes the Request from the queue clientRequestFree and clientRequestPremium and creates the "children" Finder threads
     * one for each book which execute the method findWord
     * 
     * Version: 1.0
     * Date: 13/05/2022
     ***********************************/
    void operator()() 
    {
        Request req(0,0,"",0); //An object of Request
        std::mutex mutex; //Mutex
        Search_Result sr; //An object of Search_Results

    
        while (1)
        {
            cvClient.wait(uniLockQRequests, []
                        { return !clientRequestFree.empty() || !clientRequestPremium.empty(); });
            std::unique_lock<std::mutex> lck (mutex);

            if (!clientRequestPremium.empty())
            {
                req = clientRequestPremium.front(); 
                clientRequestPremium.pop_back();
            }
            
            else if (!clientRequestFree.empty())
            {
                req = clientRequestFree.front();
                clientRequestFree.pop_back();
            }
            else
            {
                std::cout << RED<<"No available requests" << RESET<<std::endl; //The queues are empty
            }
            // Functionality Children Finder
            
            for (int i = 0; i < books.size(); i++)
            {
                std::vector<std::string> fileToRead = readFile(books[i]);

                // METHOD CREATEFINDERTHREADS
                for (int j = 0; j < NUMBERTHREADS; j++)
                {
                    int lowerLimit, upperLimit; //Limits 

                    // If is the first thread
                    if (j == 0)
                        lowerLimit = 1;
                    else
                        lowerLimit = ((fileToRead.size() / NUMBERTHREADS) * j + 1);

                    // If is the last thread
                    if (j == NUMBERTHREADS - 1)
                        upperLimit = fileToRead.size();
                    else
                        upperLimit = (lowerLimit + (fileToRead.size() / NUMBERTHREADS) - 1);

                    // Create an object Finder with her values
                    Finder finder(req.getwordToSearch(), j, lowerLimit, upperLimit);
                    // All of thread execute the method findWord 
                    vFinderThreads.push_back(std::thread(findWord, i, j, fileToRead, lowerLimit, upperLimit, req));
                }
            }
            while (1)
            {
                cvFinder.wait(uniLockFinderChildren, [] {return !requestsDone.empty() ;});
                        
                if (req.getEndRequest()) //The request is finished
                {
                    requestsDone.push_back(req); 
                    break;
                }
                else if (!req.getEndRequest())  //The request isn't finished
                {
                    systemPay();
                }
            }
        }
        std::for_each(vFinderThreads.begin(), vFinderThreads.end(), std::mem_fn(&std::thread::join));
    }

    // Constructor, getters and setters

    Finder() {}
    Finder(std::string palabra, int Id, int firstLine, int lastLine) : searchedWord(palabra), id(Id), initialLine(firstLine), finalLine(lastLine) {}

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
};

