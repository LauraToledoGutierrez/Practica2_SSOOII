
#pragma once

#include <iostream>
#include <string>
#include <ctype.h>
#include <locale>
#include <algorithm>
#include <thread>
#include <vector>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <queue>
#include <functional>
#include <iterator>
#include <sstream>
#include <mutex>
#include <pthread.h>
#include <atomic>
#include <bits/stdc++.h>

#define NUMBERCLIENTS 20
#define NUMBERTHREADS 3
#define NUMBERFINDER 4

std::vector<std::thread> vThreadClient;
std::vector<std::string> bookPath;
std::mutex mutex;
std::mutex mutexAlgo;
std::mutex mutexQRequests;
std::mutex mutexFinderChildren;
std::mutex mutexReduceBalance;
std::mutex mutexFinishedThreads;
std::unique_lock<std::mutex> uniLockQRequests(mutexQRequests);
std::unique_lock<std::mutex> uniLockFinderChildren(mutexFinderChildren);


std::condition_variable cvClient;
std::condition_variable cvPay;
std::condition_variable cvFinder;

std::atomic<int> g_id_request(0);
std::atomic<int> g_id_client(0);
std::vector<std::string> books = {"./17-LEYES-DEL-TRABJO-EN-EQUIPO.txt", "./21-LEYES-DEL-LIDERAZGO.txt", "./25-MANERAS-DE-GANARSE-A-LA-GENTE.txt",
"./ACTITUD-DE-VENDEDOR.txt", "./El-oro-y-la-ceniza.txt", "./La-última-sirena.txt", "./prueba.txt",
"./SEAMOS-PERSONAS-DE-INFLUENCIA.txt", "./VIVE-TU-SUEÑO.txt"};