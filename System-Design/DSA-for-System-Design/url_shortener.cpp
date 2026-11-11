/**
 * URL Shortener
 *
 * Design a service that maps a long URL to a short code and back.
 *
 * Brute force:
 *   - store every URL in a vector
 *   - the short code is the base62 encoding of its index
 *   - expand decodes the index
 *   - O(1) time, but codes are sequential and predictable
 *
 * Optimized:
 *   - hash the URL (FNV-1a 64-bit)
 *   - base62-encode the hash
 *   - store both code -> URL and URL -> code
 *   - on a hash collision, add a salt and try again
 *   - returning the same code for the same URL makes the operation idempotent
 *
 * Production systems also add custom aliases, expiration (TTL), analytics,
 * rate limiting, and a distributed ID generator (Snowflake, ZooKeeper, or a
 * database sequence) instead of a single-process counter.
 */

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

const string BASE62 =
    "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

string toBase62(uint64_t value) {
    if (value == 0) {
        return "0";
    }

    string result;

    while (value > 0) {
        result += BASE62[value % 62];
        value /= 62;
    }

    reverse(result.begin(), result.end());

    return result;
}

optional<uint64_t> fromBase62(const string &code) {
    if (code.empty()) {
        return nullopt;
    }
    uint64_t value = 0;

    for (char ch : code) {
        size_t index = BASE62.find(ch);

        if (index == string::npos) {
            return nullopt;
        }
        if (value > (numeric_limits<uint64_t>::max() - index) / 62) {
            return nullopt;
        }

        value = value * 62 + index;
    }

    return value;
}

// Brute force: codes are vector indices.
class BruteShortener {
    vector<string> urls;

   public:
    string shorten(const string &url) {
        urls.push_back(url);
        return toBase62(urls.size() - 1);
    }

    string expand(const string &code) {
        auto index = fromBase62(code);

        if (!index || *index >= urls.size()) {
            return "";
        }

        return urls[*index];
    }
};

class URLShortener {
    unordered_map<string, string> codeToUrl;
    unordered_map<string, string> urlToCode;

    uint64_t hashValue(const string &url) {
        uint64_t hash = 14695981039346656037ULL;

        for (unsigned char ch : url) {
            hash ^= ch;
            hash *= 1099511628211ULL;
        }

        return hash;
    }

   public:
    string shorten(const string &url) {
        auto existing = urlToCode.find(url);

        if (existing != urlToCode.end()) {
            return existing->second;
        }

        uint64_t salt = 0;

        while (true) {
            string candidate = toBase62(hashValue(url + "#" + to_string(salt)));

            if (codeToUrl.find(candidate) == codeToUrl.end()) {
                codeToUrl[candidate] = url;
                urlToCode[url] = candidate;
                return candidate;
            }

            salt++;
        }
    }

    string expand(const string &code) {
        auto found = codeToUrl.find(code);

        if (found == codeToUrl.end()) {
            return "";
        }

        return found->second;
    }

    int size() {
        return codeToUrl.size();
    }
};

int main() {
    BruteShortener brute;
    string bruteCode = brute.shorten("https://example.com/articles/123");
    cout << bruteCode << endl;  // 0
    cout << brute.expand(bruteCode) << endl;

    URLShortener shortener;

    string first = "https://example.com/articles/123";
    string second = "https://example.com/articles/456";

    string code1 = shortener.shorten(first);
    string code2 = shortener.shorten(second);
    string code1Again = shortener.shorten(first);

    cout << code1 << endl;
    cout << code2 << endl;
    cout << (code1 == code1Again) << endl;  // 1 (idempotent)

    cout << shortener.expand(code1) << endl;  // original URL
    cout << shortener.expand(code2) << endl;
    cout << shortener.expand("missing") << endl;  // empty

    cout << shortener.size() << endl;  // 2

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(1) shorten/expand, O(n) memory for all URLs
 *   hash -> O(1) average shorten/expand, O(n) memory
 * Space complexity -> O(n) for the two maps
 *
 * Special cases: the same URL shortened twice (same code), an unknown code
 * (empty result), a hash collision (salt and retry), an empty URL, very long
 * URLs, custom aliases (check availability), expired codes (TTL), deleted
 * links, and distributed ID generation.
 *
 * Codes grow as log62(id); a 64-bit hash gives up to 11 base62 characters.
 */
