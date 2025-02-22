from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import numpy as np
import subprocess
import os
import sys
from pathlib import Path
import shutil

class CMakeBuild(build_ext):
    def run(self):
        try:
            subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed to build the extension")

        source_dir = Path(__file__).parent.absolute()
        cmake_build_dir = source_dir / 'build'
        cmake_build_dir.mkdir(exist_ok=True)

        # Create cmake directory and gitversion.cmake if they don't exist
        cmake_dir = source_dir / 'cmake'
        cmake_dir.mkdir(exist_ok=True)
        gitversion_file = cmake_dir / 'gitversion.cmake'
        if not gitversion_file.exists():
            gitversion_file.write_text('file(WRITE "${output_dir}/gitversion.h" "#define GITVERSION \\"unknown\\"")\n')
        
        cmake_args = [
            f'-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={cmake_build_dir / "lib"}',
            f'-DPYTHON_EXECUTABLE={sys.executable}',
            '-DCMAKE_BUILD_TYPE=Release'
        ]
        
        # Configure and build RPG library
        subprocess.check_call(['cmake', str(source_dir)] + cmake_args, cwd=cmake_build_dir)
        subprocess.check_call(['cmake', '--build', '.'], cwd=cmake_build_dir)
        
        # Update library search path for the extension
        lib_dir = str(cmake_build_dir / "lib")
        if self.library_dirs is None:
            self.library_dirs = []
        self.library_dirs.append(lib_dir)
        
        # Run the standard build command
        super().run()

        # Copy the shared library to the extension directory
        ext_path = Path(self.get_ext_fullpath(self.extensions[0].name)).parent
        lib_name = 'librpg.so'
        lib_file = cmake_build_dir / 'lib' / lib_name
        if not lib_file.exists():
            raise RuntimeError(f"Built library not found at {lib_file}")
        shutil.copy2(lib_file, ext_path / lib_name)

# Define the extension module
rpg_module = Extension('genpoly_rpg',
                      sources=['rpg_module.c'],
                      include_dirs=[
                          np.get_include(),
                          'genpoly-rpg/src',
                          'build/src',
                      ],
                      library_dirs=[],  # Will be set by CMakeBuild
                      libraries=['rpg'],
                      runtime_library_dirs=['$ORIGIN'],
                      extra_compile_args=['-std=c99'])

setup(
    name='genpoly_rpg',
    version='1.0',
    description='Python interface for Random Polygon Generator (RPG)',
    ext_modules=[rpg_module],
    cmdclass={'build_ext': CMakeBuild},
    zip_safe=False,
)