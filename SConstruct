import os, sys, commands

build_dir     = 'build'
platform_libs = ['pthread', 'rt', 'dl']
boost_lib_path = '/usr/lib/x86_64-linux-gnu'
jsoncpp_include_path = '/usr/include/jsoncpp'
jsoncpp_lib_path     = '/usr/lib/x86_64-linux-gnu'
hiredis_lib_path     = '/usr/lib/x86_64-linux-gnu'

env = Environment()

env.Append(TOOLSET   = ['g++'])
env.Append(CXXFLAGS  = ['-pthread', '-std=c++11', '-g', '-O2', '-Wall', '-Werror', '-Wcast-align', '-Wshadow', '-Wunused-parameter'])
env.Append(LINKFLAGS = ['-pthread', '-static-libstdc++'])
env.Append(CPPPATH   = [jsoncpp_include_path])

Export('env')
Export('platform_libs')
Export('boost_lib_path')
Export('jsoncpp_lib_path')
Export('hiredis_lib_path')

base = SConscript("#/src/base/SConscript", variant_dir=build_dir+'/base', duplicate=0)

base_test = SConscript("#/src/base_test/SConscript", variant_dir=build_dir+'/base_test', duplicate=0)
