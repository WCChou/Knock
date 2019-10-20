#include <Config/Config.hpp>

#include <Task/Task.hpp>

namespace Knock {

bool isValidOptionName( const char* pOption )
{
    return nullptr != pOption && '\0' != *pOption;
}

/*!
 * \brief Get config file from --config option. If no --config, returns config.yaml.
 */
boost::filesystem::path CmdLineContext::getConfigFilePath()
{
    return getOptionValue<std::string>( ConfigCmd, "config.yaml" );
}

} // Knock