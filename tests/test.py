import re
from glob import glob
import os
import numpy as np

RED = "\033[1;31m"
GREEN = "\033[1;32m"
YELLOW = "\033[1;33m"
BLUE = "\033[1;34m"
MAGENTA = "\033[1;35m"
CYAN = "\033[1;36m"
BLACK = "\033[1;30m"

TEST_FILES_DIR = "test_files"
RESULT_FILES_DIR = "result_files"

def test_assignment(number : int):
    print(f"{CYAN}Testing assignment {number}{BLACK}")

    for approach in ["a1", "a2", "a3"]:
        print(f"  {MAGENTA}Testing approach {approach}{BLACK}")
        os.system(f"rm -rf {RESULT_FILES_DIR}")
        os.system(f"make {approach} 2> /dev/null > /dev/null")
        os.makedirs(RESULT_FILES_DIR, exist_ok=True)

        for filename in sorted(glob(f"{TEST_FILES_DIR}/t{number}_*.bin")):
            if approach == "a3" and os.stat(filename).st_size < 2**10: # skip files smaller than 1024 bytes for the third approach
                continue

            result = re.search(r"t" + str(number) + r"_.*?(-?\d+)\.bin", filename).group(1)
            file_base = os.path.basename(filename).replace(".bin", "")
            output_filename = file_base + ".txt"
            
            os.system(f"file={file_base}; ./main -t {number} -i {TEST_FILES_DIR}/$file.bin -o {RESULT_FILES_DIR}/$file.txt")
            try:
                with open(f"{RESULT_FILES_DIR}/{output_filename}", "r") as f:
                    output = f.read().strip()
            except:
                output = "file could not be opened or read."
            
            if output == result:
                print(f"    {GREEN}{filename} passed.{BLACK}")
            else:
                print(f"    {RED}{filename} failed.{BLACK} Expected: {result}, got: {output}")

def test_assignment_3():
    print(f"{CYAN}Testing assignment 3{BLACK}")

    for approach in ["a1", "a2", "a3"]:
        print(f"  {MAGENTA}Testing approach {approach}{BLACK}")
        os.system(f"rm -rf {RESULT_FILES_DIR}")
        os.system(f"make {approach} 2>/dev/null >/dev/null")
        os.makedirs(RESULT_FILES_DIR, exist_ok=True)

        os.system(f"file=t3_1; ./main -t 3 -i {TEST_FILES_DIR}/$file.txt -o {RESULT_FILES_DIR}/$file.txt 2>/dev/null >/dev/null")
        os.system(f"file=t3_2; ./main -t 3 -i {TEST_FILES_DIR}/$file.txt -o {RESULT_FILES_DIR}/$file.txt 2>/dev/null >/dev/null")

        filename = "t3_1.txt"
        try:
            with open(f"{RESULT_FILES_DIR}/{filename}", "r") as f:
                output = f.read().strip()
        except:
            output = "file could not be opened or read."

        result = "0\n276\n413\n483\n634\n541\n441\n221\n206\n100\n97\n0\n0"
        if output == result:
            print(f"    {GREEN}{filename} passed.{BLACK}")
        else:
            print(f"    {RED}{filename} failed.{BLACK} Expected: {result}, got: {output}")
        
        filename = "t3_2.txt"
        try:
            with open(f"{RESULT_FILES_DIR}/{filename}", "r") as f:
                output = f.read().strip()
        except:
            output = "file could not be opened or read."

        result = "0\n97\n97\n394\n394\n394\n394\n394\n394\n32\n58\n68\n0"
        if output == result:
            print(f"    {GREEN}{filename} passed.{BLACK}")
        else:
            print(f"    {RED}{filename} failed.{BLACK} Expected: {result}, got: {output}")

def test_assignment_4():
    np.random.seed(42)

    print(f"{CYAN}Testing assignment 4{BLACK}")
    os.system(f"rm -rf {RESULT_FILES_DIR}")
    os.makedirs(RESULT_FILES_DIR, exist_ok=True)
    os.system(f"make 2>/dev/null >/dev/null")
    filename = "t4_random.bin"

    for i in range(10):
        n = np.random.randint(1, 100)
        data = np.random.randint(-100, 100, n, dtype=np.int32)
        data.tofile(os.path.join(TEST_FILES_DIR, filename))

        os.system(f"file={filename}; ./main -t 4 -i {TEST_FILES_DIR}/$file -o {RESULT_FILES_DIR}/$file")
        try:
            with open(f"{RESULT_FILES_DIR}/{filename}", "rb") as f:
                output = f.read()
                commands = np.frombuffer(output, dtype=np.uint64)    
        except:
            print(f"    {RED}Test {i} failed.{BLACK}")
            exit(1)
        
        for command in commands:
            data[:command] = np.flip(data[:command])
        
        if np.all(data[:-1] <= data[1:]): # verify that the array is sorted
            print(f"    {GREEN}Test {i} passed.{BLACK}")
        else:
            print(f"    {RED}Test {i} failed.{BLACK}")
            exit(1)

if __name__ == "__main__":
    test_assignment(1)
    test_assignment(2)
    test_assignment_3()
    test_assignment_4()