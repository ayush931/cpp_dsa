/**
 * Merkle Tree
 *
 * A Merkle tree hashes data blocks into leaves, then hashes pairs of child
 * hashes into parent hashes until one root remains. The root commits to the
 * entire data set: changing any block changes the root.
 *
 * Why it matters:
 *   - tamper detection: a different data set almost certainly has a different
 *     root
 *   - compact proofs: a leaf can be verified with O(log n) sibling hashes
 *     instead of sending the whole data set
 *   - efficient synchronization: two replicas can compare roots, then descend
 *     only into subtrees that differ
 *
 * Proof verification:
 *   start with the hash of the block
 *   for each sibling from leaf to root:
 *     if the sibling is on the left -> hash(sibling, current)
 *     else                          -> hash(current, sibling)
 *   the final value must equal the trusted root
 *
 * This implementation uses FNV-1a as a fast demonstration hash. Production
 * systems use a cryptographic hash such as SHA-256, and they domain-separate
 * leaf hashes from internal hashes to prevent second-preimage attacks.
 *
 * Odd number of nodes: the last node is duplicated at that level. This is a
 * common convention, but it must be applied consistently by every verifier.
 */

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

const uint64_t FNV_OFFSET = 14695981039346656037ULL;
const uint64_t FNV_PRIME = 1099511628211ULL;

uint64_t hashString(const string &data) {
    uint64_t hash = FNV_OFFSET;

    for (unsigned char character : data) {
        hash ^= character;
        hash *= FNV_PRIME;
    }

    return hash;
}

uint64_t hashPair(uint64_t left, uint64_t right) {
    uint64_t hash = FNV_OFFSET;

    for (int byte = 0; byte < 8; byte++) {
        hash ^= (left >> (8 * byte)) & 0xff;
        hash *= FNV_PRIME;
    }

    for (int byte = 0; byte < 8; byte++) {
        hash ^= (right >> (8 * byte)) & 0xff;
        hash *= FNV_PRIME;
    }

    return hash;
}

struct ProofStep {
    uint64_t siblingHash;
    bool siblingIsLeft;
};

class MerkleTree {
    vector<string> blocks;
    vector<vector<uint64_t>> levels;  // levels[0] = leaf hashes

    void build() {
        levels.clear();
        if (blocks.empty()) {
            return;
        }

        vector<uint64_t> leaves;

        for (const string &block : blocks) {
            leaves.push_back(hashString(block));
        }

        levels.push_back(leaves);

        while (levels.back().size() > 1) {
            const vector<uint64_t> &current = levels.back();
            vector<uint64_t> next;

            for (int i = 0; i < current.size(); i += 2) {
                uint64_t left = current[i];
                uint64_t right =
                    (i + 1 < current.size()) ? current[i + 1] : current[i];

                next.push_back(hashPair(left, right));
            }

            levels.push_back(next);
        }
    }

   public:
    explicit MerkleTree(vector<string> data) : blocks(std::move(data)) {
        build();
    }

    uint64_t root() const {
        return levels.empty() ? 0 : levels.back()[0];
    }

    bool empty() const {
        return blocks.empty();
    }

    vector<ProofStep> proof(int index) const {
        vector<ProofStep> result;

        if (levels.empty() || index < 0 || index >= blocks.size()) {
            return result;
        }

        int current = index;

        for (int level = 0; level + 1 < levels.size(); level++) {
            int sibling = current ^ 1;

            // Odd node at this level: duplicate the last node.
            if (sibling >= levels[level].size()) {
                sibling = current;
            }

            bool siblingIsLeft = sibling < current;

            result.push_back({levels[level][sibling], siblingIsLeft});
            current /= 2;
        }

        return result;
    }

    static bool verify(const string &block, const vector<ProofStep> &proof,
                       uint64_t expectedRoot) {
        uint64_t current = hashString(block);

        for (const ProofStep &step : proof) {
            if (step.siblingIsLeft) {
                current = hashPair(step.siblingHash, current);
            } else {
                current = hashPair(current, step.siblingHash);
            }
        }

        return current == expectedRoot;
    }

    const vector<string> &data() const {
        return blocks;
    }
};

void printRoot(const string &label, uint64_t root) {
    cout << label << " 0x" << hex << setw(16) << setfill('0') << root << dec
         << setfill(' ') << endl;
}

int main() {
    vector<string> blocks = {"alpha", "beta", "gamma", "delta", "epsilon"};
    MerkleTree tree(blocks);

    printRoot("root", tree.root());

    // Every leaf proof verifies against the root.
    bool allValid = true;

    for (int i = 0; i < blocks.size(); i++) {
        if (!MerkleTree::verify(blocks[i], tree.proof(i), tree.root())) {
            allValid = false;
        }
    }

    cout << allValid << endl;  // 1

    // A tampered block fails against the original root.
    cout << MerkleTree::verify("gamma!", tree.proof(2), tree.root()) << endl;
    // 0

    // Rebuilding with the tampered block changes the root.
    vector<string> tampered = blocks;
    tampered[2] = "gamma!";

    MerkleTree tamperedTree(tampered);

    cout << (tamperedTree.root() != tree.root()) << endl;  // 1
    cout << MerkleTree::verify("gamma!", tamperedTree.proof(2),
                               tamperedTree.root())
         << endl;
    // 1

    // Single block: the root is the leaf hash and the proof is empty.
    MerkleTree single({"only"});
    printRoot("single", single.root());
    cout << (single.root() == hashString("only")) << endl;  // 1
    cout << MerkleTree::verify("only", single.proof(0), single.root()) << endl;
    // 1

    // Two blocks: each proof has one sibling.
    MerkleTree pair({"left", "right"});
    cout << MerkleTree::verify("left", pair.proof(0), pair.root()) << " "
         << MerkleTree::verify("right", pair.proof(1), pair.root()) << endl;
    // 1 1

    // Empty tree: root 0 and no proof.
    MerkleTree empty({});
    cout << empty.empty() << " " << empty.root() << " " << empty.proof(0).size()
         << endl;
    // 1 0 0

    // Invalid index: no proof.
    cout << tree.proof(-1).size() << " " << tree.proof(99).size() << endl;
    // 0 0

    // A proof for the duplicated last leaf still verifies.
    cout << MerkleTree::verify("epsilon", tree.proof(4), tree.root()) << endl;
    // 1

    return 0;
}

/**
 * Time complexity:
 *   build -> O(n) hashes, O(n) space for all levels
 *   proof -> O(log n) sibling hashes
 *   verify -> O(log n) hashes
 *   update a leaf -> O(log n) to recompute the path to the root (not shown,
 *   but the levels make it straightforward)
 * Space complexity -> O(n) for the tree, O(log n) for a proof
 *
 * Special cases:
 *   - empty tree: root 0, no valid proof
 *   - one block: root is the leaf hash, proof is empty
 *   - odd number of leaves: duplicate the last node at each level
 *   - duplicate blocks: their leaf hashes are equal, but the positions still
 *     matter; a proof is tied to a position in the tree
 *   - an invalid index returns an empty proof
 *   - a tampered block changes the root, so an old proof fails
 *   - a non-cryptographic hash does not resist collision or second-preimage
 *     attacks; use a cryptographic hash in production
 *
 * Domain separation: hash leaves as H(0x00 || data) and internal nodes as
 * H(0x01 || left || right). Without it, an attacker may present an internal
 * node as a leaf (or vice versa) and forge a proof for a different structure.
 *
 * Merkle trees are used in Git, blockchains, certificate transparency,
 * distributed databases, anti-entropy protocols, and content-addressed
 * storage. A Merkle Patricia Trie (Ethereum) adds key-value lookup on top.
 */
