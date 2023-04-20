import heapq
import random
import math
import time

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

    residue = int(abs(heapq.heappop(S_heap)[0])) if S_heap else 0

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

    return residue


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


# Repeated Random algorithm for the Partition problem
def RepeatedRandom(A):
    S = [random.choice([-1, 1]) for _ in range(len(A))]
    for _ in range(max_iter):
        S_prime = [random.choice([-1, 1]) for _ in range(len(S))]
        if residue(A, S) > residue(A, S_prime):
            S = S_prime
    return residue(A, S)


# swap one or two elements from one set, or swap one element from each set
def calculate_neighbor(S):
    S_prime = []

    choice = random.choice([1, 2])

    if choice == 1:
        i = random.randint(0, len(S)-1)
        S_prime = S.copy()
        S_prime[i] = -S_prime[i]
    else:
        i = random.randint(0, len(S)-1)
        j = random.randint(0, len(S)-1)
        while i == j:
            j = random.randint(0, len(S)-1)
        S_prime = S.copy()
        S_prime[i] = -S_prime[i]
        S_prime[j] = -S_prime[j]

    return S_prime


# Hill Climbing algorithm for the Partition problem
def HillClimbing(A):
    S = [random.choice([-1, 1]) for _ in range(len(A))]
    residue_res = residue(A, S)

    for _ in range(max_iter):
        S_prime = calculate_neighbor(S)
        residue_prime = residue(A, S_prime)
        if residue_prime < residue_res:
            S = S_prime.copy()
            residue_res = residue_prime

    return residue_res


def random_partition_neighbor(P):
    P_prime = P.copy()
    # Choose two random indices i and j from [1, n] with pi != j and set pi to j.
    i = random.randint(0, len(P)-1)
    j = random.randint(0, len(P)-1)
    while j == P[i]:
        j = random.randint(0, len(P)-1)
    P_prime[i] = j
    return P_prime


def calculate_A_prime(P):
    len_A = len(P)
    A_prime = [0 for _ in range(len_A)]
    for j in range(len_A):
        A_prime[P[j]] += A[j]
    return A_prime


def T(iter):
    return (10**10) * ((0.8)**(iter//300))


# Simulated Annealing algorithm for the Partition problem
def SimulatedAnnealing(A):
    S = [random.choice([-1, 1]) for _ in range(len(A))]

    S_prime_2 = S

    for iter in range(max_iter):
        S_prime = calculate_neighbor(S)

        if residue(A, S_prime) < residue(A, S):
            S = S_prime.copy()

        elif random.uniform(0.0, 1.0) < math.exp(-(residue(A, S_prime) - residue(A, S)) / T(iter)):
            S = S_prime.copy()

        if residue(A, S) < residue(A, S_prime_2):
            S_prime_2 = S.  copy()

    return residue(A, S_prime_2)


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
        residue_prime = Karmarkar_Karp(A_prime)

        if residue_prime < residue:
            residue = residue_prime

    return residue


# prepartitioning Hill Climbing algorithm
def HillClimbingPartition(A):
    len_A = len(A)
    residue = Karmarkar_Karp(A)
    P = None

    for i in range(max_iter):
        P_prime = (random_partition_neighbor(P) if P
                   else [random.randint(0, len_A-1) for _ in range(len_A)])
        A_prime = calculate_A_prime(P_prime)

        residue_prime = Karmarkar_Karp(A_prime)
        if residue_prime < residue:
            P = P_prime.copy()
            # A = A_prime.copy()
            residue = residue_prime

    return residue


def SimulatedAnnealingPartition(A):
    len_A = len(A)

    P = [random.randint(0, len_A-1) for _ in range(len_A)]
    A_res = calculate_A_prime(P)
    residue = Karmarkar_Karp(A_res)

    # P_prime_2 = P.copy()
    residue_prime_2 = residue

    for iter in range(max_iter):
        P_prime = random_partition_neighbor(P)
        A_prime = calculate_A_prime(P_prime)
        residue_prime = Karmarkar_Karp(A_prime)
        if residue_prime < residue:
            P = P_prime.copy()
            residue = residue_prime
        elif random.uniform(0.0, 1.0) < math.exp(-(residue_prime - residue) / T(iter)):
            P = P_prime.copy()
            residue = residue_prime
        if residue < residue_prime_2:
            # P_prime_2 = P.copy()
            residue_prime_2 = residue

    return residue_prime_2


# generate set of 100 random integers, from range [1, 10^12]
def generate_set(min, max, n):
    set = []
    for _ in range(n):
        set.append(random.randint(min, max))
    return set


kk = open("kk.txt", "w")
rr = open("rr.txt", "w")
hc = open("hc.txt", "w")
sa = open("sa.txt", "w")
rrp = open("rrp.txt", "w")
hcp = open("hcp.txt", "w")
sap = open("sap.txt", "w")
kk_time = open("kk_time.txt", "w")
rr_time = open("rr_time.txt", "w")
hc_time = open("hc_time.txt", "w")
sa_time = open("sa_time.txt", "w")
rrp_time = open("rrp_time.txt", "w")
hcp_time = open("hcp_time.txt", "w")
sap_time = open("sap_time.txt", "w")

# generate 50 random instances of the problem and run each algorithm
for _ in range(50):
    A = generate_set(1, 10 ** 12, 100)

    with open("kk.txt", "a") as kk:
        start = time.time()
        kk.write(str(Karmarkar_Karp(A)) + "\n")
        end = time.time()
        with open("kk_time.txt", "a") as kk_time:
            kk_time.write(str(end - start) + "\n")

    with open("rr.txt", "a") as rr:
        start = time.time()
        rr.write(str(RepeatedRandom(A)) + "\n")
        end = time.time()
        with open("rr_time.txt", "a") as rr_time:
            rr_time.write(str(end - start) + "\n")

    with open("hc.txt", "a") as hc:
        start = time.time()
        hc.write(str(HillClimbing(A)) + "\n")
        end = time.time()
        with open("hc_time.txt", "a") as hc_time:
            hc_time.write(str(end - start) + "\n")

    with open("sa.txt", "a") as sa:
        start = time.time()
        sa.write(str(SimulatedAnnealing(A)) + "\n")
        end = time.time()
        with open("sa_time.txt", "a") as sa_time:
            sa_time.write(str(end - start) + "\n")

    with open("rrp.txt", "a") as rrp:
        start = time.time()
        rrp.write(str(RepeatedRandomPartition(A)) + "\n")
        end = time.time()
        with open("rrp_time.txt", "a") as rrp_time:
            rrp_time.write(str(end - start) + "\n")

    with open("hcp.txt", "a") as hcp:
        start = time.time()
        hcp.write(str(HillClimbingPartition(A)) + "\n")
        end = time.time()
        with open("hcp_time.txt", "a") as hcp_time:
            hcp_time.write(str(end - start) + "\n")

    with open("sap.txt", "a") as sap:
        start = time.time()
        sap.write(str(SimulatedAnnealingPartition(A)) + "\n")
        end = time.time()
        with open("sap_time.txt", "a") as sap_time:
            sap_time.write(str(end - start) + "\n")
