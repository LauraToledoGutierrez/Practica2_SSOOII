#pragma once

#include "definitions.h"
#include "request.h"

extern std::vector<Request> clientRequestFree;
extern std::vector<Request> requestsDone;
extern std::vector<Request> clientRequestPremium;
extern void printResults(Request req);
extern std::vector<std::string> readFile(std::string bookPath);
extern void findWord(int nbook, int iteration, std::vector<std::string> vector, int lowerLimit, int uppwerLimit, Request requestCurrent);
extern void systemPay();

//Request req(0,0,"",0);

class Finder
{
private:
    std::string searchedWord;
    int id;
    int initialLine;
    int finalLine;


public:
    std::vector<std::string> books = {"./Libros_P2/17-LEYES-DEL-TRABJO-EN-EQUIPO.txt", "./Libros_P2/21-LEYES-DEL-LIDERAZGO.txt", "./Libros_P2/25-MANERAS-DE-GANARSE-A-LA-GENTE.txt",
"./Libros_P2/ACTITUD-DE-VENDEDOR.txt", "./Libros_P2/El-oro-y-la-ceniza.txt", "./Libros_P2/La-última-sirena.txt", "./Libros_P2/prueba.txt",
"./Libros_P2/SEAMOS-PERSONAS-DE-INFLUENCIA.txt", "./Libros_P2/VIVE-TU-SUEÑO.txt"};
    int tamanio= books.size();
    std::vector<std::thread> vFinderThreads;

    /***********************************
     * Method: createFinderThreads
     * Description: Divides a source into several parts so that each thread can look into a part of the source file
     * Parameters: numberLines(number of total lines of the source file), wordToSearch(word that we are searching in the files), partialLines(all the lines a thread must look into)
     * Version: 1.0
     * Date: 10/04/2022

    void Finder::createFinderThread(int numberLines, std::string wordToSearch, std::vector<std::string> partialLines)
    {

        std::for_each(vFinderThreads.begin(), vFinderThreads.end(), std::mem_fn(&std::thread::join));
    }
     ***********************************/
    void operator()() 
    {
        Request req(0,0,"",0);
        std::mutex mutexNOSE;
       
        //! NEcesitamos exclusion mutua para sacar de la cola de peticiones

        while (1)
        {
            cvClient.wait(uniLockQRequests, []
                          { return !clientRequestFree.empty() || !clientRequestPremium.empty(); });
            int random = 1 + rand() % (100 - 1);
           // Request req;
            std::unique_lock<std::mutex> lck (mutexNOSE);

            if (random <= 79 && !clientRequestPremium.empty())
            {
                //std::cout<<"ME ABURRO"<<std::endl;
                req = clientRequestPremium.front(); 
                //std::cout << "nose "<<req.getIdClient()<<std::endl;

                //std::unique_lock<std::mutex> lck (mutexNOSE);
                clientRequestPremium.pop_back();
            }
            
            else if (random > 79 && !clientRequestFree.empty())
            {
                //std::cout<<"ME ABURRO x2"<<std::endl;
                req = clientRequestFree.front();
                //std::cout << "nose "<<req.getIdClient()<<std::endl;
                //std::unique_lock<std::mutex> lck (mutexNOSE);
                clientRequestFree.pop_back();
            }
            else
            {
                std::cout << "No available requests" << std::endl;
            }
            // Functionality Children Finder
            
            for (int i = 0; i < books.size(); i++)
            {
                std::vector<std::string> fileToRead = readFile(books[i]);
                
                //std::cout<<"LINREAS"<<lines<<std::endl;
                //std::cout << "aaaaaaaaaaaaaaaaaaaaaaaa" <<std::endl;

                // METHOD CREATEFINDERTHREADS
                for (int j = 0; j < NUMBERTHREADS; j++)
                {
                    int lowerLimit, upperLimit;

                    // Si es el primer hilo
                    if (j == 0)
                        lowerLimit = 1;
                    else
                        lowerLimit = ((fileToRead.size() / NUMBERTHREADS) * j + 1);

                    // Si es el ultimo hilo
                    if (j == NUMBERTHREADS - 1)
                        upperLimit = fileToRead.size();
                    else
                        upperLimit = (lowerLimit + (fileToRead.size() / NUMBERTHREADS) - 1);

                    // Creamos el objeto finder con sus correspondientes valores
                    //std::cout <<"ALGO "<<upperLimit<<std::endl;
                    Finder finder(req.getwordToSearch(), j, lowerLimit, upperLimit);
                    //threadFinder.push_back(finder);

                    // Cada hilo realizara el metodo de buscarPalabra
                    //std::cout << "aaaaaaaaaaaaaaaaaaaaaaaa" <<std::endl;
                    vFinderThreads.push_back(std::thread(findWord, i, j, fileToRead, lowerLimit, upperLimit, req));
                    while (1)
                    {
                        cvFinder.wait(uniLockFinderChildren, [] {return !requestsDone.empty();});
                        if (req.getEndRequest())
                        {
                            requestsDone.push_back(req);
                            std::for_each(vFinderThreads.begin(), vFinderThreads.end(), std::mem_fn(&std::thread::join));
                            break;
                        }
                        else if (!req.getEndRequest())
                        {
                            systemPay();
                        }
                    }
                }
                std::for_each(vFinderThreads.begin(), vFinderThreads.end(), std::mem_fn(&std::thread::detach));
            }
        }
    }
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

