import os, sys, commands

env = Environment(ENV=os.environ)

mode = ARGUMENTS.get('mode', 'Release')
if mode == 'Debug':
    env.Append(CXXFLAGS = ['-O0'])
elif mode == 'Release':
    env.Append(CXXFLAGS = ['-O2', '-DNDEBUG'])
else:
    print('Argument mode fatal,only support debug or release.')
    exit()

common_CXXFLAGS = ['-g', '-pthread', '-std=c++11', '-pipe', '-fPIC']
env.Append(TOOLSET   = ['g++'])
env.Append(CXXFLAGS  = common_CXXFLAGS)
env.Append(CXXFLAGS  = ['-Wall', '-Wextra', '-Wunused-parameter', '-Woverloaded-virtual', '-Wpointer-arith', '-Wshadow',
                        '-Wwrite-strings', '-Wcast-align'])
#env.Append(CXXFLAGS  = ['-Werror'])
#env.Append(CXXFLAGS  = ['-Wconversion',  '-Wold-style-cast'])
env.Append(CPPPATH   = ['#include'])

if env['PLATFORM'] == 'darwin':
    platform_libs = ['pthread']
    #env.Append(CPPPATH = ['#third_party/macos/boost/include'])
    #boost_lib_path = '#third_party/macos/boost/lib/'
else:
    platform_libs = ['pthread', 'rt']
    #env.Append(CPPPATH = ['#third_party/linux/boost/include'])
    #boost_lib_path = '#third_party/linux/boost/lib/'
    env.Append(LINKFLAGS = ['-pthread', '-static-libstdc++'])

Export('env')
Export('platform_libs')
#Export('boost_lib_path')

#SConscript('include/chef_base/SConscript', variant_dir='output/lib', duplicate=0)
SConscript('test/chef_base_test/SConscript', variant_dir='output/bin/', duplicate=0)
