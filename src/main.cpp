#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <vector>

typedef std::multimap<std::string, std::string> wordmap;
typedef std::vector<std::string> wordlist;

wordmap build_anagrams(std::istream& stream) {

	std::string word, anagram;
	wordmap anagrams;

	while (stream >> word) {
		anagram = word;
		sort(anagram.begin(), anagram.end());
		anagrams.insert(wordmap::value_type(anagram, word));
	}

	return anagrams;
}

wordlist get_combinations(const std::string& chars) {

	const int limit = 1 << chars.size();

	// Adapted from http://applied-math.org/subset.pdf
	// Efficiently Enumerating the Subsets of a Set (2.1 Lexicographic Ordering)

	std::set<std::string> anagrams;

	for (int comb = 1; comb < limit; ++comb) {
		std::string word;
		for (size_t c = 0; c < chars.size(); ++c) {
			if (comb & (1 << c)) {
				word.push_back(chars[c]);
			}
		}
		anagrams.insert(word);
	}

	return wordlist(anagrams.begin(), anagrams.end());
}

wordlist find_words(const wordmap& map, const std::string& chars) {

	const wordlist anagrams = get_combinations(chars);
	wordlist words;
	std::pair<wordmap::const_iterator, wordmap::const_iterator> range;

	for (wordlist::const_iterator it = anagrams.begin(); it != anagrams.end(); ++it) {
		range = map.equal_range(*it);
		for (wordmap::const_iterator word = range.first; word != range.second; ++word) {
			words.push_back(word->second);
		}
	}

	return words;
}

int main() {

	std::string chars;

	if (!(std::cin >> chars) || chars.size() > 8) {
		std::cerr << "Invalid Characters: " << chars << std::endl;
		return 1;
	}

	sort(chars.begin(), chars.end());
	transform(chars.begin(), chars.end(), chars.begin(), toupper);

	std::ifstream list("resources/list");
	const wordlist words = find_words(build_anagrams(list), chars);
	copy(words.begin(), words.end(), std::ostream_iterator<std::string>(std::cout, "\n"));

	return 0;
}
