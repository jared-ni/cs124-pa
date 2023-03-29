import sys

dimension = int(sys.argv[1])

# open file
f = open("test_file_" + str(dimension) + ".txt", "w")

# write first matrix, one number per line
for i in range(0, dimension ** 2):
    f.write(str(i) + '\n')

# write second matrix, one number per line
for i in range(dimension ** 2, 2 * dimension ** 2):
    f.write(str(i) + '\n')

# close file
f.close()
