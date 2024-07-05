import numpy as np
import pandas as pd
import os
import subprocess
import matplotlib.pyplot as plt

np.random.seed(42)

TEST_FILES_DIR = "test_files"
RESULT_FILES_DIR = "result_files"
FIGS_DIR = "figs"

os.makedirs(TEST_FILES_DIR, exist_ok=True)
os.makedirs(RESULT_FILES_DIR, exist_ok=True)
os.makedirs(FIGS_DIR, exist_ok=True)

MIN_POWER = 20
MAX_POWER = 30
data = np.random.randint(-2**30, 2**30, 2**MAX_POWER, dtype=np.int32)

approaches = ["a1", "a2", "a3"]

for assignment in range(1, 3):
    df = pd.DataFrame(columns=approaches, index=list(range(MIN_POWER, MAX_POWER + 1)))
    for approach in approaches:
        os.system(f"make {approach} 2> /dev/null > /dev/null")

        for size in range(MIN_POWER, MAX_POWER + 1):
            data[:2**size].tofile(f"{TEST_FILES_DIR}/performance.bin")
            result = subprocess.run(f"./main -t {assignment} -i {TEST_FILES_DIR}/performance.bin -o {RESULT_FILES_DIR}/performance.txt", shell=True, stderr=subprocess.PIPE)
            stderr_output = int(result.stderr.decode('utf-8').strip())
            df.loc[size, approach] = stderr_output

    plt.figure(figsize=(12, 7))

    plt.plot(df.index, df[approaches[0]], label="Approach 1", marker='o')
    plt.plot(df.index, df[approaches[1]], label="Approach 2", marker='s')

    plt.title(f"Assignment {assignment}: Performance Comparison of Implemented Approaches")
    plt.xlabel(f"File size (2^x bytes)")
    plt.ylabel(f"Time (ms)")
    plt.ylim(0, 2000 if df.max().max() > 2000 else df.max().max() + 25)
    plt.legend()
    plt.tight_layout()

    plt.savefig(f"{FIGS_DIR}/assignment_{assignment}.png", dpi=300)
    plt.show()

os.system(f"rm -rf {TEST_FILES_DIR}/performance.bin")
