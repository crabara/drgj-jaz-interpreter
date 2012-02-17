#include "Reader.h"
#include "Executer.h"

int main(int argc, char** argv) {
    Reader readFile;
    Executer runInterpreter;
    string fileName("");
    string outFileName("");
    ifstream is;
    ofstream os;
    if (argc > 1) {
        fileName = argv[1];
        if (!fileName.compare(fileName.length() - 4, 4, ".jaz")) {
            outFileName = fileName;
            outFileName.replace(outFileName.length() - 4, 4, ".out");
        } else {
            cout << "ERR: " << fileName << " is not a JAZ formatted file.\n"
                 << "JAZ: Halting execution.\n\n"
                 << "Press ENTER to exit.";
            getchar();
            return EXIT_FAILURE;
        }
    } else {
        cout << "ERR: No file specified for execution.\n"
             << "JAZ: Halting execution.\n\n"
             << "Press ENTER to exit.";
        getchar();
        return EXIT_FAILURE;
    }
    is.open(fileName.c_str());
    if (is.is_open()) {
        runInterpreter.setInstructionList(readFile.buildList(is));
        is.close();
        runInterpreter.setLabelMap(
            readFile.mapLabels(
                runInterpreter.getCurrentInstruction()));
    } else {
        cout << "ERR: Cannot open file: " << fileName << endl
             << "JAZ: Halting execution.\n\n"
             << "Press ENTER to exit.";
            getchar();
            return EXIT_FAILURE;
    }
    os.open(outFileName.c_str());
    if (os.is_open()) {
        runInterpreter.execute(os);
        os.close();
        cout << "JAZ: Output written to file: " << outFileName
             << endl << endl;
    } else {
        cout << "ERR: Output file could not be opened: " << outFileName << endl;
        cout << "JAZ: Output will only be displayed in the shell window.\n\n";
        runInterpreter.execute(os, false);
    }
    cout << "Press ENTER to exit.";
    getchar();
    return 0;
}
