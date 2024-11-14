#include<bits/stdc++.h>
using namespace std;

int main() {
    // Open the necessary input and output files
    ifstream br("input.txt");      // input.txt - Macro definition source
    ofstream mnt("mnt.txt");       // mnt.txt - Macro Name Table (MNT)
    ofstream mdt("mdt.txt");       // mdt.txt - Macro Definition Table (MDT)
    ofstream kpdt("kpdt.txt");     // kpdt.txt - Keyword Parameter Definition Table (KPDT)
    ofstream pnt("pntab.txt");     // pntab.txt - Parameter Name Table (PNT)
    ofstream ir("intermediate.txt"); // intermediate.txt - Intermediate code

    // Check if all files are successfully opened
    if (!br.is_open() || !mnt.is_open() || !mdt.is_open() || !kpdt.is_open() || !pnt.is_open() || !ir.is_open()) {
        cerr << "Error opening one or more files!" << endl;
        return 1;  // Exit if file opening fails
    }

    // Declare variables
    map<string, int> pntab;       // Store parameter names with assigned numbers
    string line;                  // To hold each line read from the input file
    string Macroname;             // To hold the current macro name
    int mdtp = 1, kpdtp = 0, paramNo = 1, pp = 0, kp = 0, flag = 0;

    // Start reading the input file line by line
    while (getline(br, line)) {
        // Split the line into parts (words)
        istringstream iss(line);
        vector<string> parts((istream_iterator<string>(iss)), istream_iterator<string>());

        // Check if the line contains a macro definition start ("MACRO")
        if (parts[0] == "MACRO" || parts[0] == "macro") {
            flag = 1;  // Set flag indicating that we are inside a macro definition
            if (!getline(br, line)) break;  // Read the next line (macro parameters)
            istringstream iss2(line);
            vector<string> parts2((istream_iterator<string>(iss2)), istream_iterator<string>());
            Macroname = parts2[0];  // Get the macro name from the next line

            // If no parameters, directly add to MNT and continue
            if (parts2.size() <= 1) {
                mnt << parts2[0] << "\t" << pp << "\t" << kp << "\t" << mdtp << "\t" << (kp == 0 ? kpdtp : (kpdtp + 1)) << "\n";
                continue;
            }

            // Handle parameters passed to the macro
            for (size_t i = 1; i < parts2.size(); ++i) {
                string param = parts2[i];
                // Clean up parameters by removing '&' and ','
                string result = "";
                for (char c : param) {
                    if (c != '&' && c != ',') {
                        result += c;
                    }
                }
                param = result;

                // Check if the parameter is a keyword parameter (i.e., has '=')
                if (param.find('=') != string::npos) {
                    ++kp;  // Increment keyword parameter count
                    size_t pos = param.find('=');
                    string keywordParam = param.substr(0, pos);
                    string value = param.substr(pos + 1);
                    pntab[keywordParam] = paramNo++;  // Add to parameter table with a unique number
                    kpdt << keywordParam << "\t" << value << "\n";  // Write to KPDT
                } else {
                    pntab[param] = paramNo++;  // Regular parameter
                    ++pp;  // Increment positional parameter count
                }
            }

            // Add the macro name and details to MNT
            mnt << parts2[0] << "\t" << pp << "\t" << kp << "\t" << mdtp << "\t" << (kp == 0 ? kpdtp : (kpdtp + 1)) << "\n";
            kpdtp += kp;  // Update keyword parameter table pointer
            kp = 0;       // Reset keyword parameter counter
            pp = 0;       // Reset positional parameter counter

        } else if (parts[0] == "MEND" || parts[0] == "mend") {  // Macro end ("MEND")
            mdt << line << "\n";  // Write MEND to MDT
            flag = kp = pp = 0;   // Reset flags and counters
            ++mdtp;               // Increment MDT pointer
            paramNo = 1;          // Reset parameter number

            // Output the parameter names of the macro to PNT
            pnt << Macroname << ":\t";
            for (const auto& pair : pntab) {
                pnt << pair.first << "\t";  // Print each parameter name
            }
            pnt << "\n";  // End the line
            pntab.clear(); // Clear parameter table for the next macro

        } else if (flag == 1) {  // While inside a macro definition
            // Process macro invocation by replacing parameters
            for (const auto& part : parts) {
                if (part.find('&') != string::npos) {  // Keyword parameter (starts with '&')
                    string param = part;
                    string result = "";
                    for (char c : param) {
                        if (c != '&' && c != ',') {
                            result += c;  // Clean the parameter
                        }
                    }
                    param = result;
                    mdt << "(P," << pntab[param] << ")\t";  // Replace with position in PNT
                } else {
                    mdt << part << "\t";  // Regular part of the macro
                }
            }
            mdt << "\n";  // End of line in MDT
            ++mdtp;  // Increment MDT pointer
        } else {
            // If not inside a macro definition, write the line to intermediate file
            ir << line << "\n";
        }
    }

    // Close all open file streams
    br.close();
    mnt.close();
    mdt.close();
    kpdt.close();
    pnt.close();
    ir.close();

    cout << "Macro Pass1 Processing done. :)" << endl;  // Indicate successful completion
    return 0;  // Exit the program
}
