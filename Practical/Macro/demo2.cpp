#include<bits/stdc++.h>
using namespace std;

// Define a class to represent an entry in the Macro Name Table (MNT)
class MNTEntry {
public:
    // Default constructor initializing the attributes
    MNTEntry() : name(""), pp(0), kp(0), mdtp(0), kpdtp(0) {}

    // Parameterized constructor to initialize MNT entry with given values
    MNTEntry(const string& name, int pp, int kp, int mdtp, int kpdtp)
        : name(name), pp(pp), kp(kp), mdtp(mdtp), kpdtp(kpdtp) {}

    // Getter functions to retrieve values from MNT entry
    int getPp() const { return pp; }
    int getKp() const { return kp; }
    int getMdtp() const { return mdtp; }
    int getKpdtp() const { return kpdtp; }

private:
    string name;  // Macro name
    int pp;       // Number of positional parameters
    int kp;       // Number of keyword parameters
    int mdtp;     // Pointer to the Macro Definition Table (MDT)
    int kpdtp;    // Pointer to the Keyword Parameter Definition Table (KPDT)
};

int main() {
    // Open all necessary files for reading and writing
    ifstream irb("intermediate.txt");  // Intermediate code (IR) input
    if (!irb.is_open()) {
        cerr << "Error opening intermediate.txt" << endl;
        return 1;  // Exit if file cannot be opened
    }

    ifstream mdtb("mdt.txt");  // Macro Definition Table input
    if (!mdtb.is_open()) {
        cerr << "Error opening mdt.txt" << endl;
        return 1;
    }

    ifstream kpdtb("kpdt.txt");  // Keyword Parameter Definition Table input
    if (!kpdtb.is_open()) {
        cerr << "Error opening kpdt.txt" << endl;
        return 1;
    }

    ifstream mntb("mnt.txt");  // Macro Name Table input
    if (!mntb.is_open()) {
        cerr << "Error opening mnt.txt" << endl;
        return 1;
    }

    ofstream fr("pass2.txt");  // Output file for processed results
    if (!fr.is_open()) {
        cerr << "Error opening pass2.txt" << endl;
        return 1;
    }

    // Unordered maps to store the various tables
    unordered_map<string, MNTEntry> mnt;  // MNT map storing macro details
    unordered_map<int, string> aptab;     // Parameter Table (APT) storing parameter names
    unordered_map<string, int> aptabInverse;  // Reverse lookup for aptab, to map parameter names to positions

    // Vectors to store the contents of MDT and KPDT
    vector<string> mdt;
    vector<string> kpdt;

    string line;

    // Read and store MDT (Macro Definition Table) into a vector
    while (getline(mdtb, line)) {
        mdt.push_back(line);
    }

    // Read and store KPDT (Keyword Parameter Definition Table) into a vector
    while (getline(kpdtb, line)) {
        kpdt.push_back(line);
    }

    // Read MNT file and populate the MNT unordered map
    while (getline(mntb, line)) {
        istringstream iss(line);
        string parts[5];
        for (int i = 0; i < 5; ++i) {
            iss >> parts[i];  // Split the line into parts
        }
        // Store the MNT entry using the macro name as the key
        mnt[parts[0]] = MNTEntry(parts[0], stoi(parts[1]), stoi(parts[2]), stoi(parts[3]), stoi(parts[4]));
    }

    // Process the IRB (Intermediate Code) file line by line
    while (getline(irb, line)) {
        istringstream iss(line);
        vector<string> parts;
        string part;
        
        // Split the line into words/parts
        while (iss >> part) {
            parts.push_back(part);
        }

        // Check if the current line contains a macro invocation (from MNT)
        if (mnt.find(parts[0]) != mnt.end()) {
            const MNTEntry& entry = mnt[parts[0]];  // Retrieve the macro entry
            int pp = entry.getPp();  // Get the number of positional parameters
            int kp = entry.getKp();  // Get the number of keyword parameters
            int mdtp = entry.getMdtp();  // Get the MDT pointer
            int kpdtp = entry.getKpdtp();  // Get the KPDT pointer
            int paramNo = 1;  // Parameter number initialization

            // Process Positional Parameters (PP)
            for (int i = 0; i < pp; ++i) {
                if (paramNo < parts.size()) {
                    parts[paramNo] = parts[paramNo].substr(0, parts[paramNo].find(','));  // Clean parameter values
                    aptab[paramNo] = parts[paramNo];  // Add to APT
                    aptabInverse[parts[paramNo]] = paramNo;  // Inverse lookup for APT
                    paramNo++;
                }
            }

            // Process Keyword Parameters (KP)
            int j = kpdtp - 1;  // Start from the KPDT pointer
            for (int i = 0; i < kp; ++i) {
                if (j >= 0 && j < kpdt.size()) {
                    istringstream kpss(kpdt[j]);  // Read KPDT entry
                    vector<string> kpParts;
                    string kpPart;
                    while (getline(kpss, kpPart, '\t')) {
                        kpParts.push_back(kpPart);  // Split by tab
                    }
                    if (kpParts.size() > 1) {
                        aptab[paramNo] = kpParts[1];  // Store the keyword value in APT
                        aptabInverse[kpParts[0]] = paramNo;  // Update the inverse lookup
                        j++;
                        paramNo++;
                    }
                }
            }

            // Process remaining parameters (after positional and keyword parameters)
            for (int i = pp + 1; i < parts.size(); ++i) {
                parts[i] = parts[i].substr(0, parts[i].find(','));
                size_t eqPos = parts[i].find('=');
                if (eqPos != string::npos) {
                    string name = parts[i].substr(0, eqPos);
                    string value = parts[i].substr(eqPos + 1);
                    // Filter out '&' characters from parameter names
                    string filteredName;
                    for (char c : name) {
                        if (c != '&') {
                            filteredName += c;  // Remove '&' from name
                        }
                    }
                    name = filteredName;
                    if (aptabInverse.find(name) != aptabInverse.end()) {
                        aptab[aptabInverse[name]] = value;  // Update the APT with the value
                    }
                }
            }

            // Process the MDT (Macro Definition Table) and replace parameters
            int i = mdtp - 1;  // Start from the MDT pointer
            while (i < mdt.size() && mdt[i] != "MEND") {
                istringstream mdiss(mdt[i]);
                vector<string> splits;
                string split;
                while (mdiss >> split) {
                    splits.push_back(split);  // Split the MDT line into parts
                }

                fr << "+";  // Start the line with "+"
                for (const string& s : splits) {
                    if (s.find("(P,") != string::npos) {  // Check for positional parameter (P)
                        string numStr;
                        for (char c : s) {
                            if (isdigit(c)) numStr += c;  // Extract the number from "(P,number)"
                        }
                        int num = stoi(numStr);
                        if (aptab.find(num) != aptab.end()) {
                            fr << aptab[num] << "\t";  // Replace with parameter value from APT
                        }
                    } else {
                        fr << s << "\t";  // Non-parameter part, just write it
                    }
                }
                fr << "\n";  // End the line in the output file
                i++;
            }

            // Clear the APT for the next macro invocation
            aptab.clear();
            aptabInverse.clear();
        } else {
            // If the line doesn't represent a macro invocation, write it directly to the output
            fr << line << "\n";
        }
    }

    // Close all the open files
    fr.close();
    mntb.close();
    mdtb.close();
    kpdtb.close();
    irb.close();

    return 0;  // Exit the program successfully
}
