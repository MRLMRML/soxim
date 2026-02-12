#!/usr/bin/env python3
"""
soxim-saturation: Saturation curve analysis

Generates saturation curves (throughput vs injection rate) and identifies
saturation points for different routing algorithms and traffic patterns.
"""

import argparse
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from pathlib import Path
import sys
import glob


def parse_arguments():
    """Parse command line arguments."""
    parser = argparse.ArgumentParser(
        description='Saturation curve analysis for SOXIM',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog='''
Examples:
  %(prog)s results/ --algorithm DOR                    # Single algorithm
  %(prog)s results/ --compare                          # Compare algorithms
  %(prog)s results/ --find-saturation                  # Find saturation points
        '''
    )

    parser.add_argument('input',
                        help='Directory containing simulation results')
    parser.add_argument('-o', '--output',
                        help='Output file for plot')
    parser.add_argument('--algorithm',
                        help='Filter by routing algorithm')
    parser.add_argument('--compare', action='store_true',
                        help='Compare multiple algorithms')
    parser.add_argument('--find-saturation', action='store_true',
                        help='Find saturation points')
    parser.add_argument('--traffic-pattern',
                        help='Filter by traffic pattern')
    parser.add_argument('--injection-rates', nargs='+', type=float,
                        help='Specific injection rates to analyze')
    parser.add_argument('--save-data', action='store_true',
                        help='Save processed data to CSV')

    return parser.parse_args()


def load_simulation_results(results_dir):
    """Load simulation results from directory."""
    results = []
    
    # Find all TrafficInformation.csv files
    csv_files = list(Path(results_dir).rglob('TrafficInformation.csv'))
    
    for csv_file in csv_files:
        # Extract parameters from path
        # Expected structure: results/{algorithm}/{pattern}/{rate}/TrafficInformation.csv
        parts = csv_file.parts
        
        # Try to extract algorithm, pattern, and rate from path
        algorithm = None
        pattern = None
        rate = None
        
        # Look for algorithm in path
        for part in parts:
            if part in ['DOR', 'ROMM', 'MAD', 'VAL', 'ODD_EVEN']:
                algorithm = part
            elif part in ['random uniform', 'permutation', 'random', 'permutation']:
                pattern = part
            elif 'rate' in part.lower() or 'injection' in part.lower():
                # Try to extract rate value
                try:
                    rate = float(part.replace('rate', '').replace('injection', '').replace('_', ''))
                except:
                    pass
        
        # If not found in path, try to extract from config file
        if algorithm is None or pattern is None or rate is None:
            config_file = csv_file.parent / 'config.toml'
            if config_file.exists():
                try:
                    with open(config_file, 'r') as f:
                        content = f.read()
                        if 'DOR' in content:
                            algorithm = 'DOR'
                        elif 'ROMM' in content:
                            algorithm = 'ROMM'
                        elif 'MAD' in content:
                            algorithm = 'MAD'
                        elif 'VAL' in content:
                            algorithm = 'VAL'
                        elif 'ODD_EVEN' in content:
                            algorithm = 'ODD_EVEN'
                        
                        if 'random uniform' in content:
                            pattern = 'random uniform'
                        elif 'permutation' in content:
                            pattern = 'permutation'
                        
                        # Extract injection rate
                        import re
                        rate_match = re.search(r'injectionRate\s*=\s*([0-9.]+)', content)
                        if rate_match:
                            rate = float(rate_match.group(1))
                except:
                    pass
        
        # Load data
        try:
            df = pd.read_csv(csv_file)
            df.columns = df.columns.str.lower().str.replace(',', '')
            df['status'] = df['status'].astype('category')
            df['senttime'] = pd.to_numeric(df['senttime'], errors='coerce')
            df['receivedtime'] = pd.to_numeric(df['receivedtime'], errors='coerce')
            df['latency'] = df['receivedtime'] - df['senttime']
            
            # Calculate metrics
            total_packets = len(df)
            received_packets = len(df[df['status'] == 'R'])
            throughput = received_packets / total_packets * 100 if total_packets > 0 else 0
            
            received_df = df[df['status'] == 'R']
            avg_latency = received_df['latency'].mean() if len(received_df) > 0 else 0
            max_latency = received_df['latency'].max() if len(received_df) > 0 else 0
            
            results.append({
                'file': str(csv_file),
                'algorithm': algorithm or 'Unknown',
                'pattern': pattern or 'Unknown',
                'rate': rate if rate is not None else 0,
                'throughput': throughput,
                'avg_latency': avg_latency,
                'max_latency': max_latency,
                'received_packets': received_packets,
                'total_packets': total_packets
            })
            
        except Exception as e:
            print(f"Warning: Could not load {csv_file}: {e}", file=sys.stderr)
    
    return pd.DataFrame(results)


def plot_saturation_curve(df, ax, algorithm=None, pattern=None):
    """Plot throughput vs injection rate."""
    # Filter data
    if algorithm:
        df = df[df['algorithm'] == algorithm]
    if pattern:
        df = df[df['pattern'] == pattern]
    
    if len(df) == 0:
        ax.text(0.5, 0.5, 'No data to plot', ha='center', va='center', transform=ax.transAxes)
        return
    
    # Sort by injection rate
    df = df.sort_values('rate')
    
    # Plot throughput
    ax.plot(df['rate'], df['throughput'], marker='o', linewidth=2, label=f'{algorithm} {pattern}')
    ax.set_xlabel('Injection Rate')
    ax.set_ylabel('Throughput (%)')
    ax.set_title('Saturation Curve: Throughput vs Injection Rate')
    ax.grid(True, alpha=0.3)
    ax.legend()


def plot_comparison(df, ax):
    """Compare multiple algorithms."""
    if len(df) == 0:
        ax.text(0.5, 0.5, 'No data to plot', ha='center', va='center', transform=ax.transAxes)
        return
    
    # Get unique algorithms
    algorithms = df['algorithm'].unique()
    
    # Plot each algorithm
    for algorithm in algorithms:
        algo_df = df[df['algorithm'] == algorithm]
        algo_df = algo_df.sort_values('rate')
        ax.plot(algo_df['rate'], algo_df['throughput'], marker='o', linewidth=2, label=algorithm)
    
    ax.set_xlabel('Injection Rate')
    ax.set_ylabel('Throughput (%)')
    ax.set_title('Algorithm Comparison: Throughput vs Injection Rate')
    ax.grid(True, alpha=0.3)
    ax.legend()


def plot_latency_comparison(df, ax):
    """Compare latency across algorithms."""
    if len(df) == 0:
        ax.text(0.5, 0.5, 'No data to plot', ha='center', va='center', transform=ax.transAxes)
        return
    
    # Get unique algorithms
    algorithms = df['algorithm'].unique()
    
    # Plot each algorithm
    for algorithm in algorithms:
        algo_df = df[df['algorithm'] == algorithm]
        algo_df = algo_df.sort_values('rate')
        ax.plot(algo_df['rate'], algo_df['avg_latency'], marker='s', linewidth=2, label=algorithm)
    
    ax.set_xlabel('Injection Rate')
    ax.set_ylabel('Average Latency (cycles)')
    ax.set_title('Latency Comparison: Latency vs Injection Rate')
    ax.grid(True, alpha=0.3)
    ax.legend()


def find_saturation_points(df):
    """Find saturation points for each algorithm."""
    saturation_points = {}
    
    algorithms = df['algorithm'].unique()
    
    for algorithm in algorithms:
        algo_df = df[df['algorithm'] == algorithm]
        algo_df = algo_df.sort_values('rate')
        
        if len(algo_df) < 2:
            continue
        
        # Find saturation point (where throughput starts to plateau or decrease)
        throughputs = algo_df['throughput'].values
        rates = algo_df['rate'].values
        
        # Calculate derivative
        derivatives = np.diff(throughputs) / np.diff(rates)
        
        # Find where derivative drops significantly
        # Saturation point is where throughput reaches 95% of max
        max_throughput = np.max(throughputs)
        threshold = max_throughput * 0.95
        
        saturation_rate = None
        for i, throughput in enumerate(throughputs):
            if throughput >= threshold:
                saturation_rate = rates[i]
                break
        
        if saturation_rate is not None:
            saturation_points[algorithm] = {
                'saturation_rate': saturation_rate,
                'max_throughput': max_throughput,
                'saturation_latency': algo_df[algo_df['rate'] == saturation_rate]['avg_latency'].values[0] if len(algo_df[algo_df['rate'] == saturation_rate]) > 0 else None
            }
    
    return saturation_points


def print_saturation_analysis(df):
    """Print saturation analysis results."""
    print("\n" + "="*70)
    print("SATURATION ANALYSIS")
    print("="*70)
    
    algorithms = df['algorithm'].unique()
    
    print(f"\n{'Algorithm':<15} {'Max Throughput':<15} {'Saturation Rate':<15} {'Latency at Sat':<15}")
    print("-"*70)
    
    for algorithm in algorithms:
        algo_df = df[df['algorithm'] == algorithm]
        algo_df = algo_df.sort_values('rate')
        
        if len(algo_df) == 0:
            continue
        
        max_throughput = algo_df['throughput'].max()
        max_rate = algo_df.loc[algo_df['throughput'].idxmax(), 'rate']
        latency_at_max = algo_df.loc[algo_df['throughput'].idxmax(), 'avg_latency']
        
        print(f"{algorithm:<15} {max_throughput:<15.2f} {max_rate:<15.3f} {latency_at_max:<15.2f}")
    
    print("="*70)
    
    # Find saturation points
    saturation_points = find_saturation_points(df)
    
    if saturation_points:
        print("\n" + "="*70)
        print("SATURATION POINTS")
        print("="*70)
        print(f"\n{'Algorithm':<15} {'Saturation Rate':<15} {'Max Throughput':<15} {'Latency':<15}")
        print("-"*70)
        
        for algorithm, info in saturation_points.items():
            print(f"{algorithm:<15} {info['saturation_rate']:<15.3f} {info['max_throughput']:<15.2f} {info['saturation_latency']:<15.2f}")
        
        print("="*70)


def save_processed_data(df, output_path):
    """Save processed data to CSV."""
    output_file = Path(output_path).with_suffix('.saturation.csv')
    df.to_csv(output_file, index=False)
    print(f"\n💾 Processed data saved to: {output_file}")


def main():
    args = parse_arguments()
    
    # Load results
    print(f"Loading results from: {args.input}")
    df = load_simulation_results(args.input)
    
    if len(df) == 0:
        print("Error: No results found", file=sys.stderr)
        sys.exit(1)
    
    print(f"Loaded {len(df)} simulation results")
    
    # Filter by algorithm if specified
    if args.algorithm:
        df = df[df['algorithm'] == args.algorithm]
        if len(df) == 0:
            print(f"No results for algorithm: {args.algorithm}", file=sys.stderr)
            sys.exit(1)
    
    # Filter by traffic pattern if specified
    if args.traffic_pattern:
        df = df[df['pattern'] == args.traffic_pattern]
        if len(df) == 0:
            print(f"No results for pattern: {args.traffic_pattern}", file=sys.stderr)
            sys.exit(1)
    
    # Filter by injection rates if specified
    if args.injection_rates:
        df = df[df['rate'].isin(args.injection_rates)]
        if len(df) == 0:
            print(f"No results for specified injection rates", file=sys.stderr)
            sys.exit(1)
    
    # Print saturation analysis
    print_saturation_analysis(df)
    
    # Save processed data if requested
    if args.save_data and args.output:
        save_processed_data(df, args.output)

    # Generate plots
    if args.output or args.compare or args.find_saturation:
        if args.compare:
            # Compare algorithms
            fig, axes = plt.subplots(1, 2, figsize=(14, 6))
            
            plot_comparison(df, axes[0])
            plot_latency_comparison(df, axes[1])
            
            plt.tight_layout()
        else:
            # Single saturation curve
            fig, ax = plt.subplots(figsize=(10, 6))
            plot_saturation_curve(df, ax, args.algorithm, args.traffic_pattern)
        
        if args.output:
            plt.savefig(args.output, dpi=150, bbox_inches='tight')
            print(f"\n📁 Plot saved to: {args.output}")
        else:
            plt.show()


if __name__ == '__main__':
    main()
