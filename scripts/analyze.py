#!/usr/bin/env python3
"""
soxim-analyze: Analyze soxim simulation results

Parses TrafficInformation.csv and generates performance reports and visualizations.
"""

import argparse
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from pathlib import Path
import sys


def parse_arguments():
    """Parse command line arguments."""
    parser = argparse.ArgumentParser(
        description='Analyze soxim simulation results',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog='''
Examples:
  %(prog)s traffic/TrafficInformation.csv              # Basic analysis
  %(prog)s traffic/ -o results.png                     # Generate plots
  %(prog)s traffic/ --saturation-curve                 # Saturation analysis
        '''
    )
    
    parser.add_argument('input', 
                        help='Path to TrafficInformation.csv or directory containing it')
    parser.add_argument('-o', '--output', 
                        help='Output file for plots (PNG format)')
    parser.add_argument('--saturation-curve', action='store_true',
                        help='Generate saturation curve (throughput vs injection rate)')
    parser.add_argument('--latency-plot', action='store_true',
                        help='Generate latency vs time plot')
    parser.add_argument('--heatmap', action='store_true',
                        help='Generate latency heatmap by source-destination pairs')
    parser.add_argument('--stats-only', action='store_true',
                        help='Print statistics only, no plots')
    
    return parser.parse_args()


def load_traffic_data(filepath):
    """Load and parse TrafficInformation.csv."""
    df = pd.read_csv(filepath)

    # Convert column names to lowercase for consistency
    df.columns = df.columns.str.lower().str.replace(',', '')

    # Convert status to categorical
    df['status'] = df['status'].astype('category')

    # Convert time columns to numeric (handle floating point)
    df['senttime'] = pd.to_numeric(df['senttime'], errors='coerce')
    df['receivedtime'] = pd.to_numeric(df['receivedtime'], errors='coerce')

    # Calculate latency for received packets
    df['latency'] = df['receivedtime'] - df['senttime']

    return df


def calculate_statistics(df):
    """Calculate key performance metrics."""
    stats = {}
    
    # Packet statistics
    total_packets = len(df)
    received_packets = len(df[df['status'] == 'R'])
    sent_packets = len(df[df['status'] == 'S'])
    
    stats['total_packets'] = total_packets
    stats['received_packets'] = received_packets
    stats['sent_packets'] = sent_packets
    stats['received_rate'] = received_packets / total_packets * 100 if total_packets > 0 else 0
    
    # Latency statistics (for received packets)
    received_df = df[df['status'] == 'R']
    if len(received_df) > 0:
        stats['avg_latency'] = received_df['latency'].mean()
        stats['min_latency'] = received_df['latency'].min()
        stats['max_latency'] = received_df['latency'].max()
        stats['median_latency'] = received_df['latency'].median()
        stats['std_latency'] = received_df['latency'].std()
    else:
        stats['avg_latency'] = 0
        stats['min_latency'] = 0
        stats['max_latency'] = 0
        stats['median_latency'] = 0
        stats['std_latency'] = 0
    
    # Traffic pattern analysis
    stats['unique_sources'] = df['source'].nunique()
    stats['unique_destinations'] = df['destination'].nunique()
    
    return stats


def print_statistics(stats):
    """Print formatted statistics."""
    print("\n" + "="*60)
    print("SOXIM SIMULATION RESULTS")
    print("="*60)
    
    print("\n📊 PACKET STATISTICS:")
    print(f"  Total packets generated:  {stats['total_packets']:,}")
    print(f"  Packets sent:             {stats['sent_packets']:,}")
    print(f"  Packets received:         {stats['received_packets']:,}")
    print(f"  Reception rate:           {stats['received_rate']:.2f}%")
    
    print("\n⏱️  LATENCY STATISTICS:")
    print(f"  Average latency:          {stats['avg_latency']:.2f} cycles")
    print(f"  Min latency:              {stats['min_latency']:.2f} cycles")
    print(f"  Max latency:              {stats['max_latency']:.2f} cycles")
    print(f"  Median latency:           {stats['median_latency']:.2f} cycles")
    print(f"  Std deviation:            {stats['std_latency']:.2f} cycles")
    
    print("\n🌐 NETWORK TRAFFIC:")
    print(f"  Unique sources:           {stats['unique_sources']}")
    print(f"  Unique destinations:      {stats['unique_destinations']}")
    
    print("\n" + "="*60)


def plot_latency_distribution(df, ax):
    """Plot latency distribution histogram."""
    received_df = df[df['status'] == 'R']
    
    if len(received_df) == 0:
        ax.text(0.5, 0.5, 'No received packets', ha='center', va='center', transform=ax.transAxes)
        return
    
    ax.hist(received_df['latency'], bins=50, edgecolor='black', alpha=0.7, color='steelblue')
    ax.set_xlabel('Latency (cycles)')
    ax.set_ylabel('Number of Packets')
    ax.set_title('Latency Distribution')
    ax.axvline(received_df['latency'].mean(), color='red', linestyle='--', 
               label=f'Mean: {received_df["latency"].mean():.1f}')
    ax.legend()


def plot_latency_vs_time(df, ax):
    """Plot latency vs sent time."""
    received_df = df[df['status'] == 'R']
    
    if len(received_df) == 0:
        ax.text(0.5, 0.5, 'No received packets', ha='center', va='center', transform=ax.transAxes)
        return
    
    ax.scatter(received_df['senttime'], received_df['latency'], alpha=0.5, s=10)
    ax.set_xlabel('Sent Time (cycles)')
    ax.set_ylabel('Latency (cycles)')
    ax.set_title('Latency vs Time')


def plot_throughput_per_node(df, ax):
    """Plot packets received per destination node."""
    received_df = df[df['status'] == 'R']
    
    if len(received_df) == 0:
        ax.text(0.5, 0.5, 'No received packets', ha='center', va='center', transform=ax.transAxes)
        return
    
    dest_counts = received_df['destination'].value_counts().sort_index()
    ax.bar(dest_counts.index, dest_counts.values, edgecolor='black', alpha=0.7)
    ax.set_xlabel('Destination Node')
    ax.set_ylabel('Packets Received')
    ax.set_title('Throughput per Node')


def plot_latency_heatmap(df, ax):
    """Plot latency heatmap by source-destination pairs."""
    received_df = df[df['status'] == 'R']
    
    if len(received_df) == 0:
        ax.text(0.5, 0.5, 'No received packets', ha='center', va='center', transform=ax.transAxes)
        return
    
    pivot = received_df.pivot_table(values='latency', index='source', columns='destination', aggfunc='mean')
    sns.heatmap(pivot, ax=ax, cmap='viridis', cbar_kws={'label': 'Latency (cycles)'})
    ax.set_title('Average Latency Heatmap')
    ax.set_xlabel('Destination')
    ax.set_ylabel('Source')


def generate_report(df, output_path):
    """Generate comprehensive report with plots."""
    fig, axes = plt.subplots(2, 2, figsize=(14, 10))
    fig.suptitle('SOXIM Network-on-Chip Simulation Results', fontsize=16, fontweight='bold')
    
    plot_latency_distribution(df, axes[0, 0])
    plot_latency_vs_time(df, axes[0, 1])
    plot_throughput_per_node(df, axes[1, 0])
    plot_latency_heatmap(df, axes[1, 1])
    
    plt.tight_layout()
    
    if output_path:
        plt.savefig(output_path, dpi=150, bbox_inches='tight')
        print(f"\n📁 Plot saved to: {output_path}")
    else:
        plt.show()


def main():
    args = parse_arguments()
    
    # Determine input file path
    input_path = Path(args.input)
    if input_path.is_dir():
        csv_path = input_path / 'TrafficInformation.csv'
    else:
        csv_path = input_path
    
    if not csv_path.exists():
        print(f"Error: File not found: {csv_path}", file=sys.stderr)
        sys.exit(1)
    
    # Load data
    print(f"Loading data from: {csv_path}")
    df = load_traffic_data(csv_path)
    
    # Calculate and print statistics
    stats = calculate_statistics(df)
    print_statistics(stats)
    
    if not args.stats_only:
        generate_report(df, args.output)


if __name__ == '__main__':
    main()
