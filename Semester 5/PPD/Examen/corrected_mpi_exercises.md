# Corrected MPI Exercises

## Exercise 1: MPI Merge Sort (Jan-Feb 2026, Subject 3)

### Problem Statement
Consider a program that is supposed to merge-sort a vector. The function `worker()` is called in all processes except process 0, the function `mergeSort()` is called from process 0. `mergeSortLocal()` sorts the specified vector, and `mergeParts()` merges two sorted adjacent vectors.

### Original Code (WITH BUGS)
```cpp
void mergeSort(int* v, int dataSize, int myId, int nrProc) {
    if(dataSize <= 1) {
        mergeSortLocal(v, dataSize);
    } else {
        int halfProc = (nrProc+1) / 2;
        int child = myId+halfProc;
        int halfSize = (dataSize+1) / 2;
        MPI_Ssend(&halfProc, 1, MPI_INT, child, 1, MPI_COMM_WORLD);
        MPI_Ssend(&halfSize, 1, MPI_INT, child, 2, MPI_COMM_WORLD);
        MPI_Ssend(v, halfSize, MPI_INT, child, 3, MPI_COMM_WORLD);
        mergeSort(v+halfSize, dataSize/2, myId, halfProc);  // BUG: dataSize/2 and halfProc are wrong!
        MPI_Recv(v, halfSize, MPI_INT, child, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        mergeParts(v, dataSize, halfSize);
    }
}

void worker(int myId) {
    MPI_Status status;
    int dataSize, nrProc;
    MPI_Recv(&nrProc, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
    auto parent = status.MPI_SOURCE;
    MPI_Recv(&dataSize, 1, MPI_INT, parent, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::vector v(dataSize);  // BUG: Missing type parameter
    MPI_Recv(v.data(), dataSize, MPI_INT, parent, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    mergeSort(v.data(), dataSize, myId, nrProc);
    MPI_Ssend(v.data(), dataSize, MPI_INT, parent, 4, MPI_COMM_WORLD);
}
```

### Bugs Identified
1. **Wrong base case**: `dataSize <= 1` should check `nrProc <= 1` (when only one process available)
2. **Wrong remaining size**: `dataSize/2` should be `dataSize - halfSize`
3. **Wrong remaining processes**: `halfProc` should be `nrProc - halfProc`
4. **Missing template parameter**: `std::vector v(dataSize)` should be `std::vector<int> v(dataSize)`

### CORRECTED Code
```cpp
#include <mpi.h>
#include <vector>

// Assume these are provided:
void mergeSortLocal(int* v, int dataSize);
void mergeParts(int* v, int totalSize, int firstPartSize);

void mergeSort(int* v, int dataSize, int myId, int nrProc) {
    if(nrProc <= 1) {  // FIX: Base case is when only 1 process available
        mergeSortLocal(v, dataSize);
    } else {
        int halfProc = (nrProc + 1) / 2;
        int child = myId + halfProc;
        int halfSize = (dataSize + 1) / 2;
        
        // Send child's portion info
        MPI_Ssend(&halfProc, 1, MPI_INT, child, 1, MPI_COMM_WORLD);
        MPI_Ssend(&halfSize, 1, MPI_INT, child, 2, MPI_COMM_WORLD);
        MPI_Ssend(v, halfSize, MPI_INT, child, 3, MPI_COMM_WORLD);
        
        // Parent sorts remaining portion with remaining processes
        // FIX: Use (dataSize - halfSize) for remaining elements
        // FIX: Use (nrProc - halfProc) for remaining processes
        mergeSort(v + halfSize, dataSize - halfSize, myId, nrProc - halfProc);
        
        // Receive sorted portion from child
        MPI_Recv(v, halfSize, MPI_INT, child, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        // Merge the two sorted halves
        mergeParts(v, dataSize, halfSize);
    }
}

void worker(int myId) {
    MPI_Status status;
    int dataSize, nrProc;
    
    // Receive number of processes for this subtree
    MPI_Recv(&nrProc, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
    auto parent = status.MPI_SOURCE;
    
    // Receive data size and data
    MPI_Recv(&dataSize, 1, MPI_INT, parent, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::vector<int> v(dataSize);  // FIX: Added <int> type parameter
    MPI_Recv(v.data(), dataSize, MPI_INT, parent, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    
    // Sort recursively
    mergeSort(v.data(), dataSize, myId, nrProc);
    
    // Send sorted data back to parent
    MPI_Ssend(v.data(), dataSize, MPI_INT, parent, 4, MPI_COMM_WORLD);
}
```

---

## Exercise 2: MPI Polynomial Product (Feb 2022, Subject 3)

### Problem Statement
Consider a program that is supposed to compute the product of two non-zero polynomials. The function `worker()` is called in all processes except process 0, the function `product()` is called from process 0. Polynomials are represented with coefficient for degree 0 at index 0 in the vector.

### Original Code (WITH BUGS)
```cpp
void product(int nrProc,
        std::vector<int> const& p,
        std::vector<int> const& q,
        std::vector<int>& r) {
    int sizes[2];
    sizes[0] = p.size();
    sizes[1] = q.size();
    MPI_Bcast(sizes, 2, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(const_cast<int*>(p.data()), p.size(), MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(const_cast<int*>(q.data()), q.size(), MPI_INT, 0, MPI_COMM_WORLD);
    std::vector<int> partRes;
    partProd(myId, nrProc, p, q, partRes);  // BUG: myId not defined
    r.resize(p.size()+q.size()-1);
    MPI_Gather(partResult.data(), partResult.size(), MPI_INT,  // BUG: wrong variable name
        r.data(), partResult.size(), MPI_INT, 0, MPI_COMM_WORLD);
}

void worker(int myId, int nrProc) {
    int sizes[2];
    metadata[0] = nrProc;  // BUG: metadata undefined, wrong logic
    MPI_Bcast(sizes, 2, MPI_INT, 0, MPI_COMM_WORLD);
    p.resize(sizes[0]);  // BUG: p not declared
    q.resize(sizes[1]);  // BUG: q not declared
    MPI_Bcast(p.data(), p.size(), MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(q.data(), q.size(), MPI_INT, 0, MPI_COMM_WORLD);
    std::vector<int> r;
    partProd(myId, nrProc, p, q, r);
    MPI_Gather(r.data(), r.size(), MPI_INT, nullptr, 0, MPI_INT, 0, MPI_COMM_WORLD);
}

void partProd(int myId, int nrProc, std::vector<int> const& p, std::vector<int> const& q,
        std::vector<int>& r) {
    int chunkSize = (p.size()+q.size()-1) / nrProc;  // BUG: Doesn't handle remainder
    r.resize(chunkSize, 0);
    for(int i=0 ; i<chunkSize ; ++i) {
        int degree = i+myId*chunkSize;
        for(int j=0 ; j<=degree ; ++j) {
            if(j<p.size() && degree-j<q.size()) {
                r[i] += p[j]*q[degree-j];
            }
        }
    }
}
```

### Bugs Identified
1. **Undefined variable**: `myId` not passed to `product()`
2. **Wrong variable name**: `partResult` vs `partRes`
3. **Undefined variables**: `metadata`, `p`, `q` in worker
4. **Uneven distribution**: Doesn't handle when result size isn't divisible by nrProc
5. **Gather size mismatch**: Workers might have different chunk sizes

### CORRECTED Code
```cpp
#include <mpi.h>
#include <vector>

void partProd(int myId, int nrProc, std::vector<int> const& p, std::vector<int> const& q,
        std::vector<int>& r) {
    int resultSize = p.size() + q.size() - 1;
    int baseChunk = resultSize / nrProc;
    int remainder = resultSize % nrProc;
    
    // Each process gets baseChunk elements, first 'remainder' processes get one extra
    int myChunkSize = baseChunk + (myId < remainder ? 1 : 0);
    int startDegree = myId * baseChunk + std::min(myId, remainder);
    
    r.resize(myChunkSize, 0);
    for(int i = 0; i < myChunkSize; ++i) {
        int degree = startDegree + i;
        for(int j = 0; j <= degree; ++j) {
            if(j < (int)p.size() && degree - j < (int)q.size()) {
                r[i] += p[j] * q[degree - j];
            }
        }
    }
}

void product(int myId, int nrProc,  // FIX: Added myId parameter
        std::vector<int> const& p,
        std::vector<int> const& q,
        std::vector<int>& r) {
    int sizes[2];
    sizes[0] = p.size();
    sizes[1] = q.size();
    MPI_Bcast(sizes, 2, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(const_cast<int*>(p.data()), p.size(), MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(const_cast<int*>(q.data()), q.size(), MPI_INT, 0, MPI_COMM_WORLD);
    
    std::vector<int> partRes;
    partProd(myId, nrProc, p, q, partRes);  // FIX: Use correct myId
    
    int resultSize = p.size() + q.size() - 1;
    r.resize(resultSize);
    
    // FIX: Use MPI_Gatherv for variable-sized chunks
    std::vector<int> recvCounts(nrProc);
    std::vector<int> displs(nrProc);
    int baseChunk = resultSize / nrProc;
    int remainder = resultSize % nrProc;
    
    int offset = 0;
    for(int i = 0; i < nrProc; ++i) {
        recvCounts[i] = baseChunk + (i < remainder ? 1 : 0);
        displs[i] = offset;
        offset += recvCounts[i];
    }
    
    MPI_Gatherv(partRes.data(), partRes.size(), MPI_INT,
                r.data(), recvCounts.data(), displs.data(), MPI_INT, 0, MPI_COMM_WORLD);
}

void worker(int myId, int nrProc) {
    int sizes[2];
    MPI_Bcast(sizes, 2, MPI_INT, 0, MPI_COMM_WORLD);
    
    std::vector<int> p(sizes[0]);  // FIX: Properly declare p
    std::vector<int> q(sizes[1]);  // FIX: Properly declare q
    
    MPI_Bcast(p.data(), p.size(), MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(q.data(), q.size(), MPI_INT, 0, MPI_COMM_WORLD);
    
    std::vector<int> r;
    partProd(myId, nrProc, p, q, r);
    
    // FIX: Use MPI_Gatherv - workers don't need receive buffer
    MPI_Gatherv(r.data(), r.size(), MPI_INT,
                nullptr, nullptr, nullptr, MPI_INT, 0, MPI_COMM_WORLD);
}
```

---

## Exercise 3: MPI Scalar Product (Subject 2)

### Problem Statement
Consider a program that is supposed to compute the scalar product of two vectors of the same length. The function `worker()` is called in all processes except process 0, the function `product()` is called from process 0.

### Original Code (WITH BUGS)
```cpp
int product(int nrProc, std::vector<int> const& p, std::vector<int> const& q) {
    int chunkSize = (p.size() + nrProc - 1) / nrProc;  // Rounds up
    std::vector<int> partResults(nrProc);
    partProd(chunkSize, p.data(), q.data(), partResults.data());
    int sum = 0;
    for(int const& v : partResults) sum += v;
    return sum;
}

void worker(int myId, int nrProc) {
    partProd(0, nullptr, nullptr, nullptr);
}

void partProd(int chunkSize, int const* p, int const* q, int* r) {
    MPI_Bcast(&chunkSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
    std::vector<int> pp(chunkSize);
    std::vector<int> qq(chunkSize);
    MPI_Scatter(p, chunkSize, MPI_INT, pp.data(), chunkSize, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(q, chunkSize, MPI_INT, qq.data(), chunkSize, MPI_INT, 0, MPI_COMM_WORLD);
    int sum = 0;
    for(int i=0 ; i<chunkSize ; ++i) {
        sum += pp[i]*qq[i];
    }
    MPI_Gather(&sum, 1, MPI_INT, r, 1, MPI_INT, 0, MPI_COMM_WORLD);
}
```

### Issues When Vector Length Not Multiple of nrProc
- **A: Illegal memory access** - YES! `MPI_Scatter` expects `chunkSize * nrProc` elements, but vector might be smaller
- **E: Some terms not added** - YES! If we use exact division, remainder elements are ignored

### CORRECTED Code
```cpp
#include <mpi.h>
#include <vector>
#include <algorithm>

void partProd(int myId, int nrProc, int vectorSize, int const* p, int const* q, int* r) {
    // Broadcast the actual vector size
    MPI_Bcast(&vectorSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Calculate variable chunk sizes
    int baseChunk = vectorSize / nrProc;
    int remainder = vectorSize % nrProc;
    
    // Each process gets baseChunk elements, first 'remainder' processes get one extra
    std::vector<int> sendCounts(nrProc);
    std::vector<int> displs(nrProc);
    
    int offset = 0;
    for(int i = 0; i < nrProc; ++i) {
        sendCounts[i] = baseChunk + (i < remainder ? 1 : 0);
        displs[i] = offset;
        offset += sendCounts[i];
    }
    
    int myChunkSize = sendCounts[myId];
    std::vector<int> pp(myChunkSize);
    std::vector<int> qq(myChunkSize);
    
    // Use MPI_Scatterv for variable-sized chunks
    MPI_Scatterv(p, sendCounts.data(), displs.data(), MPI_INT, 
                 pp.data(), myChunkSize, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(q, sendCounts.data(), displs.data(), MPI_INT, 
                 qq.data(), myChunkSize, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Compute local sum
    int sum = 0;
    for(int i = 0; i < myChunkSize; ++i) {
        sum += pp[i] * qq[i];
    }
    
    // Gather all partial sums
    MPI_Gather(&sum, 1, MPI_INT, r, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

int product(int nrProc, std::vector<int> const& p, std::vector<int> const& q) {
    std::vector<int> partResults(nrProc);
    
    // FIX: Pass actual vector size, not rounded-up chunk size
    partProd(0, nrProc, p.size(), p.data(), q.data(), partResults.data());
    
    int sum = 0;
    for(int const& v : partResults) sum += v;
    return sum;
}

void worker(int myId, int nrProc) {
    // FIX: Pass myId for proper chunk calculation
    partProd(myId, nrProc, 0, nullptr, nullptr, nullptr);
}
```

---

## Summary of Common MPI Issues

| Issue | Cause | Fix |
|-------|-------|-----|
| **Illegal memory access** | Scatter/Gather with fixed chunk size when data size isn't divisible by nrProc | Use `MPI_Scatterv`/`MPI_Gatherv` with variable chunk sizes |
| **Missing terms** | Integer division ignores remainder | Distribute remainder elements across first `remainder` processes |
| **Deadlock** | Mismatched send/receive counts or tags | Ensure all processes call collective operations with consistent parameters |
| **Wrong recursion** | Incorrect calculation of remaining work | Use `dataSize - halfSize` for remaining elements, `nrProc - halfProc` for remaining processes |
