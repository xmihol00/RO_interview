import numpy as np
import os

DST_DIR = "test_files"
os.makedirs(DST_DIR, exist_ok=True)

np.array([-1, 1, -1], dtype=np.int32).tofile(os.path.join(DST_DIR, "t1_1.bin"))
np.array([4, 3, 2], dtype=np.int32).tofile(os.path.join(DST_DIR, "t1_2.bin"))
np.array([4, 3, -2], dtype=np.int32).tofile(os.path.join(DST_DIR, "t1_-2.bin"))
np.array([200, -6, 300, -6, -6, 400, 100, -6, 6], dtype=np.int32).tofile(os.path.join(DST_DIR, "t1_6.bin"))
