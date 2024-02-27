import os
from setuptools import find_packages
from setuptools import setup

setup(
    name='force_dimension',
    version='0.2.0',
    packages=find_packages(
        include=('force_dimension', 'force_dimension.*')),
)
