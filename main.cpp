#include <iostream>
#include <map>
#include <utility>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

typedef pair<unsigned int, unsigned int> key_pair;

// struct for training data structure + number of entries
struct dataset {
    map<key_pair, unsigned int> data;
    size_t entries = 0;
};

// probability function for bayes classifier
double prob(const key_pair& k, const dataset& d) {

    // check if the key exists first -- if not, use smoothing    
    if(d.data.find(k) == d.data.end())
	return 1.0 / (d.entries + 2);

    // finds the count related to the index + value key
    // return count / total number of tuples in +1 or -1 class
    auto it = d.data.find(k);
    return static_cast<double>(it->second) / d.entries;
}

// function to read test file and classify tuples
void test(const string& test_file, const dataset& pos, const dataset& neg) {
    
    // counters for true positive, false negative, etc
    unsigned int TP = 0, FN = 0, FP = 0, TN = 0;
    
    // open the training file
    ifstream infile;
    infile.open(test_file);
    
    // dummy variables for parsing
    string line;
    int label, index, value;
    char colon;

    // same parsing loop
    while(getline(infile, line)) {
        
        istringstream buffer(line);
        buffer >> label;
        
        // variables to hold and compare probability
        double p = 1, n = 1;
        
        // calculate probability that a tuple is +1 or -1
        while(buffer >> index >> colon >> value) {
            p *= prob(make_pair(index, value), pos);
            n *= prob(make_pair(index, value), neg);
        }
        
        // if-statements to compare and check validity
        // increment TP/FN/FP/TN accordingly
        if(p > n) {
            if(label == 1)  
                TP++;
	    else FP++;
	}
     
        else if(p < n) {
	    if(label == -1)
                TN++;
	    else FN++;
	}
    }
    
    // close file and print report
    infile.close();
    cout << TP << " " << FN << " " << FP << " " << TN << endl;
}

int main(int argc, const char* argv[]) {

    dataset pos, neg;
    
    // open the training file
    ifstream infile;
    infile.open(argv[1]);
    
    // dummy variables for parsing
    string line;
    int label, index, value;
    char colon;
    
    // file parsing loop
    while(getline(infile, line)) {
        
        // load one tuple into the buffer and read label
        istringstream buffer(line);
        buffer >> label;
        
        // if the tuple is classified as +1, hash index + value key and
        // increment its count and the number of tuples in the +1 class
        if(label == 1) {
            while(buffer >> index >> colon >> value)
                pos.data[make_pair(index, value)]++;
            pos.entries++;
        }
        
        // same for tuples classified as -1
        else {
            while(buffer >> index >> colon >> value)
                neg.data[make_pair(index, value)]++;
            neg.entries++;
        }
    }
    
    // close training file and open test file
    infile.close();
    
    // call function to test the files passed as parameters
    test(argv[1], pos, neg);
    test(argv[2], pos, neg);
    
    return EXIT_SUCCESS;
}
