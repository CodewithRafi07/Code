print("Hello, World!")

for i in range(65, 91):
    print(chr(i), end=" ")

for i in range(5):
    print("* " * 5)


for i in range(65, 70):
    for j in range(65, i + 1):
        print(chr(j), end=" ")
    print()


