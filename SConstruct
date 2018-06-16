import os, sys, commands

env = Environment(ENV=os.environ)

mode = ARGUMENTS.get('mode', 'release')
if mode == 'debug':
    env.Append(CXXFLAGS = ['-O0'])
elif mode == 'release':
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
env.Append(CXXFLAGS  = ['-Wconversion',  '-Wold-style-cast'])
env.Append(LINKFLAGS = ['-pthread', '-static-libstdc++'])
env.Append(CPPPATH   = [])

if env['PLATFORM'] == 'darwin':
    platform_libs = ['pthread']
else:
    platform_libs = ['pthread', 'rt']

Export('env')
Export('platform_libs')

SConscript('src/base/SConscript', variant_dir='output/base', duplicate=0)
SConscript('src/base_test/SConscript', variant_dir='output/base_test', duplicate=0)
