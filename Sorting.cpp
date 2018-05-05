#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include <deque>

using namespace std;

void quicksort(long long int*, long long int, long long int);
long long int get_file_size(string filename);
long long int MAX = 99999999999;
int main()

{

    int end = -1;

    long long int size, RamSize = 1024 * 1024;

    long long int FileSize
        = get_file_size("number.txt") - 4;
    long long int SizeInCode = (FileSize / 12) * 8;
    long long int TotalDigits = SizeInCode / 8;

    long long int ChunkSize = RamSize / 8; // Number of digits in single chunk, Since long long int takes 8 bytes.

    long long int NumberOfChunks[2];
    NumberOfChunks[0] = ceil((double(TotalDigits)) / double(ChunkSize));

    cout << "File Size = " << FileSize << "\n";
    cout << "NumberOfChunks = " << NumberOfChunks << "\n";
    cout << "SizeInCode = " << SizeInCode << "\n";
    cout << "ChunkSize = " << ChunkSize << "\n";

    long long int* ptr;
    long long int i, k, count = 0, Value, Chunk;

    ptr = (long long int*)malloc(ChunkSize * sizeof(long long int));

    int* FilePosition = new int[NumberOfChunks[0]];

    FilePosition[0] = 0;
    FILE *FileRead, *FileWrite;
//Number.txt Contains data
    FileRead = (fopen("C:\\Users\\Rajesh\\Desktop\\workspace2\\sort_C++\\number.txt", "r"));
    if (FileRead == NULL) {
        printf("Error!");
        exit(0);
    }
    FileWrite = (fopen("file0.txt", "w"));

    //Read part of the data from file
    //Sort the data using QuickSort
    //Store it into new file
    //Repeat the above steps till all the chunks are sorted

    for (Chunk = 0; Chunk < NumberOfChunks[0]; Chunk++) {
        for (i = 0; i < ChunkSize; i++) {

            fscanf(FileRead, "%lld", ptr + i);

            if (ptr[i] == -1) {
                break;
            }
        }
        size = i;
        quicksort(ptr, 0, size - 1);
        printf("chunk %lld sorted\n", Chunk);
        for (i = 0; i < size; i++) {
            fprintf(FileWrite, "%lld\n", ptr[i]);
        }
        fprintf(FileWrite, "%d\n", end);

        if (Chunk < (NumberOfChunks[0] - 1)) {
            FilePosition[Chunk + 1] = ftell(FileWrite);
        }
        if (ptr[i] == -1) {

            break;
        }
    }
    free(ptr);
    fclose(FileWrite);
    fclose(FileRead);

    cout << "Chunk Sorting Done";


    int CombineChunks[2];
    CombineChunks[0] = 32;
    int TotalCombinedChunks[2];

    TotalCombinedChunks[0] = ceil(double(NumberOfChunks[0]) / double(CombineChunks[0]));
    TotalCombinedChunks[1] = 1;

    CombineChunks[1] = TotalCombinedChunks[0];
    const char* FileReading[2] = { "file0.txt", "file1.txt" };
    const char* FileWriting[2] = { "file1.txt", "file0.txt" };

    NumberOfChunks[1] = TotalCombinedChunks[0];

    int* TempPosition = new int[TotalCombinedChunks[0]];
    TempPosition[0] = 0;

    //Layer is how many level merging is required
    for (int Layer = 0; Layer < 2; Layer++) {
        if (Layer != 0)

        {
            FilePosition = TempPosition;
        }

        FileRead = (fopen(FileReading[Layer], "r"));
        FileWrite = (fopen(FileWriting[Layer], "w"));

        int SingleQueueSize;
        SingleQueueSize = ChunkSize / CombineChunks[Layer];

        deque<long long int>* Dqueue = new deque<long long int>[CombineChunks[Layer]];

        long long int OffSet = CombineChunks[Layer];
        int l;
        //Combine Chunks
        //Read elements from Sorted chunks
        //Initially fill all the Queue
        for (Chunk = 0; Chunk < TotalCombinedChunks[Layer]; Chunk++)

        {
            cout << Chunk << "\n";
            OffSet = Chunk * CombineChunks[Layer];
            for (i = 0; i < CombineChunks[Layer]; i++) {
                if ((i + OffSet) < NumberOfChunks[Layer]) {
                    fseek(FileRead, FilePosition[i + OffSet], SEEK_SET);
                    while (int(Dqueue[i].size()) < SingleQueueSize) {

                        fscanf(FileRead, "%lld", &Value);
                        if (Value == -1)

                        {
                            Value = MAX;
                        }
                        Dqueue[i].push_back(Value);
                    }
                    FilePosition[i + OffSet] = ftell(FileRead);
                }
                else {

                    Dqueue[i].push_back(MAX);
                }
            }

            int Q;
            Value = 0;

            Q = 0;



            //Find the Smallest element among all the Queue
            //Store that element in new file and pop that element from Queue
            //When the Queue is Empty , Read elements from sorted chunks
            while (true) {

                Value = Dqueue[0].front();
                Q = 0;
                for (k = 0; k < CombineChunks[Layer] - 1; k++) {
                    if (Value > Dqueue[k + 1].front()) {
                        Value = Dqueue[k + 1].front();
                        Q = k + 1;
                    }
                }
                if (Value == MAX)

                {
                    count = 1;
                    break;
                }
                fprintf(FileWrite, "%lld\n", Value);
                Dqueue[Q].pop_front();
                if (Dqueue[Q].size() == 0) {
                    fseek(FileRead, FilePosition[Q + OffSet], SEEK_SET);
                    while (int(Dqueue[Q].size()) < SingleQueueSize) {
                        fscanf(FileRead, "%lld", &Value);
                        if (Value == -1)

                        {
                            Value = MAX;
                        }
                        Dqueue[Q].push_back(Value);
                    }
                    FilePosition[Q + OffSet] = ftell(FileRead);
                }
            }

            if (count == 1) {
                fprintf(FileWrite, "%d\n", end);
                if (Chunk < (TotalCombinedChunks[0] - 1))

                {
                    TempPosition[Chunk + 1] = ftell(FileWrite);
                }
                cout << "\nSorting Complete\n";
            }

            for (int h = 0; h < CombineChunks[Layer]; h++) {

                Dqueue[h].clear();
            }
        }

        free(Dqueue);
        fclose(FileRead);
        fclose(FileWrite);
        free(FilePosition);

        cout << "\nDone !!!!!!!!\n";
    }
    return 0;
}
void quicksort(long long int x[], long long int first, long long int last)
{
    long long int pivot, j, temp, i;

    if (first < last) {
        pivot = first;
        i = first;
        j = last;

        while (i < j) {
            while (x[i] <= x[pivot] && i < last)
                i++;
            while (x[j] > x[pivot])
                j--;
            if (i < j) {
                temp = x[i];
                x[i] = x[j];
                x[j] = temp;
            }
        }

        temp = x[pivot];
        x[pivot] = x[j];
        x[j] = temp;
        quicksort(x, first, j - 1);
        quicksort(x, j + 1, last);
    }
}
long long int get_file_size(string filename)
{
    FILE* p_file = NULL;
    p_file = fopen(filename.c_str(), "rb");
    fseek(p_file, 0, SEEK_END);
    long long int size = ftell(p_file);
    fclose(p_file);
    return size;
}
