#include "maptel.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <cctype>
#include <cassert>
#include <unordered_set>

using std::endl;

typedef std::unordered_map<std::string, std::string> telephone_map_type;
typedef std::unique_ptr<telephone_map_type> telephone_map_pointer;
typedef std::vector<telephone_map_pointer> telephone_map_vector_type;
typedef std::vector<unsigned long> free_numbers_vector_type;

namespace {
// functions to prevent "static initialization order fiasco"
// see https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use
// because we must not define our class/structures we cant use Nifty Counter/Schwarz Counter
// see http://stackoverflow.com/a/211307 - Schwarz Counter

bool IsValidNumber(char const *tel) {
	size_t i = 0;
	while (i < TEL_NUM_MAX_LEN && tel[i])
		if (!isdigit(tel[i++]))
			return false;
	return i < TEL_NUM_MAX_LEN || !tel[i];
}

telephone_map_vector_type& GetMapsVector() {
	static telephone_map_vector_type* maps_vector = new telephone_map_vector_type();
	return *maps_vector;
}

free_numbers_vector_type& GetFreeNumbersVector() {
	static free_numbers_vector_type* free_numbers = new free_numbers_vector_type();
	return *free_numbers;
}

std::ostream& GetDebuggingStream() {
	#ifdef NDEBUG
	class NullBuffer : public std::streambuf {
	public:
	  int overflow(int c) { return c; }
	};
	static NullBuffer null_buffer;
	static std::ostream null_stream(&null_buffer);
	return null_stream;
	#else
	return std::cerr;
	#endif
}

}

unsigned long maptel_create() {
	GetDebuggingStream() << "maptel: maptel_create()" << endl;

	telephone_map_vector_type& maps_vector = GetMapsVector();
	free_numbers_vector_type& free_numbers = GetFreeNumbersVector();

	if (free_numbers.empty()) {
		free_numbers.push_back(maps_vector.size());
		maps_vector.push_back(telephone_map_pointer());
	}

	unsigned long id = free_numbers.back();
	free_numbers.pop_back();
	maps_vector[id].reset(new telephone_map_type);

	GetDebuggingStream() << "maptel: maptel_create: new map id = " << id << endl;

	return id;
}

void maptel_delete(unsigned long id) {
	GetDebuggingStream() << "maptel: maptel_delete(" << id << ")" << endl;

	telephone_map_vector_type& maps_vector = GetMapsVector();
	free_numbers_vector_type& free_numbers = GetFreeNumbersVector();

	assert(id < maps_vector.size());
	assert(maps_vector[id].get() != nullptr);
	maps_vector[id].reset();
	free_numbers.push_back(id);

	GetDebuggingStream() << "maptel: maptel_delete: map " << id << " deleted" << endl;
}

void maptel_insert(unsigned long id, char const *tel_src, char const *tel_dst) {
	GetDebuggingStream() << "maptel: maptel_insert(" << id << ", " << tel_src << ", " << tel_dst << ")" << endl;

	telephone_map_vector_type& maps_vector = GetMapsVector();

	assert(IsValidNumber(tel_dst));
	assert(IsValidNumber(tel_src));
	assert(id < maps_vector.size());
	assert(maps_vector[id].get() != nullptr);

	telephone_map_type& map = *maps_vector[id];
	map[tel_src] = tel_dst;

	GetDebuggingStream() << "maptel: maptel_insert: inserted" << endl;
}

void maptel_erase(unsigned long id, char const *tel_src) {
	GetDebuggingStream() << "maptel: maptel_erase(" << id << ", " << tel_src << ")" << endl;

	telephone_map_vector_type& maps_vector = GetMapsVector();

	assert(IsValidNumber(tel_src));
	assert(id < maps_vector.size());
	assert(maps_vector[id].get() != nullptr);

	telephone_map_type& map = *maps_vector[id];
	size_t number_of_erased = map.erase(tel_src);

	if (number_of_erased)
		GetDebuggingStream() << "maptel: maptel_erase: erased" << endl;
	else
		GetDebuggingStream() << "maptel: maptel_erase: nothing to erase" << endl;

}

void maptel_transform(unsigned long id, char const *tel_src, char *tel_dst, size_t len) {
	GetDebuggingStream() << "maptel: maptel_transform(" << id << ", " << tel_src << ", " << (void*)tel_dst << ", " << len << ")" << endl;

	telephone_map_vector_type& maps_vector = GetMapsVector();

	assert(IsValidNumber(tel_src));
	assert(id < maps_vector.size());
	assert(maps_vector[id].get() != nullptr);

	const telephone_map_type& map = *maps_vector[id];

	std::unordered_set<std::string> visited_numbers;
	std::string source = tel_src;
	telephone_map_type::const_iterator it;
	while ((it = map.find(source)) != map.end()) {
		visited_numbers.insert(source);
		source = it->second;
		if (visited_numbers.count(source) > 0) {
			source = tel_src;
			GetDebuggingStream() << "maptel: maptel_transform: cycle detected" << endl;
			break;
		}
	}


	assert(source.size() < len);
	source.copy(tel_dst, len - 1, 0);
	tel_dst[std::min(len - 1, source.size())] = '\0';

	GetDebuggingStream() << "maptel: maptel_transform: " << tel_src << " -> " << tel_dst << ", " << endl;
}
