from setuptools import setup, find_packages

setup(
    name="ecc_implementation",
    version="0.1.0",
    description="A Python library for Elliptic Curve Cryptography",
    author="Zyleree Kov",
    author_email="example@example.com",
    url="https://github.com/yourusername/ecc_implementation",
    package_dir={"": "src"},
    packages=find_packages(where="src"),
    classifiers=[
        "Development Status :: 3 - Alpha",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3",
        "Topic :: Security :: Cryptography",
    ],
    python_requires=">=3.6",
) 