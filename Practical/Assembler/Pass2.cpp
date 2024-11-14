#include <bits/stdc++.h>  // Includes all standard libraries for convenience
using namespace std;

// Function to fetch the address of a symbol or literal from its respective table
string table(ifstream &fin, string n) {
    string no, name, addr;  // Variables to store entries from the table

    // Loop to read entries from the file (symbol table or literal table)
    while(fin >> no >> name >> addr) {
        if(no == n) {  // If the number matches, return the corresponding address
            fin.seekg(0, ios::beg);  // Reset file pointer to beginning
            return addr;  // Return the address found in the table
        }
    }
    fin.seekg(0, ios::beg);  // Reset file pointer to the beginning for the next lookup
    return "NAN";  // Return "NAN" if the entry is not found
}

int main() {
    ifstream ic, st, lt;  // Input file streams for intermediate code, symbol table, and literal table
    ic.open("ic.txt");    // Open intermediate code file
    st.open("symtable.txt");  // Open symbol table file
    lt.open("littable.txt");  // Open literal table file

    ofstream mc;  // Output file stream for machine code output
    mc.open("machine_code.txt");  // Open the machine code output file

    string lc, ic1, ic2, ic3;  // Variables to hold parts of each intermediate code line

    // Output headers for the console display
    cout << "\n -- ASSEMBLER PASS-2 OUTPUT --" << endl;
    cout << "\n LC\t <INTERMEDIATE CODE>\t\t\tLC\t <MACHINE CODE>" << endl;

    // Main loop to read each line of the intermediate code (ic) file
    while(ic >> lc >> ic1 >> ic2 >> ic3) {  // Read location counter (LC) and intermediate code parts
        string MC;  // Variable to store generated machine code

        // If opcode is Assembler Directive (AD) or Declarative (DL) for DS (storage)
        if(ic1.substr(1, 2) == "AD" || (ic1.substr(1, 2) == "DL" && ic1.substr(4, 2) == "02"))
            MC = " -No Machine Code-";  // No machine code for AD and DS (DL,02)

        // If opcode is DL (Declarative) and specifies DC (constant)
        else if(ic1.substr(1, 2) == "DL" && ic1.substr(4, 2) == "01") {
            // Generate machine code with constant value
            MC = "00\t0\t00" + ic2.substr(3, 1);  // Formatting for constant with "00 0 00[constant]"
        }

        else {  // Otherwise, the opcode is an Imperative Statement (IS)
            if(ic1 == "(IS,00)")  // Special case for STOP opcode
                MC = ic1.substr(4, 2) + "\t0\t000";  // "00 0 000" format for STOP instruction

            else if(ic2.substr(1, 1) == "S")  // ORIGIN statement with symbol address
                MC = ic1.substr(4, 2) + "\t0\t" + table(st, ic2.substr(4, 1));  // Get symbol address from table

            else {  // Handle general IS opcodes with symbols or literals
                if(ic3.substr(1, 1) == "S")  // If third argument is a symbol
                    MC = ic1.substr(4, 2) + "\t" + ic2.substr(1, 1) + "\t" + table(st, ic3.substr(4, 1));  // Symbol lookup
                else  // If third argument is a literal
                    MC = ic1.substr(4, 2) + "\t" + ic2.substr(1, 1) + "\t" + table(lt, ic3.substr(4, 1));  // Literal lookup
            }
        }

        // Special formatting for ORIGIN directive for console output
        if(ic1 == "(AD,03)") {
            cout << " " << lc << "\t" << ic1 << "\t" << ic2 << " " << ic3 << "\t\t\t" << lc << "\t" << MC << endl;
            mc << lc << "\t" << MC << endl;
            continue;  // Skip to the next iteration
        }

        // Print line of machine code to console
        cout << " " << lc << "\t" << ic1 << "\t" << ic2 << "\t " << ic3 << "\t\t\t" << lc << "\t" << MC << endl;

        // Write machine code to the output file
        mc << lc << "\t" << MC << endl;
    }

    return 0;
}
