#ifndef KNOCK_UTIL_HASH_HPP
#define KNOCK_UTIL_HASH_HPP

#include <Config/Config.hpp>

#include <vector>

#include <boost/filesystem.hpp>

namespace Knock {

namespace Hash { // Knock::Hash

constexpr size_t Sha256DigestLength = 32;

std::string fileSha256( const boost::filesystem::path& filePath );
std::string bufferSha256( const std::vector<unsigned char>& buf );
std::string bufferSha256( const void* pBuf, size_t bufSize );

} // Knock::Hash

} // Knock

#endif // End of include guard