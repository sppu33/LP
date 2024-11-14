#include<iostream>
#include<cstring>
#include<string>
#include<sstream>
#include<fstream>

using namespace std;

struct OPtab{
    string opcode;
    string mclass;
    string mnemonic;
};

struct OPtab optab[18] = {
    {"STOP", "IS", "00"},
    {"ADD", "IS", "01"},
    {"SUB", "IS", "02"},
    {"MULT", "IS", "03"},
    {"MOVER", "IS","04"},
    {"MOVEM", "IS","05"},
    {"COMP", "IS", "06"},
    {"BC", "IS", "07"},
    {"DIV", "IS", "08"},
    {"READ", "IS", "09"},
    {"PRINT", "IS", "10"},
    {"START", "AD", "01"},
    {"END", "AD", "02"},
    {"ORIGIN", "AD", "03"},
    {"EQU", "AD", "04"},
    {"LTORG", "AD", "05"},
    {"DC", "DL", "01"},
    {"DS", "DL", "02"}
};

int getOP (string s)
{
    for(int i=0; i<18; i++)
    {
        if(optab[i].opcode == s)
        {
            return i;
        }
    }
    return -1;
}

int getRegId(string s)
{
    if(s == "AREG")
    {
        return 1;
    }
    else if(s == "BREG")
    {
        return 2;
    }
    else if(s == "CREG")
    {
        return 3;
    }
    else if(s == "DREG")
    {
        return 4;
    }
    else
    {
        return -1;
    }
}

int getConditionCodes(string s)
{
    if(s == "LT")
    {
        return 1;
    }
    else if(s == "LE")
    {
        return 2;
    }
    else if(s == "EQ")
    {
        return 3;
    }
    else if(s == "GT")
    {
        return 4;
    }
    else if(s == "GE")
    {
        return 5;
    }
    else if(s == "ANY")
    {
        return 6;
    }
    else
    {
        return -1;
    }
}

struct symtab{
    int no;
    string sname;
    string addr;
};

struct symtab ST[10];   // array of type symtab

bool ispresentST(string s)  
{
    for(int i=0; i<10; i++)
    {
        if(ST[i].sname == s)
        {
            return true;
        }
    }
    return false;
}

int getSymId(string s)
{
    for(int i=0; i<10; i++)
    {
        if(ST[i].sname == s)
        {
            return i;
        }
    }
    return -1;
}

struct littab{
    int no;
    string lname;
    string addr;
};

struct littab LT[10];

bool ispresentLT(string s)
{
    for(int i=0; i<10; i++)
    {
        if(LT[i].lname == s)
        {
            return true;
        }
    }
    return false;
}

int getLitId(string s)
{
    for(int i=0; i<10; i++)
    {
        if(LT[i].lname == s)
        {
            return i;
        }
    }
    return -1;
}

struct pooltab{
    int no;
    string lno;
};

struct pooltab PT[10];

int main()
{
    ifstream fin;
    fin.open("input.txt");
    ofstream ic, st, lt;
    const char * path1 = "ic.txt";
    const char * path2 = "st.txt";
    const char * path3 = "lt.txt";
    ic.open(path1);
    st.open(path2);
    lt.open(path3);
    string label, opcode, op1, op2;
    int scnt=0, lcnt=0, nlcnt=0, pcnt=0, LC=0;
    cout<<"----INTERMEDIATE CODE----"<<endl;
    cout<<"LABEL\tOPCODE\tOP1\tOP2\tLC\tINTERMEDIATE-CODE"<<endl;
    while(!fin.eof())
    {
        fin>>label>>opcode>>op1>>op2;
        string IC, lc;
        int id;
        id = getOP(opcode);
        IC = "("+optab[id].mclass+","+optab[id].mnemonic+") ";

        if(opcode == "START")
        {
            lc = "--";
            if(op1 != "NAN")
            {
                LC = stoi(op1);
                IC += "(C,"+op1+") NAN";
            }

        }


        if(opcode == "EQU")
        {
            lc = "--";
            IC += "NAN NAN";
            if(ispresentST(label))
            {
                ST[getSymId(label)].addr = ST[getSymId(op1)].addr;
            }
            else
            {
                ST[scnt].no = scnt+1;
                ST[scnt].sname = label;
                ST[scnt].addr = ST[getSymId(op1)].addr;
                scnt++;
            }

        }

        else if(label != "NAN")
        {
            if(ispresentST(label))
            {
                ST[getSymId(label)].addr = to_string(LC);
            }
            else
            {
                ST[scnt].no = scnt+1;
                ST[scnt].sname = label;
                ST[scnt].addr = to_string(LC);
                scnt++;
            }

        }
        if(opcode == "ORIGIN")
        {
            lc = "--";
            string token1, token2;
            char op;
            stringstream ss(op1);
            size_t found = op1.find('+');
            if(found != string::npos)
            {
                op = '+';
            }
            else
            {
                op = '-';
            }
            getline(ss, token1, op);
            getline(ss, token2, op);
            if(op == '+')
            {
                LC = stoi(ST[getSymId(token1)].addr) + stoi(token2);
                IC += "(S,0" + to_string(ST[getSymId(token1)].no) + ")+" + token2 + " NAN";
            }
            else
            {
                LC = stoi(ST[getSymId(token1)].addr) - stoi(token2);
                IC += "(S,0" + to_string(ST[getSymId(token1)].no) + ")-" + token2 + " NAN";

            }


        }


        if(opcode == "LTORG")
        {
            cout << "  " << label << "\t" << opcode << "\t" << op1 << "\t" << op2 << "\t";
			for(int i = lcnt - nlcnt; i < lcnt; ++i)
			{
				lc = to_string(LC);
				IC = "(DL,01) (C,";
				string c(1, LT[i].lname[2]);
				IC += c + ")    NAN";
				LT[i].addr = to_string(LC);
				LC++;
				if(i < lcnt - 1)
					cout << lc << "\t" << IC << "\n\t\t\t\t";
				else
					cout << lc << "\t" << IC << endl;
				ic << lc << "\t" << IC << endl;
			}
			// managing pool table in LTORG
			PT[pcnt].lno = "#" + to_string(LT[lcnt - nlcnt].no); 
            PT[pcnt].no = pcnt + 1; 
            pcnt++;
			nlcnt = 0;
			continue;

        }
        if(opcode == "END")
        {
            lc = "---";
			IC += " NAN     NAN";
			cout << "  " << label << "\t" << opcode << "\t" << op1 << "\t" << op2 << "\t" << lc << "\t" << IC << endl;
			ic << lc << "\t" << IC << endl;
			if(nlcnt)
			{
				for(int i = lcnt - nlcnt; i < lcnt; ++i)
				{
					lc = to_string(LC);
					IC = "(DL,01) (C,";
					string c(1, LT[i].lname[2]);
					IC += c + ")    NAN";
					LT[i].addr = to_string(LC);
					LC++;
					cout << "\t\t\t\t" << lc << "\t" << IC << endl;
					ic << lc << "\t" << IC << endl;
				}
				// managing pool table after END (if any literals are left)
				PT[pcnt].lno = "#" + to_string(LT[lcnt - nlcnt].no); 
                PT[pcnt].no = pcnt + 1; 
                pcnt++;
			}
			break;

        }
        if(opcode == "DC" || opcode == "DS")
        {
            lc = to_string(LC);
            if(opcode == "DS")
            {
                IC += "(C," + op1 + ") NAN";
                LC += stoi(op1);
            }
            else if(opcode == "DC")
            {
                string c(1, op1[1]);
                IC += "(C," + c + ") NAN";
                LC++;
            }

        }
        if(opcode != "START" && opcode != "EQU" && opcode != "ORIGIN" && opcode != "LTORG" && opcode != "END" && opcode != "DC" && opcode != "DS")
        {
            if(op2 == "NAN")
            {
                if(op1 == "NAN")
                {
                    lc = to_string(LC);
                    LC++;
                    IC += "NAN NAN";
                }
                else
                {
                    if(ispresentST(op1))
                    {
                        lc = to_string(LC);
                        LC++;
                        IC += "(S,0" + to_string(ST[getSymId(op1)].no) + ")";
                    }
                    else
                    {
                        ST[scnt].no = scnt+1;
                        ST[scnt].sname = op1;
                        scnt++;
                        lc = to_string(LC);
                        LC++;
                        IC += "(S,0" + to_string(ST[getSymId(op1)].no) + ")";

                    }
                }
            }
            else
            {
                if(opcode == "BC")
                {
                    IC += "(" + to_string(getConditionCodes(op1)) + ") ";
                }
                else
                {
                    IC += "(" + to_string(getRegId(op1)) + ") ";

                }
                
                if(op2[0] == '=')
                {
                    LT[lcnt].no = lcnt+1;
                    LT[lcnt].lname = op2;
                    lcnt++;
                    nlcnt++;
                    IC += "(L,0" + to_string(LT[getLitId(op2)].no) + ")";
                }
                else
                {
                    if(ispresentST(op2))
                    {
                        IC += "(S,0" + to_string(ST[getSymId(op2)].no) + ")";
                    }
                    else
                    {
                        ST[scnt].no = scnt+1;
                        ST[scnt].sname = op2;
                        scnt++;
                        IC += "(S,0" + to_string(ST[getSymId(op2)].no) + ")";
                    }
                }

                lc = to_string(LC);
                LC++;

            }



        }
        cout<<label<<"\t"<<opcode<<"\t"<<op1<<"\t"<<op2<<"\t"<<lc<<"\t"<<IC<<endl;
        ic<<lc<<"\t"<<IC<<endl;
    }

    cout<<"----------------------------------------------------------------------"<<endl;
    cout<<"Symbol table"<<endl;
    cout<<"No.\tname\taddress"<<endl;
    for(int i=0; i<scnt; i++)
    {
        cout<<ST[i].no<<"\t"<<ST[i].sname<<"\t"<<ST[i].addr<<endl;
        st<<ST[i].no<<"\t"<<ST[i].sname<<"\t"<<ST[i].addr<<endl;
    }

    cout<<"----------------------------------------------------------------------"<<endl;
    cout<<"Literal table"<<endl;
    cout<<"No.\tname\taddress"<<endl;
    for(int i=0; i<lcnt; i++)
    {
        cout<<LT[i].no<<"\t"<<LT[i].lname<<"\t"<<LT[i].addr<<endl;
        lt<<LT[i].no<<"\t"<<LT[i].lname<<"\t"<<LT[i].addr<<endl;
    }

    cout<<"----------------------------------------------------------------------"<<endl;
    cout<<"Pool table"<<endl;
    cout<<"No.\tlno"<<endl;
    for(int i=0; i<pcnt; i++)
    {
        cout<<PT[i].no<<"\t"<<PT[i].lno<<endl;
    }
}