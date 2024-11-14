#include <bits/stdc++.h>
#define max_block_size 1000
using namespace std;

void bestfit(int block_[], int process[], int b, int p)
{
    int block[b];
    for (int i = 0; i < b; i++)
    {
        block[i] = block_[i];
    }
    cout << endl;
    cout << "Applying BEST FIT algorithm..." << endl;
    cout << "_" << endl;
    cout << "Process No.    Process Size     Block no.   Block size remaining" << endl;
    cout << "_" << endl;

    int allotted[b];
    for (int i = 0; i < b; i++)
    {
        allotted[i] = -1;
    }
    for (int i = 0; i < p; i++)
    {
        int min = max_block_size;
        int temp = -1;
        for (int j = 0; j < b; j++)
        {
            if (allotted[j] == -1 && block[j] <= min && block[j] >= process[i])
            {
                min = block[j];
                temp = j;
            }
        }
        if (temp == -1)
        {
            cout << "    " << i + 1 << "               " << process[i] << "           " << "-" << "              " << "-" << endl;
        }
        else
        {
            allotted[temp] = i;
            block[temp] -= process[i];
            cout << "    " << i + 1 << "               " << process[i] << "           " << temp + 1 << "              " << block[temp] << endl;
        }
    }
    cout << "_" << endl;
}

void worstfit(int block_[], int process[], int b, int p)
{
    int block[b];
    for (int i = 0; i < b; i++)
    {
        block[i] = block_[i];
    }
    cout << endl;
    cout << "Applying WORST FIT algorithm..." << endl;
    cout << "_" << endl;
    cout << "Process No.    Process Size     Block no.   Block size remaining" << endl;
    cout << "_" << endl;

    for (int i = 0; i < p; i++)
    {
        int max = -1;
        int temp = -1;
        for (int j = 0; j < b; j++)
        {
            if (block[j] >= process[i] && block[j] > max)
            {
                max = block[j];
                temp = j;
            }
        }
        if (temp == -1)
        {
            cout << "    " << i + 1 << "               " << process[i] << "           " << "-" << "              " << "-" << endl;
        }
        else
        {
            block[temp] -= process[i];
            cout << "    " << i + 1 << "               " << process[i] << "           " << temp + 1 << "              " << block[temp] << endl;
        }
    }
    cout << "_" << endl;
}

void firstfit(int block_[], int process[], int b, int p)
{
    int block[b];
    for (int i = 0; i < b; i++)
    {
        block[i] = block_[i];
    }
    cout << endl;
    cout << "Applying FIRST FIT algorithm..." << endl;
    cout << "_" << endl;
    cout << "Process No.    Process Size     Block no.   Block size remaining" << endl;
    cout << "_" << endl;

    for (int i = 0; i < p; i++)
    {
        int temp = -1;
        for (int j = 0; j < b; j++)
        {
            if (block[j] >= process[i])
            {
                temp = j;
                break;
            }
        }
        if (temp == -1)
        {
            cout << "    " << i + 1 << "               " << process[i] << "           " << "-" << "              " << "-" << endl;
        }
        else
        {
            block[temp] -= process[i];
            cout << "    " << i + 1 << "               " << process[i] << "           " << temp + 1 << "              " << block[temp] << endl;
        }
    }
    cout << "_" << endl;
}

void nextfit(int block_[], int process[], int b, int p)
{
    int block[b];
    for (int i = 0; i < b; i++)
    {
        block[i] = block_[i];
    }

    cout << endl;
    cout << "Applying NEXT FIT algorithm..." << endl;
    cout << "_" << endl;
    cout << "Process No.    Process Size     Block no.   Block size remaining" << endl;
    cout << "_" << endl;

    int x = 0;
    for (int i = 0; i < p; i++)
    {
        int temp = -1;
        for (int j = x; j < b; j = (j + 1) % b)
        {
            if (block[j] >= process[i])
            {
                temp = j;
                x = (temp + 1) % b;
                break;
            }
            if (j == b - 1) break;
        }
        if (temp == -1)
        {
            cout << "    " << i + 1 << "               " << process[i] << "           " << "-" << "              " << "-" << endl;
        }
        else
        {
            block[temp] -= process[i];
            cout << "    " << i + 1 << "               " << process[i] << "           " << temp + 1 << "              " << block[temp] << endl;
        }
    }
    cout << "_" << endl;
}

int main()
{
    int b, p;
    cout << endl << "Enter the number of blocks: ";
    cin >> b;
    int block[b];
    cout << "Enter the size of each block: " << endl;
    for (int i = 0; i < b; i++)
    {
        cout << "Block " << i + 1 << ": ";
        cin >> block[i];
    }
    cout << "DONE_" << endl;
    cout << endl << "Enter the number of processes: ";
    cin >> p;
    int process[p];
    for (int i = 0; i < p; i++)
    {
        cout << "Process " << i + 1 << ": ";
        cin >> process[i];
    }
    cout << "_DONE_" << endl;

    char op = 'y';
    while (op == 'y')
    {
        int choice = 0;
        cout << "MEMORY ALLOCATION METHODS" << endl << endl;
        cout << "1. Best Fit" << endl;
        cout << "2. Worst Fit" << endl;
        cout << "3. First Fit" << endl;
        cout << "4. Next Fit" << endl;
        cout << "_" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            bestfit(block, process, b, p);
            break;
        case 2:
            worstfit(block, process, b, p);
            break;
        case 3:
            firstfit(block, process, b, p);
            break;
        case 4:
            nextfit(block, process, b, p);
            break;
        default:
            cout << "INVALID INPUT" << endl;
        }

        cout << "DO YOU WANT TO CONTINUE (y/n): ";
        cin >> op;
    }
    return 0;
}