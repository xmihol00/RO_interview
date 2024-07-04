import numpy as np
import os

DST_DIR = "test_files"
os.makedirs(DST_DIR, exist_ok=True)

np.array([-1, 1, -1], dtype=np.int32).tofile(os.path.join(DST_DIR, "t1_1.bin"))
np.array([4, 3, 2], dtype=np.int32).tofile(os.path.join(DST_DIR, "t1_2.bin"))
np.array([4, 3, -2], dtype=np.int32).tofile(os.path.join(DST_DIR, "t1_-2.bin"))
np.array([200, -6, 300, -6, -6, 400, 100, -6, 6], dtype=np.int32).tofile(os.path.join(DST_DIR, "t1_6.bin"))


np.array([5, 4, 0, 0, -1, 0, 2, 0, 0], dtype=np.int32).tofile(os.path.join(DST_DIR, "t2_3.bin"))
np.array([0, 2, 0, 2, 0, 2, 0, 2, 0], dtype=np.int32).tofile(os.path.join(DST_DIR, "t2_4.bin"))
np.array([2, 0, 2, 0, 2, 0, 2, 0, 2], dtype=np.int32).tofile(os.path.join(DST_DIR, "t2_5.bin"))
np.array([2, 0, 2, 0], dtype=np.int32).tofile(os.path.join(DST_DIR, "t2_2.bin"))
np.array([5, 4, 0, 0, -1, 0, 2, -5, 5, -5, 5, 0, 0, 4, -4, 0, 1, 0, 22, 3, 0, 7, 7, 7, 7], dtype=np.int32).tofile(os.path.join(DST_DIR, "t2_7.bin"))
