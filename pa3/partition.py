import heapq
import random
import math

max_iter = 25000

# split into two sets of {+1, -1} labels
def Karmarkar_Karp(A):
    S_heap = [(-n, n, i) for i, n in enumerate(A)]
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
    
    residue = abs(heapq.heappop(S_heap)[0]) if S_heap else 0

    # now we process the popped off
    S_res = [0] * len(A)
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

# HIIIII JAREDDDD


# repeated random algorithm for the Partition problem
"""
    Start with a random solution S
    for iter = 1 to max iter
        S′ = a random solution
        if residue(S′) < residue(S) then S = S′
    return S
"""

# residue function 
def residue(A, S):
    res = 0
    for i in range(len(A)):
        res += A[i] * S[i]
    return abs(res)


# takes A, generate A'
def prepartitioning(A):
    len_A = len(A)
    partition = [random.randint(0, len_A-1) for _ in range(len_A)]
    A_prime = [0 for _ in range(len_A)]
	# combine elements with same value in partition
    for i in range(len_A):
        A_prime[partition[i]] += A[i]        
    return A_prime


# Repeated Random algorithm for the Partition problem
def RepeatedRandom(A):
    S = [random.choice([-1, 1]) for _ in range(len(A))]
    for _ in range(max_iter):
        S_prime = [random.choice([-1, 1]) for _ in range(len(S))]
        if residue(A, S) > residue(A, S_prime):
            S = S_prime
    return S, residue(A, S)


def RepeatedRandomPartition(A):
    len_A = len(A)     

    residue = math.inf

    for i in range(max_iter):
        # generate new random P, then from P generate A'
        P = [random.randint(0, len_A-1) for _ in range(len_A)]
        A_prime = [0 for _ in range(len_A)]
        for j in range(len_A):
            A_prime[P[j]] += A[j]

        # 2) run Karmarkar-Karp on A'
        residue_prime = Karmarkar_Karp(A_prime)[1]

        if residue_prime < residue:
            residue = residue_prime
            
    return residue


def calculate_neighbors(S):
    neighbors = []
    for i in range(len(S)):
        neighbor = S.copy()
        neighbor[i] = -S[i]
        neighbors.append(neighbor)
    return neighbors


# Hill Climbing algorithm for the Partition problem
def HillClimbing(A, prepartition=False):
    S = [random.choice([-1, 1]) for _ in range(len(A))]
    neighbors = calculate_neighbors(S)
    
    for i in range(max_iter):
        S_prime = random.choice(neighbors)
        if residue(A, S_prime) < residue(A, S):
            S = S_prime.copy()
            neighbors = calculate_neighbors(S)

        # if i < 100:
        #     print("S residue: ", residue(A, S))

    return S, residue(A, S)

# Simulated Annealing algorithm for the Partition problem
"""
Start with a random solution S
S′′ = S
for iter = 1 to max iter
    S′ = a random neighbor of S
    if residue(S′) < residue(S) then S = S′
    else S = S′ with probability exp(−(res(S′)-res(S))/T(iter))

    if residue(S) < residue(S′′) then S′′ = SE
return S′′
"""
def SimulatedAnnealing(A):
    S = [random.choice([-1, 1]) for _ in range(len(A))]
    S_prime_2 = S
    neighbors = calculate_neighbors(S)

    def T(iter):
        return (10**10) * ((0.8)**(iter//300))
    
    for iter in range(max_iter):
        S_prime = random.choice(neighbors)
        if residue(A, S_prime) < residue(A, S):
            S = S_prime.copy()
            neighbors = calculate_neighbors(S)
        else:
            prob1 = -(residue(A, S_prime) - residue(A, S))
            T_iter = T(iter)
            prob = math.exp(prob1 / T_iter)
            if random.uniform(0.0, 1.0) < prob:
                S = S_prime.copy()
                neighbors = calculate_neighbors(S)
        if residue(A, S) < residue(A, S_prime_2):
            S_prime_2 = S.copy()
            
    return S_prime_2, residue(A, S_prime_2)


if __name__ == "__main__":
    # A = [10, 5, 6, 10, 10, 7]
    # A = [7,1,9,100,6,1,7,1,9,9,5,2]
    A = [3,1,3,5,0,1,5,6,8,8,7,9, 103, 1, 1, 23, 123]

    # -10 + 5 + 6 - 10 - 10 - 7 = 
    print(Karmarkar_Karp(A))

    # print(RepeatedRandom(A))

    # print(HillClimbing(A))

    # print(SimulatedAnnealing(A))

    print(RepeatedRandomPartition(A))