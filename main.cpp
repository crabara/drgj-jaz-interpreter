#include "Reader.h"
#include "Labeller.h"
#include "Executer.h"

int main(int argc, char** argv) {
    //filenames for the in file and out file.
    string fileName("");
    string outFileName("");
    
    //streams for handling input and output for files.
    ifstream is;
    ofstream os;
    
    //listHead will point to the front of the instruction list once the reader
    //is invoked.
    InstructionItem* listHead = NULL;
    
    //labelList will contain the positions of all the labels in the instruction
    //list once the labeller is invoked.
    map<string, InstructionItem*> labelList;
    
    //the executer handles all processing on the jaz file.
    Executer runFile;
    
    //check to see that a filename has been provided on the command line.
    if (argc > 1) {
        fileName = argv[1];
        
        //make sure the file supplied is formatted as "*.jaz".
        if (!fileName.compare(fileName.length() - 4, 4, ".jaz")) {
            //parse the filename so that the correct "*.out" is written.
            outFileName = fileName;
            outFileName.replace(outFileName.length() - 4, 4, ".out");
            
        } else {
            //file is not formatted correctly.
            cout << "ERR: " << fileName << " is not a JAZ formatted file.\n"
                 << "JAZ: Halting execution.\n\n"
                 << "Press ENTER to exit.";
            getchar();
            return EXIT_FAILURE;
        }
        
    } else {
        //user did not specify any file for execution.
        cout << "ERR: No file specified for execution.\n"
             << "JAZ: Halting execution.\n\n"
             << "Press ENTER to exit.";
        getchar();
        return EXIT_FAILURE;
    }
    
    //open the jaz file for parsing.
    is.open(fileName.c_str());
    
    if (is.is_open()) {
        //build the instruction list and get a pointer to the head of it.
        Reader readFile;
        listHead = readFile.buildList(is);
        is.close();
        
        //search the instruction list for labels
        Labeller labelFile;
        labelList = labelFile.mapLabels(listHead);
        
        //place the newly built instruction list and label locations in the
        //executer.
        runFile.setInstructionList(listHead);
        runFile.setLabelMap(labelList);
        
    } else {
        //the specified file cannot be opened or read.
        cout << "ERR: Cannot open file: " << fileName << endl
             << "JAZ: Halting execution.\n\n"
             << "Press ENTER to exit.";
            getchar();
            return EXIT_FAILURE;
    }
    
    //open the out file for writing.
    os.open(outFileName.c_str());
    if (os.is_open()) {
        //execute the instruction list.
        runFile.execute(os);
        os.close();
        cout << "JAZ: Output written to file: " << outFileName << endl << endl;
        
    } else {
        //cannot open the out file for some reason
        //write output only to the command window.
        cout << "ERR: Output file could not be opened: " << outFileName << endl;
        cout << "JAZ: Output will only be displayed in the shell window.\n\n";
        runFile.execute(os, false);
    }
    
    cout << "Press ENTER to exit.";
    getchar();
    return 0;
}
