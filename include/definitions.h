
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

#define NUMBERCLIENTS 4
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
std::vector<std::string> books = {"./Libros_P2/17-LEYES-DEL-TRABJO-EN-EQUIPO.txt", "./Libros_P2/21-LEYES-DEL-LIDERAZGO.txt", "./Libros_P2/25-MANERAS-DE-GANARSE-A-LA-GENTE.txt",
"./Libros_P2/ACTITUD-DE-VENDEDOR.txt", "./Libros_P2/El-oro-y-la-ceniza.txt", "./Libros_P2/La-última-sirena.txt", "./Libros_P2/prueba.txt",
"./Libros_P2/SEAMOS-PERSONAS-DE-INFLUENCIA.txt", "./Libros_P2/VIVE-TU-SUEÑO.txt"};