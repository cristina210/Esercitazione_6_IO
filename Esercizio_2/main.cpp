#include <iostream>
#include <chrono>
#include <unistd.h>
#include <vector>
#include <numeric>
#include <SortingAlgorithm.hpp>
#include <cstdlib>

double durataMS(vector<double> &v)
{
    chrono::steady_clock::time_point t_begin_MS=std::chrono::steady_clock::now();
    SortLibrary::MergeSort(v, 0, v.size() - 1);
    chrono::steady_clock::time_point t_end_MS=std::chrono::steady_clock::now();
    double duration_MS = std::chrono::duration_cast<std::chrono::nanoseconds>(t_end_MS-t_begin_MS).count();
    return duration_MS;
}

double durataBS(vector<double> &v)
{
    chrono::steady_clock::time_point t_begin_MS=std::chrono::steady_clock::now();
    SortLibrary::BubbleSort(v);
    chrono::steady_clock::time_point t_end_MS=std::chrono::steady_clock::now();
    double duration_MS = std::chrono::duration_cast<std::chrono::nanoseconds>(t_end_MS-t_begin_MS).count();
    return duration_MS;
}
/// Nella riga di comando sono inseriti tre dimensioni dei vettori, la prima sarà usata in 1) e le altre due
/// rappresentano invece la dimensione massima e minima in 2). Es: 300, 60, 200
using namespace std;
int main(int argc, char **argv)
{
    ///1) Calcolo tempo computazionale di Bubble sort e Merge sort rispetto al vettore dato tramite riga di comando
    char* nonConv1;
    char* nonConv2;
    char* nonConv3;
    if (argc<4)
    {
        cerr << "numero di argomenti inseriti nella riga di comando non valido, inserisci tre interi"<<endl;
        return 1;
    }
    long int size = strtol(argv[1], &nonConv1, 10);
    if (size <= 1 || *nonConv1 != '\0')
    {
        cerr<<"Errore nell'input del primo argomento in riga di comando"<<endl;
    }
    else
    {
    vector<double> v1(size);
    unsigned int i = 0;
    for(unsigned int j = 0; j<size ; j++) // creo un vettore di numeri randomici
    {
        v1[j] = double(rand());
    }
    vector<double> v2 = v1;
    double durationMS = durataMS(v1);
    double durationBS = durataBS(v2);
    cout << "~ Tempo di calcolo in nanosecondi per un vettore random di dimensione " << size << " (assegnata per riga di comando) ~"<<endl;
    cout << "Bubble sort: "<<durationBS<<endl;
    cout << "Merge sort: "<<durationMS<<endl;
    cout << '\n';
    }

    ///2) CONFRONTO 2: Mostro come bubble sort è preferibile a merge sort in caso di vettori di dimensioni ridotte.
    //int num = std::stoi(str) converto in intero l
    long int minDim = strtol(argv[2], &nonConv2, 10);
    long int maxDim = strtol(argv[3], &nonConv3, 10);
    if(*nonConv2 != '\0' || *nonConv3 !='\0' || minDim>=maxDim || minDim<=1)
    {
        cerr<<"Errore nell'imput del secondo e/o terzo argomento in riga di comando"<<endl;
    }
    else
    {
        vector<double> mean_duration_MergeSort(maxDim - minDim);
        vector<double> mean_duration_BubbleSort(maxDim - minDim);
        for (int i = minDim; i<maxDim; i++) //creo vettori con dimensione crescente
        {
            size_t size = i;
            vector<double> vec1(size);
            double sum_duration_MergeSort = 0;
            double sum_duration_BubbleSort = 0;
            unsigned int n = 10;
            for(int k = 0; k<n; k++) // creo n vettori di dimensioni fissata
            {
                for(unsigned int j = 0; j<size ; j++)
                {
                    vec1[j] = double(rand())/RAND_MAX;
                }
                vector<double> vec2 = vec1;
                double duration_Merge_Sort = durataMS(vec1);
                sum_duration_MergeSort = sum_duration_MergeSort + duration_Merge_Sort;
                double duration_Bubble_Sort = durataBS(vec2);
                sum_duration_BubbleSort = sum_duration_BubbleSort + duration_Bubble_Sort;
            }
            mean_duration_MergeSort[i-minDim] = sum_duration_MergeSort/n;
            mean_duration_BubbleSort[i-minDim] = sum_duration_BubbleSort/n;
        }
        cout << "~ Confronto tra efficienza di Bubble sort e Merge sort rispetto alla dimensione dei vettori da ordinare ~"<<endl;
        cout << "Medie del tempo (in nanosecondi) impiegato dai due algoritmi per ordinare vettori con dimensione crescente con"<<endl;
        cout << "Dimensione iniziale: " << minDim << endl;
        cout << "Dimensione finale: " << maxDim << endl;
        cout << '\n';
        cout << "Bubble sort: " << endl;
        for (size_t i = 0; i < mean_duration_BubbleSort.size(); ++i)
        {
            cout << mean_duration_BubbleSort[i] << " ";
        }
        cout << std::endl;
        cout << '\n';
        cout << "Merge sort: " << endl;
        for (size_t i = 0; i < mean_duration_MergeSort.size(); ++i)
        {
            cout << mean_duration_MergeSort[i] << " ";
        }
        cout << std::endl;
        cout << '\n';
        cout << "Delta di tempi tra tempi per Bubble sort e Merge sort a dimensione dei vettori crescente:"<<endl;
        for (size_t i = 0; i < mean_duration_BubbleSort.size(); ++i) {
            std::cout << mean_duration_BubbleSort[i]-mean_duration_MergeSort[i] << ", ";
        }
        cout << endl;
        cout << "-> Migliore performance del Bubble sort solo per vettori a dimensione ridotta "<< endl;
        cout << "-> Differenza tra i tempi di Bubble sort e Merge sort in aumento con l'aumentare della dimensione dei vettori che dimostra il differente andamento dei tempi nei due algoritmi (O(n^2) e O(nlog(n))"<<endl;
    // per vedere questo ho impostato nella riga di comando minDim = 60 e maxDim = 200 (per esempio)
    }
    ///3) CONFRONTO 3: Mostro come bubble sort è talvolta preferibile a merge sort se il vettore è già parzialmente ordinato.
    int t;
    vector<double> vec3(1000);
    iota(vec3.begin(),vec3.end(),t);
    for (unsigned int i =10;i<50;i++) //lo ordino solo parzialmente
    {
        vec3[i] = rand(); //sostituisco i primi numeri con dei numeri randomici distribuiti uniformemente tra 0 e randmax
    }
    vector<double> vec4 = vec3;
    double duration_Merge_Sort = durataMS(vec3);
    double duration_Bubble_Sort = durataBS(vec4);
    cout << '\n';
    cout << "~ Ordinamento di vettori parzialmente ordinati ~" << endl;
    cout << "Si nota come il tempo impiegato dal Merge sort (" << duration_Merge_Sort <<" nanosecondi) e' maggiore di quello impiegato dal Bubble sort (" << duration_Bubble_Sort <<" nanosecondi)" << endl;
    cout << "-> Il Bubble sort e' piu' efficiente del Merge sort, anche per vettori a grandi dimensioni, se questi sono gia' parzialmente ordinati"<<endl;
    return 0;
}



