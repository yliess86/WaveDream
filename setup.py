from setuptools import Extension
from setuptools import setup

import os
import sys
import pybind11


__version__ = "0.0.1"


desciption = (
    f"WaveDream is an experimental c++ made python module for virtual analog "
    f"synth. https://soundcloud.com/yliess-hati/wavedream-demo"
)

with open("README.md", "r") as fh:
    long_description = fh.read()

cpp_args = [
    "-std=c++17", 
    "-lsoundio",
    "-lpthread"
]

ext_modules = [
    Extension(
        "wavedream",
        ["src/binding.cpp"],
        include_dirs=[
            pybind11.get_include(False), 
            pybind11.get_include(True)
        ],
        language="c++",
        extra_compile_args=cpp_args
    )
]

setup(
    name="wavedream",
    version=__version__,
    author="Yliess HATI",
    author_email="hatiyliess86@gmail.com",
    description=desciption,
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/yliess86/WaveDream",
    python_requires=">=3.6",
    install_requires=["numpy", "pybind11"],
    ext_modules=ext_modules
)