Analyze and compare the different persistence strategies in terms of time
complexity, memory usage, and structural sharing.

Path Copying has a time complexity of O(log n), similarly a memory usage of O(log n) per operation,
    but it has excellent structural sharing as it fully shares between the subtrees

Fat Node has an O(log n * MAX_MODS) time complexity and an amortized memory usage of O(1),
    but doesn't have the best structural sharing, as the node reuse is limted by the amount of Mods slots

Lazy has an amortized O(1) time complexity, an O(1) memory usage per operation,
    and has great structural sharing at the cost of some spikes in latency.