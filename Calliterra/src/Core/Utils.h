#pragma once

template<class Iter>
void SplitStringIter(const std::string& str, const std::string& delim, Iter out)
{
	if (delim.empty())
	{
		*out++ = str;
	}
	else
	{
		size_t a = 0, b = str.find(delim);

		while (b != std::string::npos)
		{
			*out++ = std::move(str.substr(a, b - a));

			a = b + delim.length();
			b = str.find(delim, a);
		}
		*out++ = std::move(str.substr(a, str.length() - a));
	}
}

inline std::vector<std::string> SplitString(const std::string& str, const std::string& delim)
{
	std::vector<std::string> strings;
	SplitStringIter(str, delim, std::back_inserter(strings));
	return strings;
}
