from __future__ import annotations

import argparse
from pathlib import Path

import matplotlib.pyplot as plt
import pandas as pd


ALGO_ORDER = [
    "std_quicksort",
    "std_mergesort",
    "string_quicksort_3way",
    "string_mergesort_lcp",
    "msd_radix_sort",
    "msd_radix_quick_sort",
]


def plot_metric(df, array_type, metric, ylabel, out_path, log_y=False):
    sub = df[df["array_type"] == array_type]
    if sub.empty:
        return

    fig, ax = plt.subplots(figsize=(9, 6))
    for algo in ALGO_ORDER:
        d = sub[sub["algorithm"] == algo].sort_values("n")
        if d.empty:
            continue
        ax.plot(d["n"], d[metric], marker="o", markersize=3, label=algo)

    ax.set_title(f"{ylabel}, array_type: {array_type}")
    ax.set_xlabel("n")
    ax.set_ylabel(ylabel)
    if log_y:
        ax.set_yscale("log")
    ax.grid(True, alpha=0.3)
    ax.legend(loc="best", fontsize=9)
    fig.tight_layout()
    fig.savefig(out_path, dpi=130)
    plt.close(fig)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--csv", default="data/results.csv")
    ap.add_argument("--out", default="plots")
    args = ap.parse_args()

    csv_path = Path(args.csv)
    out_dir = Path(args.out)
    out_dir.mkdir(parents=True, exist_ok=True)

    df = pd.read_csv(csv_path)
    array_types = sorted(df["array_type"].unique())

    for at in array_types:
        plot_metric(df, at, "time_ns_median",
                    "time, ns (median)",
                    out_dir / f"time_{at}.png", log_y=True)
        plot_metric(df, at, "char_comparisons",
                    "char comparisons",
                    out_dir / f"cmp_{at}.png", log_y=True)

    print(f"Saved plots to {out_dir.resolve()}")


if __name__ == "__main__":
    main()
