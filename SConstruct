import os, sys, commands

build_dir     = 'build'
platform_libs = ['pthread', 'rt']

env = Environment(ENV=os.environ)

mode = ARGUMENTS.get('mode', 'release')
if mode == 'debug':
    env.Append(CXXFLAGS = ['-O0'])
elif mode == 'release':
    env.Append(CXXFLAGS = ['-O2', '-DNDEBUG'])
else:
    print('Argument mode(%s) fatal.', mode)
    exit()

env.Append(TOOLSET   = ['g++'])
env.Append(CXXFLAGS  = ['-g', '-pthread', '-std=c++14', '-pipe'])
env.Append(CXXFLAGS  = ['-Wall', '-Wextra', '-Werror', '-Wunused-parameter', '-Woverloaded-virtual', '-Wpointer-arith', '-Wshadow', '-Wwrite-strings', '-Wcast-align', '-Wreorder'])
env.Append(CXXFLAGS  = ['-Wconversion',  '-Wold-style-cast'])
env.Append(LINKFLAGS = ['-pthread', '-static-libstdc++'])
env.Append(CPPPATH   = [])

jsoncpp_include_path = '/usr/include/jsoncpp'
libcurl_include_path = '/usr/include'
boost_lib_path       = '/usr/lib/x86_64-linux-gnu'
jsoncpp_lib_path     = '/usr/lib/x86_64-linux-gnu'
hiredis_lib_path     = '/usr/lib/x86_64-linux-gnu'
libcurl_lib_path     = '/usr/lib/x86_64-linux-gnu'

Export('env')
Export('libcurl_include_path')
Export('jsoncpp_include_path')
Export('platform_libs')
Export('boost_lib_path')
Export('jsoncpp_lib_path')
Export('hiredis_lib_path')
Export('libcurl_lib_path')

base = SConscript("#/src/base/SConscript", variant_dir=build_dir+'/base', duplicate=0)

base_test = SConscript("#/src/base_test/SConscript", variant_dir=build_dir+'/base_test', duplicate=0)

#recipe = SConscript("#/src/recipe/SConscript", variant_dir=build_dir+'/recipe', duplicate=0)
