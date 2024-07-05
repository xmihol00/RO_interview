import re
from glob import glob
import os

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
        

if __name__ == "__main__":
    test_assignment(1)
    test_assignment(2)