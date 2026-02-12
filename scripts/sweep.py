#!/usr/bin/env python3
"""
soxim-sweep: Run parameter sweeps and generate saturation curves

Automates multiple simulation runs with varying injection rates to generate
throughput vs injection rate curves (saturation analysis).
"""

import argparse
import subprocess
import json
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path
import tempfile
import shutil
import sys


def parse_arguments():
    """Parse command line arguments."""
    parser = argparse.ArgumentParser(
        description='Run parameter sweeps and generate saturation curves',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog='''
Examples:
  %(prog)s -c configs/example.toml --rates 0.01 0.05 0.1 0.2 0.3  # Sweep rates
  %(prog)s -c configs/example.toml --min-rate 0.01 --max-rate 0.5 -n 10  # Auto rates
  %(prog)s -c configs/example.toml --output saturation.png         # Save plot
        '''
    )
    
    parser.add_argument('-c', '--config', required=True,
                        help='Base configuration file')
    parser.add_argument('--rates', nargs='+', type=float,
                        help='List of injection rates to test')
    parser.add_argument('--min-rate', type=float, default=0.01,
                        help='Minimum injection rate (if --rates not specified)')
    parser.add_argument('--max-rate', type=float, default=0.5,
                        help='Maximum injection rate (if --rates not specified)')
    parser.add_argument('-n', '--num-points', type=int, default=10,
                        help='Number of rate points (if --rates not specified)')
    parser.add_argument('-o', '--output',
                        help='Output plot file (PNG)')
    parser.add_argument('--soxim-path', default='./soxim',
                        help='Path to soxim executable')
    parser.add_argument('--keep-raw', action='store_true',
                        help='Keep raw simulation outputs')
    
    return parser.parse_args()


def modify_config(base_config, injection_rate, output_path):
    """Create a modified config with specified injection rate."""
    with open(base_config, 'r') as f:
        lines = f.readlines()
    
    # Modify injection_rate line
    modified_lines = []
    for line in lines:
        if 'injection_rate' in line and '=' in line:
            # Replace the value
            parts = line.split('=')
            modified_lines.append(f"injection_rate = {injection_rate}\n")
        else:
            modified_lines.append(line)
    
    with open(output_path, 'w') as f:
        f.writelines(modified_lines)


def parse_simulation_output(output_text):
    """Parse simulation output to extract metrics."""
    results = {}
    
    for line in output_text.split('\n'):
        if 'Throughput:' in line:
            try:
                results['throughput'] = float(line.split(':')[1].split()[0])
            except:
                pass
        elif 'Average latency:' in line:
            try:
                results['latency'] = float(line.split(':')[1].split()[0])
            except:
                pass
        elif 'Demand:' in line:
            try:
                results['demand'] = float(line.split(':')[1].split()[0])
            except:
                pass
    
    return results


def run_simulation(soxim_path, config_path, work_dir):
    """Run soxim simulation and return results."""
    cmd = [soxim_path, str(config_path)]
    
    try:
        result = subprocess.run(
            cmd,
            cwd=work_dir,
            capture_output=True,
            text=True,
            timeout=300  # 5 minute timeout
        )
        
        if result.returncode != 0:
            print(f"Warning: Simulation failed: {result.stderr}", file=sys.stderr)
            return None
        
        return parse_simulation_output(result.stdout)
        
    except subprocess.TimeoutExpired:
        print(f"Warning: Simulation timed out", file=sys.stderr)
        return None
    except Exception as e:
        print(f"Error running simulation: {e}", file=sys.stderr)
        return None


def plot_saturation_curve(results_df, output_path):
    """Generate saturation curve plot."""
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5))
    
    # Throughput vs Injection Rate
    ax1.plot(results_df['injection_rate'], results_df['throughput'], 
             'bo-', linewidth=2, markersize=8)
    ax1.plot([0, results_df['injection_rate'].max()], 
             [0, results_df['injection_rate'].max()], 
             'k--', alpha=0.5, label='Ideal (throughput = injection rate)')
    ax1.set_xlabel('Injection Rate (packets/cycle)', fontsize=12)
    ax1.set_ylabel('Throughput (flits/cycle/node)', fontsize=12)
    ax1.set_title('Saturation Curve', fontsize=14, fontweight='bold')
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    # Latency vs Injection Rate
    ax2.plot(results_df['injection_rate'], results_df['latency'], 
             'ro-', linewidth=2, markersize=8)
    ax2.set_xlabel('Injection Rate (packets/cycle)', fontsize=12)
    ax2.set_ylabel('Average Latency (cycles)', fontsize=12)
    ax2.set_title('Latency vs Injection Rate', fontsize=14, fontweight='bold')
    ax2.grid(True, alpha=0.3)
    
    plt.tight_layout()
    
    if output_path:
        plt.savefig(output_path, dpi=150, bbox_inches='tight')
        print(f"\n📁 Saturation curve saved to: {output_path}")
    else:
        plt.show()


def main():
    args = parse_arguments()
    
    # Determine injection rates to test
    if args.rates:
        rates = args.rates
    else:
        rates = np.linspace(args.min_rate, args.max_rate, args.num_points)
    
    print(f"Running {len(rates)} simulations with injection rates: {rates}")
    print("="*60)
    
    # Create temporary directory for runs
    with tempfile.TemporaryDirectory() as tmpdir:
        work_dir = Path(tmpdir)
        
        results = []
        
        for i, rate in enumerate(rates, 1):
            print(f"\n[{i}/{len(rates)}] Testing injection rate: {rate:.4f}")
            
            # Create modified config
            config_path = work_dir / f'config_{rate:.4f}.toml'
            modify_config(args.config, rate, config_path)
            
            # Run simulation
            sim_results = run_simulation(args.soxim_path, config_path, work_dir)
            
            if sim_results:
                results.append({
                    'injection_rate': rate,
                    'throughput': sim_results.get('throughput', 0),
                    'latency': sim_results.get('latency', 0),
                    'demand': sim_results.get('demand', 0)
                })
                print(f"  ✓ Throughput: {sim_results.get('throughput', 0):.4f}, "
                      f"Latency: {sim_results.get('latency', 0):.2f}")
            else:
                print(f"  ✗ Simulation failed")
        
        if not results:
            print("\n❌ No successful simulations", file=sys.stderr)
            sys.exit(1)
        
        # Create DataFrame
        results_df = pd.DataFrame(results)
        
        # Print summary table
        print("\n" + "="*60)
        print("RESULTS SUMMARY")
        print("="*60)
        print(f"{'Injection Rate':<15} {'Throughput':<12} {'Latency':<12}")
        print("-"*60)
        for _, row in results_df.iterrows():
            print(f"{row['injection_rate']:<15.4f} {row['throughput']:<12.4f} {row['latency']:<12.2f}")
        
        # Save CSV
        csv_path = Path(args.output).with_suffix('.csv') if args.output else 'saturation_results.csv'
        results_df.to_csv(csv_path, index=False)
        print(f"\n📊 Results saved to: {csv_path}")
        
        # Generate plot
        plot_saturation_curve(results_df, args.output)


if __name__ == '__main__':
    main()
