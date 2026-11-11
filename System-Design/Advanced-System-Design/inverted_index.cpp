/**
 * Inverted Index
 *
 * An inverted index maps a term to the documents that contain it, plus the
 * positions and frequencies inside each document. It is the core data structure
 * behind full-text search.
 *
 * Posting list for a term:
 *   document id -> { frequency, sorted positions }
 *
 * Query types:
 *   - AND: documents containing every term
 *   - OR: documents containing at least one term
 *   - phrase: documents where the terms appear consecutively, in order
 *   - ranked search: TF-IDF score, highest first
 *
 * Tokenization:
 *   - lowercase the text
 *   - split on non-alphanumeric characters
 *   - keep positions so phrase queries can check adjacency
 *
 * TF-IDF:
 *   tf  = number of occurrences of the term in the document
 *   idf = log((N + 1) / (df + 1)) + 1, where df is the number of documents
 *         containing the term and N is the total number of documents
 *   score = sum over query terms of tf * idf
 *
 * The +1 smoothing avoids a zero idf for a term that appears in every
 * document. A real search engine uses BM25, field boosts, stemming, stop words,
 * compression, and distributed shards.
 */

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;

struct Posting {
    int frequency = 0;
    vector<int> positions;
};

class InvertedIndex {
    unordered_map<string, map<int, Posting>> index;  // term -> doc -> posting
    vector<string> documents;

   public:
    static vector<string> tokenize(const string &text) {
        vector<string> tokens;
        string current;

        for (unsigned char character : text) {
            if (isalnum(character)) {
                current += static_cast<char>(tolower(character));
            } else if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
        }

        if (!current.empty()) {
            tokens.push_back(current);
        }

        return tokens;
    }

    int addDocument(const string &text) {
        int documentId = documents.size();
        documents.push_back(text);

        vector<string> tokens = tokenize(text);

        for (int position = 0; position < tokens.size(); position++) {
            Posting &posting = index[tokens[position]][documentId];
            posting.frequency++;
            posting.positions.push_back(position);
        }

        return documentId;
    }

    vector<int> queryAnd(const vector<string> &terms) const {
        if (terms.empty()) {
            return {};
        }

        auto first = index.find(terms[0]);
        if (first == index.end()) {
            return {};
        }

        vector<int> result;

        for (const auto &[documentId, posting] : first->second) {
            bool present = true;

            for (int i = 1; i < terms.size(); i++) {
                auto termEntry = index.find(terms[i]);

                if (termEntry == index.end() ||
                    termEntry->second.find(documentId) ==
                        termEntry->second.end()) {
                    present = false;
                    break;
                }
            }

            if (present) {
                result.push_back(documentId);
            }
        }

        return result;
    }

    vector<int> queryOr(const vector<string> &terms) const {
        set<int> documentsFound;

        for (const string &term : terms) {
            auto termEntry = index.find(term);
            if (termEntry == index.end()) {
                continue;
            }

            for (const auto &[documentId, posting] : termEntry->second) {
                documentsFound.insert(documentId);
            }
        }

        return vector<int>(documentsFound.begin(), documentsFound.end());
    }

    vector<int> phrase(const vector<string> &terms) const {
        if (terms.empty()) {
            return {};
        }

        vector<int> candidates = queryAnd(terms);
        vector<int> result;

        for (int documentId : candidates) {
            const vector<int> &firstPositions =
                index.at(terms[0]).at(documentId).positions;

            for (int start : firstPositions) {
                bool matches = true;

                for (int i = 1; i < terms.size(); i++) {
                    const vector<int> &positions =
                        index.at(terms[i]).at(documentId).positions;

                    if (!binary_search(positions.begin(), positions.end(),
                                       start + i)) {
                        matches = false;
                        break;
                    }
                }

                if (matches) {
                    result.push_back(documentId);
                    break;
                }
            }
        }

        return result;
    }

    vector<pair<int, double>> search(const vector<string> &terms,
                                     int limit) const {
        if (terms.empty() || limit <= 0) {
            return {};
        }

        set<string> uniqueTerms(terms.begin(), terms.end());
        vector<pair<int, double>> scores;

        for (int documentId = 0; documentId < documents.size(); documentId++) {
            double score = 0;

            for (const string &term : uniqueTerms) {
                auto termEntry = index.find(term);
                if (termEntry == index.end()) {
                    continue;
                }

                auto documentEntry = termEntry->second.find(documentId);
                if (documentEntry == termEntry->second.end()) {
                    continue;
                }

                double termFrequency = documentEntry->second.frequency;
                double documentFrequency = termEntry->second.size();
                double inverseDocumentFrequency =
                    log((documents.size() + 1.0) / (documentFrequency + 1.0)) +
                    1.0;

                score += termFrequency * inverseDocumentFrequency;
            }

            if (score > 0) {
                scores.push_back({documentId, score});
            }
        }

        sort(scores.begin(), scores.end(),
             [](const pair<int, double> &a, const pair<int, double> &b) {
                 if (a.second != b.second) {
                     return a.second > b.second;
                 }
                 return a.first < b.first;
             });

        if (scores.size() > limit) {
            scores.resize(limit);
        }

        return scores;
    }

    const string &document(int id) const {
        return documents[id];
    }
};

void printIds(const vector<int> &ids) {
    cout << "[";

    for (int i = 0; i < ids.size(); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << ids[i];
    }

    cout << "]" << endl;
}

int main() {
    InvertedIndex index;

    index.addDocument("The quick brown fox");
    index.addDocument("The quick fox jumps");
    index.addDocument("Brown fox sleeps");
    index.addDocument("quick quick fox");

    // AND: documents that contain both terms.
    printIds(index.queryAnd({"quick", "fox"}));  // [0, 1, 3]

    // OR: documents that contain at least one term.
    printIds(index.queryOr({"brown", "sleeps"}));  // [0, 2]

    // Phrase: the terms must be adjacent and in order.
    printIds(index.phrase({"quick", "brown"}));  // [0]
    printIds(index.phrase({"quick", "fox"}));    // [1, 3]
    printIds(index.phrase({"quick", "quick"}));  // [3]
    printIds(index.phrase({"brown", "quick"}));  // []

    // Case and punctuation are normalized by the tokenizer.
    printIds(index.queryAnd(InvertedIndex::tokenize("The QUICK, fox!")));
    // [0, 1]  ("the" only appears in documents 0 and 1)

    // Unknown terms return nothing.
    printIds(index.queryAnd({"unknown"}));  // []
    printIds(index.queryOr({"unknown"}));   // []

    // Empty query.
    printIds(index.queryAnd({}));  // []
    printIds(index.phrase({}));    // []

    // TF-IDF ranking: document 3 has "quick" twice, so it ranks first.
    vector<pair<int, double>> ranked = index.search({"quick", "fox"}, 3);

    cout << ranked.size() << endl;  // 3

    for (const auto &[documentId, score] : ranked) {
        cout << documentId << " " << score << endl;
    }
    // 3 first, then 0 and 1 (ties broken by document id)

    // A limit larger than the result set is fine.
    cout << index.search({"quick", "fox"}, 100).size() << endl;  // 3

    // A zero or negative limit returns nothing.
    cout << index.search({"quick", "fox"}, 0).size() << endl;  // 0

    return 0;
}

/**
 * Time complexity:
 *   addDocument -> O(tokens * log documents) for the map insertions
 *   queryAnd -> O(df(first) * number of terms * log df) for the membership
 *   checks
 *   queryOr -> O(sum of df over terms * log documents)
 *   phrase -> O(candidates * occurrences * terms * log positions)
 *   search -> O(documents * terms * log documents + results log results)
 * Space complexity -> O(total tokens) for the postings and positions
 *
 * Special cases:
 *   - empty query -> empty result
 *   - unknown term -> empty result
 *   - punctuation and mixed case -> normalized by tokenize
 *   - repeated query terms -> search deduplicates them with a set, so a term
 *     is not counted twice
 *   - phrase with one term -> equivalent to an AND query for that term
 *   - phrase with repeated terms -> positions must be consecutive, so
 *     "quick quick" only matches a document with two adjacent "quick" tokens
 *   - positions are sorted because tokens are appended left to right, which
 *     makes binary_search valid
 *   - a document id is assigned in insertion order and never reused
 *
 * The postings are stored in a std::map so document ids are sorted. A real
 * engine stores them in a compressed array with skip pointers, which makes
 * intersection and union much faster and more cache-friendly.
 *
 * Ranking is deliberately simple. TF-IDF rewards frequent terms and rare
 * terms. BM25 adds document-length normalization and saturation, which avoids
 * letting a single repeated term dominate the score.
 */
