#!/usr/bin/env python3
"""
soxim-visualize: Enhanced visualization for SOXIM simulation results

Generates comprehensive visualizations including:
- Saturation curves (throughput vs injection rate)
- Latency heatmaps
- Network topology visualization
- Performance comparison plots
- Contention analysis
"""

import argparse
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from pathlib import Path
import sys
import json
from collections import defaultdict


def parse_arguments():
    """Parse command line arguments."""
    parser = argparse.ArgumentParser(
        description='Enhanced visualization for SOXIM simulation results',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog='''
Examples:
  %(prog)s traffic/TrafficInformation.csv              # Basic visualization
  %(prog)s traffic/ --saturation-curve                 # Saturation analysis
  %(prog)s traffic/ --network-topology                 # Network topology
  %(prog)s traffic/ --contention-analysis              # Contention analysis
  %(prog)s traffic/ --compare results1/ results2/      # Compare multiple runs
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
    parser.add_argument('--network-topology', action='store_true',
                        help='Visualize network topology')
    parser.add_argument('--contention-analysis', action='store_true',
                        help='Analyze contention patterns')
    parser.add_argument('--compare', nargs='+',
                        help='Compare multiple simulation results')
    parser.add_argument('--stats-only', action='store_true',
                        help='Print statistics only, no plots')
    parser.add_argument('--save-data', action='store_true',
                        help='Save processed data to CSV')

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
        stats['p95_latency'] = received_df['latency'].quantile(0.95)
        stats['p99_latency'] = received_df['latency'].quantile(0.99)
    else:
        stats['avg_latency'] = 0
        stats['min_latency'] = 0
        stats['max_latency'] = 0
        stats['median_latency'] = 0
        stats['std_latency'] = 0
        stats['p95_latency'] = 0
        stats['p99_latency'] = 0

    # Traffic pattern analysis
    stats['unique_sources'] = df['source'].nunique()
    stats['unique_destinations'] = df['destination'].nunique()

    # Throughput per node
    if len(received_df) > 0:
        throughput_per_node = received_df['destination'].value_counts()
        stats['max_throughput_node'] = throughput_per_node.max()
        stats['min_throughput_node'] = throughput_per_node.min()
        stats['avg_throughput_per_node'] = throughput_per_node.mean()
    else:
        stats['max_throughput_node'] = 0
        stats['min_throughput_node'] = 0
        stats['avg_throughput_per_node'] = 0

    return stats


def print_statistics(stats):
    """Print formatted statistics."""
    print("\n" + "="*70)
    print("SOXIM SIMULATION RESULTS - ENHANCED ANALYSIS")
    print("="*70)

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
    print(f"  95th percentile:          {stats['p95_latency']:.2f} cycles")
    print(f"  99th percentile:          {stats['p99_latency']:.2f} cycles")

    print("\n🌐 NETWORK TRAFFIC:")
    print(f"  Unique sources:           {stats['unique_sources']}")
    print(f"  Unique destinations:      {stats['unique_destinations']}")
    print(f"  Max throughput/node:      {stats['max_throughput_node']}")
    print(f"  Min throughput/node:      {stats['min_throughput_node']}")
    print(f"  Avg throughput/node:      {stats['avg_throughput_per_node']:.1f}")

    print("\n" + "="*70)


def plot_latency_distribution(df, ax):
    """Plot latency distribution histogram."""
    received_df = df[df['status'] == 'R']

    if len(received_df) == 0:
        ax.text(0.5, 0.5, 'No received packets', ha='center', va='center', transform=ax.transAxes)
        return

    # Histogram
    ax.hist(received_df['latency'], bins=50, edgecolor='black', alpha=0.7, color='steelblue')
    ax.set_xlabel('Latency (cycles)')
    ax.set_ylabel('Number of Packets')
    ax.set_title('Latency Distribution')
    ax.axvline(received_df['latency'].mean(), color='red', linestyle='--',
               label=f'Mean: {received_df["latency"].mean():.1f}')
    ax.axvline(received_df['latency'].quantile(0.95), color='orange', linestyle='--',
               label=f'95th: {received_df["latency"].quantile(0.95):.1f}')
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


def plot_network_topology(df, ax):
    """Visualize network topology with throughput overlay."""
    received_df = df[df['status'] == 'R']

    if len(received_df) == 0:
        ax.text(0.5, 0.5, 'No received packets', ha='center', va='center', transform=ax.transAxes)
        return

    # Get unique nodes
    nodes = sorted(set(df['source'].unique()) | set(df['destination'].unique()))
    
    # Calculate throughput per node
    throughput = received_df['destination'].value_counts()
    
    # Create a simple grid visualization
    # For simplicity, we'll use a scatter plot with node positions
    n_nodes = len(nodes)
    grid_size = int(np.ceil(np.sqrt(n_nodes)))
    
    positions = {}
    for i, node in enumerate(nodes):
        row = i // grid_size
        col = i % grid_size
        positions[node] = (col, row)
    
    # Plot nodes
    for node in positions:
        x, y = positions[node]
        size = throughput.get(node, 0) * 10 + 100  # Scale by throughput
        color = plt.cm.viridis(throughput.get(node, 0) / throughput.max() if throughput.max() > 0 else 0)
        ax.scatter(x, y, s=size, c=[color], edgecolor='black', alpha=0.7)
        ax.text(x, y, str(node), ha='center', va='center', fontsize=8)
    
    ax.set_title('Network Topology (node size = throughput)')
    ax.set_xlabel('X Position')
    ax.set_ylabel('Y Position')
    ax.set_aspect('equal')


def plot_contention_analysis(df, ax):
    """Analyze contention patterns."""
    received_df = df[df['status'] == 'R']

    if len(received_df) == 0:
        ax.text(0.5, 0.5, 'No received packets', ha='center', va='center', transform=ax.transAxes)
        return

    # Calculate packets per time slot (contention indicator)
    time_bins = np.arange(0, df['senttime'].max() + 100, 100)
    packets_per_bin = []
    
    for i in range(len(time_bins) - 1):
        bin_start = time_bins[i]
        bin_end = time_bins[i + 1]
        bin_packets = len(df[(df['senttime'] >= bin_start) & (df['senttime'] < bin_end)])
        packets_per_bin.append(bin_packets)
    
    ax.plot(time_bins[:-1], packets_per_bin, marker='o', linewidth=2)
    ax.set_xlabel('Time (cycles)')
    ax.set_ylabel('Packets Generated per 100 cycles')
    ax.set_title('Contention Analysis (Packets vs Time)')
    ax.grid(True, alpha=0.3)


def plot_saturation_curve(df, ax):
    """Plot throughput vs injection rate (saturation curve)."""
    # This requires multiple runs with different injection rates
    # For now, we'll plot a simple throughput vs time
    received_df = df[df['status'] == 'R']

    if len(received_df) == 0:
        ax.text(0.5, 0.5, 'No received packets', ha='center', va='center', transform=ax.transAxes)
        return

    # Calculate cumulative throughput
    sorted_df = received_df.sort_values('receivedtime')
    cumulative = np.arange(1, len(sorted_df) + 1)
    
    ax.plot(sorted_df['receivedtime'], cumulative, linewidth=2)
    ax.set_xlabel('Time (cycles)')
    ax.set_ylabel('Cumulative Packets Received')
    ax.set_title('Throughput vs Time')
    ax.grid(True, alpha=0.3)


def plot_latency_cdf(df, ax):
    """Plot CDF of latency."""
    received_df = df[df['status'] == 'R']

    if len(received_df) == 0:
        ax.text(0.5, 0.5, 'No received packets', ha='center', va='center', transform=ax.transAxes)
        return

    latencies = np.sort(received_df['latency'])
    cdf = np.arange(1, len(latencies) + 1) / len(latencies)
    
    ax.plot(latencies, cdf, linewidth=2)
    ax.set_xlabel('Latency (cycles)')
    ax.set_ylabel('CDF')
    ax.set_title('Latency CDF')
    ax.grid(True, alpha=0.3)
    
    # Add percentile markers
    percentiles = [50, 95, 99]
    for p in percentiles:
        p_val = np.percentile(latencies, p)
        ax.axvline(p_val, color='red', linestyle='--', alpha=0.5)
        ax.text(p_val, 0.5, f'{p}th: {p_val:.0f}', rotation=90, va='center')


def plot_comparison(results, ax):
    """Compare multiple simulation results."""
    if not results:
        ax.text(0.5, 0.5, 'No results to compare', ha='center', va='center', transform=ax.transAxes)
        return

    # Extract metrics
    names = []
    throughputs = []
    latencies = []
    
    for name, (df, stats) in results.items():
        names.append(name)
        throughputs.append(stats['received_rate'])
        latencies.append(stats['avg_latency'])
    
    # Plot throughput comparison
    x = np.arange(len(names))
    width = 0.35
    
    ax.bar(x - width/2, throughputs, width, label='Throughput %', alpha=0.7)
    ax.bar(x + width/2, latencies, width, label='Avg Latency', alpha=0.7)
    
    ax.set_xlabel('Configuration')
    ax.set_ylabel('Metric Value')
    ax.set_title('Performance Comparison')
    ax.set_xticks(x)
    ax.set_xticklabels(names, rotation=45, ha='right')
    ax.legend()
    ax.grid(True, alpha=0.3)


def generate_report(df, output_path, args):
    """Generate comprehensive report with plots."""
    fig, axes = plt.subplots(3, 3, figsize=(18, 15))
    fig.suptitle('SOXIM Network-on-Chip Simulation Results', fontsize=16, fontweight='bold')

    plot_latency_distribution(df, axes[0, 0])
    plot_latency_vs_time(df, axes[0, 1])
    plot_throughput_per_node(df, axes[0, 2])
    plot_latency_heatmap(df, axes[1, 0])
    plot_network_topology(df, axes[1, 1])
    plot_contention_analysis(df, axes[1, 2])
    plot_saturation_curve(df, axes[2, 0])
    plot_latency_cdf(df, axes[2, 1])
    
    # Leave last subplot empty or add summary
    axes[2, 2].axis('off')
    stats = calculate_statistics(df)
    summary_text = f"""
    Summary Statistics
    
    Total Packets: {stats['total_packets']:,}
    Received Rate: {stats['received_rate']:.1f}%
    Avg Latency: {stats['avg_latency']:.1f} cycles
    Max Latency: {stats['max_latency']:.1f} cycles
    95th Percentile: {stats['p95_latency']:.1f} cycles
    99th Percentile: {stats['p99_latency']:.1f} cycles
    
    Network Stats
    -------------
    Unique Sources: {stats['unique_sources']}
    Unique Destinations: {stats['unique_destinations']}
    Max Throughput/Node: {stats['max_throughput_node']}
    Avg Throughput/Node: {stats['avg_throughput_per_node']:.1f}
    """
    axes[2, 2].text(0.1, 0.5, summary_text, fontsize=10, family='monospace',
                    verticalalignment='center', transform=axes[2, 2].transAxes)

    plt.tight_layout()

    if output_path:
        plt.savefig(output_path, dpi=150, bbox_inches='tight')
        print(f"\n📁 Plot saved to: {output_path}")
    else:
        plt.show()


def save_processed_data(df, output_path):
    """Save processed data to CSV."""
    output_file = Path(output_path).with_suffix('.processed.csv')
    df.to_csv(output_file, index=False)
    print(f"💾 Processed data saved to: {output_file}")


def main():
    args = parse_arguments()

    # Handle comparison mode
    if args.compare:
        results = {}
        for result_path in args.compare:
            path = Path(result_path)
            if path.is_dir():
                csv_path = path / 'TrafficInformation.csv'
            else:
                csv_path = path
            
            if csv_path.exists():
                name = path.name if path.is_dir() else path.stem
                df = load_traffic_data(csv_path)
                stats = calculate_statistics(df)
                results[name] = (df, stats)
            else:
                print(f"Warning: {csv_path} not found", file=sys.stderr)
        
        if results:
            fig, ax = plt.subplots(figsize=(12, 8))
            plot_comparison(results, ax)
            
            if args.output:
                plt.savefig(args.output, dpi=150, bbox_inches='tight')
                print(f"\n📁 Comparison plot saved to: {args.output}")
            else:
                plt.show()
            
            # Print comparison table
            print("\n" + "="*70)
            print("COMPARISON RESULTS")
            print("="*70)
            print(f"{'Configuration':<20} {'Throughput %':<15} {'Avg Latency':<15} {'Max Latency':<15}")
            print("-"*70)
            for name, (df, stats) in results.items():
                print(f"{name:<20} {stats['received_rate']:<15.2f} {stats['avg_latency']:<15.2f} {stats['max_latency']:<15.2f}")
            print("="*70)
        sys.exit(0)

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

    # Save processed data if requested
    if args.save_data and args.output:
        save_processed_data(df, args.output)

    if not args.stats_only:
        generate_report(df, args.output, args)


if __name__ == '__main__':
    main()
