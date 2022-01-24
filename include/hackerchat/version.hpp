#ifndef HACKERCHAT_VERSION_HPP
#define HACKERCHAT_VERSION_HPP

#define VMAJOR 0
#define VMINOR 1
#define VPATCH 0

#define VERSION \
  (VMAJOR * 1000 + VMINOR * 100 + VPATCH)

#include <string>

namespace hackerchat
{
	[[maybe_unused]] inline const std::string get_version()
	{
		std::string version{std::to_string(VMAJOR)};
		version.append(".");
		version.append(std::to_string(VMINOR));
		version.append(".");
		version.append(std::to_string(VPATCH));

		return version;
	}
}

#endif
