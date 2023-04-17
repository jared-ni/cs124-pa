import heapq

# split into two sets of {+1, -1} labels
def Karmarkar_Karp(S):
    S_heap = [(-n, n, i) for i, n in enumerate(S)]
    # max heapify S
    heapq.heapify(S_heap)

    popped_off = []

    while len(S_heap) > 1:
        a = heapq.heappop(S_heap)
        b = heapq.heappop(S_heap)
        popped_off.append(a)
        popped_off.append(b)
        
        if a[0] != b[0]:
            new_val = 0
            new_val_og = 0
            new_index = 0

            # meaning if a is bigger
            if a[0] < b[0]:
                new_val = a[0] - b[0]
                new_val_og = a[1]
                new_index = a[2]
            else:
                new_val = b[0] - a[0]
                new_val_og = b[1]
                new_index = b[2]

            heapq.heappush(S_heap, (new_val, new_val_og, new_index))
    
    residue = abs(heapq.heappop(S_heap)[0])

    print("Pop off: ")
    print(popped_off)

    # now we process the popped off
    S_res = [0] * len(S)
    while popped_off:
        a = popped_off.pop()
        b = popped_off.pop()

        # check if either index is in S_res
        index_num1, index_num2 = S_res[a[2]], S_res[b[2]]
        if index_num1 != 0:
            S_res[b[2]] = -(index_num1)
        elif index_num2 != 0:
            S_res[a[2]] = -(index_num2)
        else:
            S_res[a[2]] = 1
            S_res[b[2]] = -1

    return S_res, residue


S = [10, 5, 6, 10, 10, 7]
print(Karmarkar_Karp(S))

# HIIIII JAREDDDD

