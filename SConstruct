import os, sys, commands

env = Environment()

env.Append(TOOLSET   = ['g++'])
env.Append(CXXFLAGS  = ['-std=c++11', '-g', '-O2', '-Wall', '-Werror', '-Wcast-align', '-Wshadow', '-Wunused-parameter'])
env.Append(LINKFLAGS = ['-static-libstdc++'])

build_dir     = 'build'
platform_libs = ['pthread', 'rt', 'dl']
boost_path    = '/usr/lib/x86_64-linux-gnu'

Export('env')
Export('platform_libs')
Export('boost_path')

base = SConscript("#/src/base/SConscript", variant_dir=build_dir+'/base', duplicate=0)

base_test = SConscript("#/src/base_test/SConscript", variant_dir=build_dir+'/base_test', duplicate=0)
