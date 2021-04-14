import random

sample = list(range(0, 9)) * 8
random.shuffle(sample)
for i in range(8):
    for j in range(9):
        print(sample[i * 9 + j], end=' ')
    print()
