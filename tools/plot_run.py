import csv
import sys

import matplotlib.pyplot as plt

xs, ys = [], []
with open(sys.argv[1], newline="") as f:
    for row in csv.DictReader(f):
        xs.append(float(row["x"]))
        ys.append(float(row["y"]))

fig, ax = plt.subplots()
ax.plot(xs, ys, label="driven path")
ax.plot(xs[0], ys[0], "o", label="start")
ax.set_aspect("equal")
ax.set_xlabel("x [m]")
ax.set_ylabel("y [m]")
ax.legend()
ax.grid(True)
fig.savefig(sys.argv[2] if len(sys.argv) > 2 else "run.png", dpi=150)
