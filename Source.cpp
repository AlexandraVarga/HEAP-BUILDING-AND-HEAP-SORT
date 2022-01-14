/*
    
    
    Tema 3 - Varga Alexandra-Ioana - grupa 30223



*/

#include <stdio.h>
#include <iostream>
#include "Profiler.h"

Profiler p("Heap");

void afisare(int* a, int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);
}

/*

Din reprezentarea grafica se observa ca:

- varianta bottom_up face mai putine asignari decat varianta top_down
- varianta top_down face mai putine comparatii decat varianta bottom_up
- doar ca per total varianta bottom_up este mai eficienta decat cea top_down
- O(n*lg n), iar cazul defavorabil se intalneste atunci cand numerele sunt descrescatoare in sir

Strategia Top-Down == inserarea succesiva a unor noi noduri, la inceput frunze
si refacerea, la fiecare pas, a proprietatii de heap
prin interschimbarea repetata a nodului nou inserat cu nodul parinte pana cand conditia de heap este refacuta

*/


void construct_heap_top_down(int sir[], int size_sir) {
    
    Operation comparatii = p.createOperation("COMPARATII_TOP_DOWN", size_sir);
    Operation asignari = p.createOperation("ASIGNARI_TOP_DOWN", size_sir);
    Operation total = p.createOperation("TOTAL_TOP_DOWN", size_sir);
    
    int heap[10000];
    for (int i = 0; i < size_sir; i++) {
        int j = i;
        heap[j] = sir[j];
        comparatii.count();
        total.count();
        while ((j - 1) / 2 > 0 && heap[(j - 1) / 2] > heap[j]) {
            comparatii.count();
            total.count();
            int aux = heap[j];
            heap[j] = heap[(j - 1) / 2];
            heap[(j - 1) / 2] = aux;
            asignari.count(3);
            total.count(3);
            j = (j - 1) / 2;
        }
    }
    for (int i = 0; i < size_sir; i++)
        sir[i] = heap[i];
    asignari.count(size_sir);
    total.count(size_sir);
    
}

/*

O(lg n)
Fiecare apel al lui heapify() se face in O(lg n) si se fac O(n) astfel de apeluri => construct_heap_bottom_up()
se executa in O(n*lg n) , iar cazul defavorabil se intalneste atunci cand numerele sunt descrescatoare in sir

*/

void heapify(int sir[], int size_sir, int i, int x) 
{
        if (x == 0) 
        {
        Operation comparatii = p.createOperation("COMPARATII_BOTTOM_UP", size_sir);
        Operation asignari = p.createOperation("ASIGNARI_BOTTOM_UP", size_sir);
        Operation total = p.createOperation("TOTAL_BOTTOM_UP", size_sir);
        
        int smallest = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        comparatii.count();
        total.count();
        if (l < size_sir && sir[l] < sir[smallest])
            smallest = l;
        comparatii.count();
        total.count();
        if (r < size_sir && sir[r] < sir[smallest])
            smallest = r;

        if (smallest != i)
        {
            int aux = sir[i];
            sir[i] = sir[smallest];
            sir[smallest] = aux;

            asignari.count(3);
            total.count(3);
            heapify(sir, size_sir, smallest, 0);
        }
    }
    else if (x == 1) 
    {
        int smallest = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        if (l < size_sir && sir[l] < sir[smallest])
            smallest = l;
        if (r < size_sir && sir[r] < sir[smallest])
            smallest = r;

        if (smallest != i)
        {
            int aux = sir[i];
            sir[i] = sir[smallest];
            sir[smallest] = aux;
            heapify(sir, size_sir, smallest, 1);
        }
    }
}

/*

Strategia Bottom-Up de constructie a unui heap == adaugarea unui nod radacina 
ce are drept copil stang si drept doi subarbori care respecta
proprietatea de HEAP si „scufundarea” radacinii la nivelul corespunzator astfel incat 
proprietatea de heap sa fie refacuta dupa unirea celor doua heap-uri deja
existente
(2 heap-uri + 1 nod radacina = 1 nou heap)

*/


void construct_heap_bottom_up(int sir[], int size_sir) 
{
    
    for (int i = size_sir / 2 - 1; i >= 0; i--) 
    {
        heapify(sir, size_sir, i, 0);//x==0
    }
}

/*

Fiecare apel al lui heapify() se face in O(lg n) si se fac n-1 astfel de apeluri => heapSort()
se executa in O(n*lg n)

*/

void heapSort(int sir[], int size_sir) 
{
    
    for (int i = size_sir / 2 - 1; i >= 0; i--)
        heapify(sir, size_sir, i, 1);//x==1
    
    int schimb[10000];
    for (int i = size_sir - 1; i >= 0; i--)
    {
        int aux = sir[0];
        sir[0] = sir[i];
        sir[i] = aux;
        heapify(sir, i, 0, 1);//x==1
    }
    
    for (int i = 0; i < size_sir; i++)
        schimb[i] = sir[size_sir - 1 - i];
    
    for (int i = 0; i < size_sir; i++)
        sir[i] = schimb[i];
}

void demo()
{
    int a[10000];
    int n = 10;
    FillRandomArray(a, n);
    //afisare(a, n);

    heapSort(a, n);
    //afisare(a, n);

    for (int n = 100; n <= 10000; n += 100) {
        for (int k = 0; k < 5; k++) {
            FillRandomArray(a, n);
            construct_heap_top_down(a, n);
        }
    }
    
    for (int n = 100; n <= 10000; n += 100) {
        for (int k = 0; k < 5; k++) {
            FillRandomArray(a, n);
            construct_heap_bottom_up(a, n);
        }
    }

    p.divideValues("COMPARATII_TOP_DOWN", 5);
    p.divideValues("COMPARATII_BOTTOM_UP", 5);
    p.divideValues("ASIGNARI_TOP_DOWN", 5);
    p.divideValues("ASIGNARI_BOTTOM_UP", 5);
    p.divideValues("TOTAL_TOP_DOWN", 5);
    p.divideValues("TOTAL_BOTTOM_UP", 5);
    p.createGroup("ASIGNARI CAZ MEDIU", "ASIGNARI_TOP_DOWN", "ASIGNARI_BOTTOM_UP");
    p.createGroup("COMPARATII CAZ MEDIU", "COMPARATII_TOP_DOWN", "COMPARATII_BOTTOM_UP");
    p.createGroup("TOTAL CAZ MEDIU", "TOTAL_TOP_DOWN", "TOTAL_BOTTOM_UP");
    p.showReport();
    
}
int main()
{
    demo();

	return 0;
}