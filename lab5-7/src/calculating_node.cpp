#include "calculating_node.h"

using namespace mysys;

std::vector<int> PrefixFunction(const std::string& s) {
	unsigned int n = s.size();
	std::vector<int> p(n);
	for (int i = 1; i < n; ++i) {
		p[i] = p[i - 1];
		while (p[i] > 0 and s[i] != s[p[i]]) {
			p[i] = p[p[i] - 1];
		}
		if (s[i] == s[p[i]]) {
			++p[i];
		}
	}
	return p;
}

std::vector<int> KMPWeak(const std::string& text, const std::string& pattern) {
	std::vector<int> p = PrefixFunction(pattern);
	int m = pattern.size();
	int n = text.size();
	int i = 0;
	std::vector<int> ans;
	if (m > n) {
		return ans;
	}
	while (i < n - m + 1) {
		int j = 0;
		while (j < m and pattern[j] == text[i + j]) {
			++j;
		}
		if (j == m) {
			ans.push_back(i);
		} else {
			if (j > 0 and j > p[j - 1]) {
				i = i + j - p[j - 1] - 1;
			}
		}
		++i;
	}
	return ans;
}

std::vector<int> CalculatingNode::_exec(const std::string& text, const std::string& pattern) {
    return KMPWeak(text, pattern);
}
