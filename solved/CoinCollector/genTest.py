import sys, subprocess
import random

def gen_test(n: int):
    file = open('t0.in', 'w')
    file.write(f'{n} {int(n * (n - 1) / 2)}\n')

    for i in range(n):
        file.write(f'{random.randint(1, 1000000000)} ')
    file.write('\n')

    for i in range(n):
        for j in range(i + 1, n):
            file.write(f'{i + 1} {j + 1}\n')


def gen_test2(n, m):
    file = open('t0.in', 'w')
    file.write(f'{n} {m}\n')

    for i in range(n):
        file.write(f'{random.randint(1, 3)} ')
    file.write('\n')

    for i in range(m):
        file.write(f'{random.randint(1, n)} {random.randint(1, n)}\n')



def run_test(trials):
   
    for i in range(trials):
        gen_test2(10, 40)
        r1 = subprocess.run(['./a.out'], stdin=open('t0.in', 'r'), capture_output=True)
        o1 = int(r1.stdout)
        sys.stdout.flush()

        r2 = subprocess.run(['./test'], stdin=open('t0.in', 'r'), capture_output=True)
        o2 = int(r2.stdout)
        sys.stdout.flush()

        #print(f'{o1} {o2}')

        if (o1 != o2):
            print('error')
            break

run_test(int(sys.argv[1]))
