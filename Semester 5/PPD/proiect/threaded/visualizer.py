"""
Optional visualization for threaded n-body simulation.
Requires matplotlib for 3D plotting.
"""

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from typing import List
from shared.body import Body


class NBodyVisualizer:
    """
    Visualizer for n-body simulation trajectories.
    """
    
    def __init__(self, figsize: tuple = (12, 10)):
        """
        Initialize the visualizer.
        
        Args:
            figsize: Figure size (width, height)
        """
        self.fig = plt.figure(figsize=figsize)
        self.ax = self.fig.add_subplot(111, projection='3d')
        self.trajectories = {}  # Store trajectories for each body
    
    def update(self, bodies: List[Body]) -> None:
        """
        Update trajectories with current body positions.
        
        Args:
            bodies: List of bodies with current positions
        """
        for body in bodies:
            if body.id not in self.trajectories:
                self.trajectories[body.id] = []
            self.trajectories[body.id].append(body.position.copy())
    
    def plot(self, show_current: bool = True) -> None:
        """
        Plot all trajectories.
        
        Args:
            show_current: Whether to highlight current positions
        """
        self.ax.clear()
        
        # Plot trajectories
        colors = plt.cm.tab20(np.linspace(0, 1, len(self.trajectories)))
        for i, (body_id, trajectory) in enumerate(self.trajectories.items()):
            trajectory = np.array(trajectory)
            self.ax.plot(trajectory[:, 0], trajectory[:, 1], trajectory[:, 2],
                        color=colors[i], alpha=0.6, linewidth=1.5,
                        label=f'Body {body_id}')
            
            # Highlight current position
            if show_current and len(trajectory) > 0:
                self.ax.scatter(trajectory[-1, 0], trajectory[-1, 1], 
                              trajectory[-1, 2], color=colors[i], 
                              s=100, marker='o', edgecolors='black')
        
        self.ax.set_xlabel('X (m)')
        self.ax.set_ylabel('Y (m)')
        self.ax.set_zlabel('Z (m)')
        self.ax.set_title('N-Body Simulation Trajectories')
        self.ax.legend()
        plt.tight_layout()
    
    def show(self) -> None:
        """Display the plot."""
        plt.show()
    
    def save(self, filename: str, dpi: int = 300) -> None:
        """
        Save the plot to a file.
        
        Args:
            filename: Output filename
            dpi: Resolution in dots per inch
        """
        self.fig.savefig(filename, dpi=dpi, bbox_inches='tight')
        print(f"Plot saved to {filename}")

