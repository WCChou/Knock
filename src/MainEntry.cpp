#include <iostream>

#include <cstdlib>
#include <cstring>

#include <boost/program_options.hpp>

#include <Task/Task.hpp>
#include <Task/FileGenerator.hpp>
#include <Util/Hash.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

Knock::TaskResult onHashFile( Knock::CmdLineContext& cmdLine )
{
    static const char* funcName( "onHashFile" );

    if ( !cmdLine.has( Knock::HashFileCmd ) ) {
        return Knock::TaskResult::Continue;
    }

    if ( !cmdLine.has( Knock::InputCmd ) ) {
        std::cout << "[Knock] please specify input via --input=xxx\n";
        return Knock::TaskResult::Failed;
    }

    std::string digest = Knock::Hash::fileSha256(
        cmdLine.getOptionValue<std::string>( Knock::InputCmd, "" ) );
    std::cout << "SHA256: " << digest << std::endl;

    return Knock::TaskResult::Done;
}
/*!
 * \par Usage
 *      --generate.file --size=10mb --count=10 [--output=output]
 */
Knock::TaskResult onGenerateFile( Knock::CmdLineContext& cmdLine )
{
    static const char* funcName( "onGenerateFile" );

    using namespace std;
    using namespace Knock;

    if ( !cmdLine.has( Knock::GenerateFileCmd ) ) {
        return Knock::TaskResult::Continue;
    }

    size_t fileSize = 0;
    if ( !cmdLine.getFileSize( fileSize ) ) {
        cout << "[" << Knock::GenerateFileCmd
             << "] usage: --generate.file --size=10mb --count10 [--output=output]"
             << endl;
        return Knock::TaskResult::Failed;
    }

    fs::path outDir = cmdLine.getOutputPath();
    Knock::FileGenerator generator;
    generator.generateFiles(
        outDir,
        cmdLine.getOptionValue( CountCmd, 1 ),
        fileSize );

    return Knock::TaskResult::Done;
}

Knock::TaskResult onListProcess( Knock::CmdLineContext& cmdLine )
{
    using namespace std;
    using namespace Knock;

    if ( !cmdLine.has( Knock::ListProcessCmd ) ) {
        return Knock::TaskResult::Continue;
    }

    for ( const fs::directory_entry& entry : fs::directory_iterator( "/proc" ) ) {
        if ( !fs::is_directory( entry.status() ) ) {
            continue;
        }

        if ( !::isdigit( entry.path().leaf().string().c_str()[ 0 ] ) ) {
            continue;
        }

        ifstream in( (entry.path() / "stat").string(), ios::in );
        if ( !in ) {
            continue;
        }

        pid_t pid;
        in >> pid;

        string name;
        in >> name;
        // assume ')' is not a valid process name
        if ( ')' != *name.rbegin() ) {
            string moreName;
            in >> moreName;
            name += " ";
            name += moreName;
        }
        name = name.substr( 1, name.length() - 2 );

        //cout << "name: " << name << endl;
        in.close();

        in.open( (entry.path() / "status").string(), ios::in );
        if ( !in ) {
            continue;
        }

        string line;
        while ( std::getline( in, line ) ) {
            istringstream parser( line );
            string field;
            parser >> field;
            if ( "Seccomp:" != field ) {
                continue;
            }

            int seccompMode = 0;
            parser >> seccompMode;
            if ( 0 != seccompMode )
                cout << "pid: " << pid << ", name: " << name << ", seccomp: " << seccompMode << endl;
        }
    }

    return Knock::TaskResult::Done;
}

int main( int argc, char* argv[] )
{
    using namespace std;
    using namespace Knock;

    try {
        po::options_description desc( "Allowed options" );
        desc.add_options()
            ( HelpCmd,
              "Show help message" )
            ( InputCmd,
              po::value<std::string>(),
              "Used with other commands to specify the input" )
            ( OutputCmd,
              po::value<std::string>(),
              "Used with other command to specify the output file/directory/..." )
            ( CountCmd,
              po::value<int>()->default_value( 1 ),
              "Used with other commands to specify count" )
            ( SizeCmd,
              po::value<std::string>(),
              "Used with other commands to specify size" )
            ( NameCmd,
              po::value<std::string>(),
              "Used with other commands to specify name attribute" )
            ( SandboxCmd,
              "Used with other commands to specify sandbox attribute" )
            ( SeccompCmd,
              "Used with other commands to specify seccomp mode" )

            ( GenerateFileCmd,
              "Generate file(s) with random content. e.g. --generate.file --count=10 --size=10MB" )
            ( ScanFilesCmd,
              po::value<std::string>(),
              "Scan files that in the specified directory. [dir]" )
            ( HashFileCmd,
              po::value<std::string>(),
              "Show hash digest of specified file. [sha256]" )
            ( ListProcessCmd,
              "List Processes" );

        if ( 1 == argc ) {
            cout << desc << endl;
            return 0;
        }

        po::variables_map vm;
        po::store( po::parse_command_line( argc, argv, desc), vm );
        po::notify( vm );

        if ( vm.count( HelpCmd ) ) {
            cout << desc << endl;
            return 0;
        }

        struct CmdDispatchTable {
            const char* cmd;
            CmdHandler  handler;
        } table[] = {
            { Knock::GenerateFileCmd, &onGenerateFile },
            { Knock::HashFileCmd, &onHashFile },
            { Knock::ListProcessCmd, &onListProcess }
        };

        Knock::CmdLineContext   cmdLine( vm );
        for ( auto& item : table ) {
            if ( !vm.count( item.cmd ) ) {
                continue;
            }

            if ( Knock::TaskResult::Continue == item.handler( cmdLine ) ) {
                break;
            }
        }
    }
    catch ( std::exception& ex ) {
        cout << "Exception: " << ex.what() << endl;;
    }

    return 0;
}