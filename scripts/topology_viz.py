#!/usr/bin/env python3
"""
soxim-topology: Network topology visualization

Visualizes network topology with routing paths and performance metrics.
"""

import argparse
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from pathlib import Path
import sys
import json


def parse_arguments():
    """Parse command line arguments."""
    parser = argparse.ArgumentParser(
        description='Network topology visualization for SOXIM',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog='''
Examples:
  %(prog)s --topology MESH --x 4 --y 4              # 4x4 Mesh
  %(prog)s --topology TORUS --x 4 --y 4             # 4x4 Torus
  %(prog)s traffic/TrafficInformation.csv --topology MESH --x 4 --y 4
        '''
    )

    parser.add_argument('--topology', choices=['MESH', 'TORUS'], default='MESH',
                        help='Network topology type')
    parser.add_argument('--x', type=int, default=4, help='X dimension')
    parser.add_argument('--y', type=int, default=4, help='Y dimension')
    parser.add_argument('--z', type=int, default=1, help='Z dimension (3D)')
    parser.add_argument('--input', help='Traffic data file for overlay')
    parser.add_argument('-o', '--output', help='Output file for plot')
    parser.add_argument('--show-routing', action='store_true',
                        help='Show routing paths from traffic data')
    parser.add_argument('--highlight-nodes', nargs='+', type=int,
                        help='Highlight specific nodes')
    parser.add_argument('--show-labels', action='store_true',
                        help='Show node labels')

    return parser.parse_args()


def load_traffic_data(filepath):
    """Load and parse TrafficInformation.csv."""
    df = pd.read_csv(filepath)

    # Convert column names to lowercase for consistency
    df.columns = df.columns.str.lower().str.replace(',', '')

    # Convert status to categorical
    df['status'] = df['status'].astype('category')

    return df


def plot_mesh_topology(ax, x_dim, y_dim, z_dim=1):
    """Plot 2D/3D mesh topology."""
    if z_dim > 1:
        plot_3d_mesh(ax, x_dim, y_dim, z_dim)
    else:
        plot_2d_mesh(ax, x_dim, y_dim)


def plot_2d_mesh(ax, x_dim, y_dim):
    """Plot 2D mesh topology."""
    # Plot nodes
    for y in range(y_dim):
        for x in range(x_dim):
            node_id = y * x_dim + x
            ax.scatter(x, y, s=300, c='lightblue', edgecolor='black', zorder=2)
            ax.text(x, y, str(node_id), ha='center', va='center', fontsize=8, zorder=3)

    # Plot links (horizontal)
    for y in range(y_dim):
        for x in range(x_dim - 1):
            ax.plot([x, x + 1], [y, y], 'k-', alpha=0.3, linewidth=1, zorder=1)

    # Plot links (vertical)
    for x in range(x_dim):
        for y in range(y_dim - 1):
            ax.plot([x, x], [y, y + 1], 'k-', alpha=0.3, linewidth=1, zorder=1)

    # Add torus links if requested
    if hasattr(ax, 'is_torus') and ax.is_torus:
        # Horizontal wrap-around
        for y in range(y_dim):
            ax.plot([0, x_dim - 1], [y, y], 'k--', alpha=0.3, linewidth=1, zorder=1)
        # Vertical wrap-around
        for x in range(x_dim):
            ax.plot([x, x], [0, y_dim - 1], 'k--', alpha=0.3, linewidth=1, zorder=1)

    ax.set_xlim(-0.5, x_dim - 0.5)
    ax.set_ylim(-0.5, y_dim - 0.5)
    ax.set_aspect('equal')
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_title(f'{x_dim}x{y_dim} Mesh Topology')
    ax.grid(True, alpha=0.3)


def plot_3d_mesh(ax, x_dim, y_dim, z_dim):
    """Plot 3D mesh topology (simplified 2D projection)."""
    # For 3D, we'll show layers side by side
    fig = ax.figure
    axes = []
    
    for z in range(z_dim):
        # Create sub-axes for each layer
        if z == 0:
            axes.append(ax)
        else:
            axes.append(fig.add_subplot(1, z_dim, z + 1))
    
    for z, layer_ax in enumerate(axes):
        # Plot nodes for this layer
        for y in range(y_dim):
            for x in range(x_dim):
                node_id = z * x_dim * y_dim + y * x_dim + x
                layer_ax.scatter(x, y, s=200, c='lightblue', edgecolor='black', zorder=2)
                layer_ax.text(x, y, str(node_id), ha='center', va='center', fontsize=7, zorder=3)

        # Plot links (horizontal)
        for y in range(y_dim):
            for x in range(x_dim - 1):
                layer_ax.plot([x, x + 1], [y, y], 'k-', alpha=0.3, linewidth=1, zorder=1)

        # Plot links (vertical)
        for x in range(x_dim):
            for y in range(y_dim - 1):
                layer_ax.plot([x, x], [y, y + 1], 'k-', alpha=0.3, linewidth=1, zorder=1)

        layer_ax.set_xlim(-0.5, x_dim - 0.5)
        layer_ax.set_ylim(-0.5, y_dim - 0.5)
        layer_ax.set_aspect('equal')
        layer_ax.set_xlabel('X')
        layer_ax.set_ylabel('Y')
        layer_ax.set_title(f'Layer {z}')
        layer_ax.grid(True, alpha=0.3)


def plot_torus_topology(ax, x_dim, y_dim):
    """Plot torus topology."""
    ax.is_torus = True
    plot_2d_mesh(ax, x_dim, y_dim)
    ax.set_title(f'{x_dim}x{y_dim} Torus Topology')


def plot_traffic_overlay(ax, df, x_dim, y_dim, z_dim=1):
    """Plot traffic overlay on topology."""
    if df is None or len(df) == 0:
        return

    # Filter received packets
    received_df = df[df['status'] == 'R']
    if len(received_df) == 0:
        return

    # Calculate throughput per node
    throughput = received_df['destination'].value_counts()
    
    # Plot nodes with size based on throughput
    for y in range(y_dim):
        for x in range(x_dim):
            node_id = y * x_dim + x
            if node_id in throughput.index:
                size = throughput[node_id] * 5 + 100
                color = plt.cm.viridis(throughput[node_id] / throughput.max())
                ax.scatter(x, y, s=size, c=[color], edgecolor='black', zorder=2, alpha=0.7)
            else:
                ax.scatter(x, y, s=100, c='lightgray', edgecolor='black', zorder=2, alpha=0.5)


def plot_routing_paths(ax, df, x_dim, y_dim):
    """Plot routing paths from traffic data."""
    if df is None or len(df) == 0:
        return

    # Sample a few paths to avoid clutter
    sample_df = df[df['status'] == 'R'].sample(n=min(20, len(df)), random_state=42)
    
    for _, row in sample_df.iterrows():
        src = row['source']
        dst = row['destination']
        
        # Convert node IDs to coordinates
        src_x = src % x_dim
        src_y = src // x_dim
        dst_x = dst % x_dim
        dst_y = dst // x_dim
        
        # Plot path (simple straight line for visualization)
        ax.plot([src_x, dst_x], [src_y, dst_y], 'r-', alpha=0.3, linewidth=1, zorder=1)
        
        # Mark source and destination
        ax.scatter(src_x, src_y, s=200, c='green', edgecolor='black', zorder=3)
        ax.scatter(dst_x, dst_y, s=200, c='red', edgecolor='black', zorder=3)


def plot_highlighted_nodes(ax, nodes, x_dim, y_dim):
    """Highlight specific nodes."""
    for node in nodes:
        if 0 <= node < x_dim * y_dim:
            x = node % x_dim
            y = node // x_dim
            ax.scatter(x, y, s=400, c='yellow', edgecolor='black', zorder=4, alpha=0.7)
            ax.text(x, y, str(node), ha='center', va='center', fontsize=10, 
                   fontweight='bold', zorder=5)


def create_topology_plot(args):
    """Create topology visualization."""
    fig, ax = plt.subplots(figsize=(12, 10))
    
    # Plot topology
    if args.topology == 'MESH':
        plot_mesh_topology(ax, args.x, args.y, args.z)
    elif args.topology == 'TORUS':
        plot_torus_topology(ax, args.x, args.y)
    
    # Load traffic data if provided
    df = None
    if args.input:
        input_path = Path(args.input)
        if input_path.is_dir():
            csv_path = input_path / 'TrafficInformation.csv'
        else:
            csv_path = input_path
        
        if csv_path.exists():
            df = load_traffic_data(csv_path)
            print(f"Loaded traffic data from: {csv_path}")
        else:
            print(f"Warning: Traffic data not found: {csv_path}")
    
    # Add traffic overlay
    if df is not None and args.show_routing:
        plot_traffic_overlay(ax, df, args.x, args.y, args.z)
        plot_routing_paths(ax, df, args.x, args.y)
    
    # Highlight nodes
    if args.highlight_nodes:
        plot_highlighted_nodes(ax, args.highlight_nodes, args.x, args.y)
    
    # Show labels
    if args.show_labels:
        for y in range(args.y):
            for x in range(args.x):
                node_id = y * args.x + x
                ax.text(x, y, str(node_id), ha='center', va='center', fontsize=8, zorder=3)
    
    plt.tight_layout()
    
    if args.output:
        plt.savefig(args.output, dpi=150, bbox_inches='tight')
        print(f"\n📁 Topology plot saved to: {args.output}")
    else:
        plt.show()


def main():
    args = parse_arguments()
    create_topology_plot(args)


if __name__ == '__main__':
    main()
