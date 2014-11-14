"""
Add parent dir to sys.path so that one can use the product from an uninstalled
source tree.
"""
import os
import sys

parentPath = os.path.join(os.path.dirname(__file__), os.pardir)
sys.path.insert(0, parentPath)
