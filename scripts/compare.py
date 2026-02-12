#!/usr/bin/env python3
"""
soxim-compare: Compare multiple simulation runs

Compares results from different configurations or routing algorithms.
"""

import argparse
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path
import sys


def parse_arguments():
    """Parse command line arguments."""
    parser = argparse.ArgumentParser(
        description='Compare multiple simulation runs'
    )
    
    parser.add_argument('inputs', nargs='+',
                        help='TrafficInformation.csv files or directories to compare')
    parser.add_argument('-l', '--labels', nargs='+',
                        help='Labels for each run (default: file names)')
    parser.add_argument('-o', '--output',
                        help='Output plot file (PNG)')
    parser.add_argument('--latency-dist', action='store_true',
                        help='Compare latency distributions')
    
    return parser.parse_args()


def load_and_analyze(filepath):
    """Load CSV and calculate statistics."""
    df = pd.read_csv(filepath)

    # Convert column names to lowercase for consistency
    df.columns = df.columns.str.lower().str.replace(',', '')

    # Convert time columns to numeric
    df['senttime'] = pd.to_numeric(df['senttime'], errors='coerce')
    df['receivedtime'] = pd.to_numeric(df['receivedtime'], errors='coerce')

    df['latency'] = df['receivedtime'] - df['senttime']

    received = df[df['status'] == 'R']

    stats = {
        'total': len(df),
        'received': len(received),
        'received_pct': len(received) / len(df) * 100,
        'avg_latency': received['latency'].mean() if len(received) > 0 else 0,
        'latencies': received['latency'].values if len(received) > 0 else np.array([])
    }

    return stats


def main():
    args = parse_arguments()
    
    # Load all runs
    all_stats = []
    labels = args.labels or [Path(f).stem for f in args.inputs]
    
    for filepath, label in zip(args.inputs, labels):
        path = Path(filepath)
        if path.is_dir():
            path = path / 'TrafficInformation.csv'
        
        if not path.exists():
            print(f"Warning: File not found: {path}", file=sys.stderr)
            continue
        
        stats = load_and_analyze(path)
        stats['label'] = label
        all_stats.append(stats)
    
    if not all_stats:
        print("No valid data to compare", file=sys.stderr)
        sys.exit(1)
    
    # Print comparison table
    print("\n" + "="*80)
    print("COMPARISON RESULTS")
    print("="*80)
    print(f"{'Configuration':<20} {'Total':<10} {'Received':<10} {'%':<8} {'Avg Latency':<12}")
    print("-"*80)
    
    for stats in all_stats:
        print(f"{stats['label']:<20} {stats['total']:<10} {stats['received']:<10} "
              f"{stats['received_pct']:<8.1f} {stats['avg_latency']:<12.2f}")
    
    # Generate plots
    fig, axes = plt.subplots(1, 2, figsize=(14, 5))
    
    # Reception rate comparison
    labels_list = [s['label'] for s in all_stats]
    rates = [s['received_pct'] for s in all_stats]
    axes[0].bar(labels_list, rates, color='steelblue', edgecolor='black')
    axes[0].set_ylabel('Reception Rate (%)')
    axes[0].set_title('Packet Reception Rate Comparison')
    axes[0].set_ylim(0, 105)
    plt.setp(axes[0].xaxis.get_majorticklabels(), rotation=45, ha='right')
    
    # Average latency comparison
    latencies = [s['avg_latency'] for s in all_stats]
    axes[1].bar(labels_list, latencies, color='coral', edgecolor='black')
    axes[1].set_ylabel('Average Latency (cycles)')
    axes[1].set_title('Average Latency Comparison')
    plt.setp(axes[1].xaxis.get_majorticklabels(), rotation=45, ha='right')
    
    plt.tight_layout()
    
    if args.output:
        plt.savefig(args.output, dpi=150, bbox_inches='tight')
        print(f"\n📁 Comparison plot saved to: {args.output}")
    else:
        plt.show()


if __name__ == '__main__':
    main()
