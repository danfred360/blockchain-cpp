#include <ctime>
#include <vector>
#include <string>
#include <iostream>
#include <ostream>

using namespace std;

// Transaction Data
struct TransactionData {
    double amount;
    string senderKey;
    string receiverKey;
    time_t timestamp;
};

// Block Class
class Block
{
    private:
      int index;
      size_t blockHash;
      size_t previousHash;
      size_t generateHash();

    public:
      // Constructor
        Block(int idx, TransactionData d, size_t previousHash);

        // Get Original Hash
        size_t getHash();

        // Get Previous Hash
        size_t getPreviousHash();

        // Transaction Data
        TransactionData data;

        // Validate Hash
        bool isHashValid();
};

// Constructor with params
Block::Block(int idx, TransactionData d, size_t prevHash)
{
    index = idx;
    data = d;
    previousHash = prevHash;
    blockHash = generateHash();
};

// private functions
size_t Block::generateHash()
{
    hash<string> hash1;
    hash<size_t> hash2;
    hash<size_t> finalHash;
    string toHash = to_string(data.amount) + data.receiverKey + data.senderKey + to_string(data.timestamp);

    return finalHash(hash1(toHash) + hash2(previousHash));
};

// Public Functions
size_t Block::getHash()
{
    return blockHash;
}

size_t Block::getPreviousHash()
{
    return previousHash;
}

bool Block::isHashValid()
{
    return generateHash() == blockHash;
}

// Blockchain
class Blockchain
{
    private:
      Block createGenesisBlock();
    public:
      // Public Chain
      vector<Block> chain;

      // Constructor
      Blockchain();

      // Public Functions
      void addBlock(TransactionData data);
      bool isChainValid();

      // Contrived Example For Demo Only!!!
      Block *getLatestBlock();
};

// Blockchain Constructor
Blockchain::Blockchain()
{
    Block genesis = createGenesisBlock();
    chain.push_back(genesis);
};

Block Blockchain::createGenesisBlock()
{
    time_t current;
    TransactionData d;
    d.amount = 0;
    d.receiverKey = "None";
    d.senderKey = "None";
    d.timestamp = time(&current);

    hash<int> hash1;
    Block genesis(0, d, hash1(0));
    return genesis;
};

// Bad!! Only for demo!!!
Block *Blockchain::getLatestBlock()
{
    return &chain.back();
};

void Blockchain::addBlock(TransactionData d)
{
    int index = (int)chain.size() - 1;
    Block newBlock(index, d, getLatestBlock()->getHash());
};

bool Blockchain::isChainValid()
{
    vector<Block>::iterator it;
    int chainLen = (int)chain.size();

    for(it = chain.begin(); it != chain.end(); ++it)
    {
        Block currentBlock = *it;
        if (!currentBlock.isHashValid())
        {
            // INVALID!!
            return false;
        }

        if(chainLen > 1)
        {
            Block previousBlock = *(it - 1);
            if (currentBlock.getPreviousHash() != previousBlock.getHash())
            {
                // INVALID!!
                return false;
            }
        }
    }

    return true;
};

int main()
{
    // Start Blockchain!!
    Blockchain FrostyCoin;

    // Data for first added Block
    TransactionData data1;
    time_t data1Time;
    data1.amount = 1.5;
    data1.receiverKey = "frosty";
    data1.senderKey = "npole";
    data1.timestamp = time(&data1Time);

    FrostyCoin.addBlock(data1);

    cout << "Is chain valid?" << endl
         << FrostyCoin.isChainValid() << endl;

    // Data for second added Block
    TransactionData data2;
    time_t data2Time;
    data2.amount = 0.00013322;
    data2.receiverKey = "npole";
    data2.senderKey = "frosty";
    data2.timestamp = time(&data2Time);

    FrostyCoin.addBlock(data2);

    cout << "Now is chain valid?" << endl
         << FrostyCoin.isChainValid() << endl;

    // Sneaky
    Block *hackBlock = FrostyCoin.getLatestBlock();
    hackBlock->data.amount = 10000;
    hackBlock->data.receiverKey = "anon";
    
    cout << "Now is the chain still valid???" << endl;
    cout << FrostyCoin.isChainValid() << endl;

};