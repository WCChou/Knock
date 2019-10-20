#ifndef KNOCK_PROGRAM_CONFIG_CONFIG_HPP
#define KNOCK_PROGRAM_CONFIG_CONFIG_HPP

#include <Config/Config.hpp>

#include <string>
#include <vector>

#include <boost/filesystem.hpp>

namespace YAML {
    class Node;
}

namespace Knock {

class Config {
public:
    bool init( const boost::filesystem::path& filePath );
};

}

#endif // End of namespace