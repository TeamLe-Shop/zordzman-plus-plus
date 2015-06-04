import setuptools


setuptools.setup(
    name="zordzman",
    version="0.1.0",
    license="GPLv2",
    url="https://github.com/TeamLe-Shop/zordzman-plus-plus",
    description="Python tool and components for Zordzman++",
    classifiers=[
        "Development Status :: 2 - Pre-Alpha",
        "License :: OSI Approved :: GNU General Public License v2 (GPLv2)",
    ],
    packages=setuptools.find_packages("python"),
    package_dir={"": "python"},
)
