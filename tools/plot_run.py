import csv
import sys

import matplotlib.pyplot as plt

# Usage: python tools/plot_run.py run.csv [track.csv] [out.png]


def read_xy(path):
    xs, ys = [], []
    with open(path, newline="") as f:
        for row in csv.DictReader(f):
            xs.append(float(row["x"]))
            ys.append(float(row["y"]))
    return xs, ys


run_x, run_y = read_xy(sys.argv[1])

fig, ax = plt.subplots()
if len(sys.argv) > 2:
    track_x, track_y = read_xy(sys.argv[2])
    # Close the loop for drawing: the last point connects back to the first.
    ax.plot(track_x + track_x[:1], track_y + track_y[:1], "--", color="grey", label="track")
ax.plot(run_x, run_y, label="driven path")
ax.plot(run_x[0], run_y[0], "o", label="start")
ax.set_aspect("equal")
ax.set_xlabel("x [m]")
ax.set_ylabel("y [m]")
ax.legend()
ax.grid(True)
fig.savefig(sys.argv[3] if len(sys.argv) > 3 else "run.png", dpi=150)
