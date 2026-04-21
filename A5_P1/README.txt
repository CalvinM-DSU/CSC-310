- Binomial heaps merge in O(log n) by simply combining root lists and linking trees of equal degree,
    whereas binary heaps require rebuilding the heap, which is O(n)

- insert, merge, and deleteMin are all O(log n), findMin is O(log n) (or O(1) with a pointer),
    and decreaseKey is O(log n) after locating the node

- This structure of left/right lets each node have an arbitrary number of children using only two pointers,
    making tree linking, traversal, and merging more efficient.

- I could not access the full article due to a 401 Unknown Reason, but reading the abstract,
    I find it very interesting that a binomial heap is a better data structure for the task than an RB tree.
    I'd assume that this is due to there not being too many arbitrary values to account for, and they just need a min/max.