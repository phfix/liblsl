#include "cast.h"
#include <locale>
#include <sstream>

template <> std::string lsl::to_string(double src) {
	std::ostringstream os;
	os.imbue(std::locale::classic());
	os.precision(12);
	os << src;
	return os.str();
}

template <> std::string lsl::to_string(float src) {
	return lsl::to_string(static_cast<double>(src));
}

template <> double lsl::from_string(const std::string &str) {
	std::istringstream is(str);
	is.imbue(std::locale::classic());
	double res;
	is >> res;
	return res;
}

template <> float lsl::from_string(const std::string &src) {
	return static_cast<float>(lsl::from_string<double>(src));
}