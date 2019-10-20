#include <iostream>

#include <cstdlib>
#include <cstring>

#include <boost/program_options.hpp>

#include <Task/Task.hpp>
#include <Util/Hash.hpp>

namespace po = boost::program_options;

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
            ( HashFileCmd,
              po::value<std::string>(),
              "Show hash digest of specified file. [sha256]" );

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
            { Knock::HashFileCmd, &onHashFile }
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