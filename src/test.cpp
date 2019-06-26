

#include <iostream>
#include <string>
#include "ProgramOptions.hpp"
#include <fstream>

using namespace std;

int main(int argc,char **argv)
{
    putils::ProgramOptions options;
    
    options.addOption("val1","fake value 1","1.0d0");
    options.addOption("val2","fake value 2");
    options.addOption(string("val3"),string("fake value 3"));
    options.addOption("option_file","option file name");
    std::ofstream out("fake_options");
    
    out << "val1= 3.0d0\n";
    out << "val2= 4.0d0\n";
    
    out.close();
    
    cout << "before parsing \n";
    cout <<  options << "\n";
    options.parseCommandLine(argc,argv);
    if  (options.wasSet("option_file")) {
        string f = options.getValue("option_file");
        options.parseOptionFile(f);
    }
    options.parseEnvironment("PAT");
    
    cout << options << "\n";
    return EXIT_SUCCESS;   
}