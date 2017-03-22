# TODOO
# * （小问题，忽略）没任何改变时，还ar了一次
# * (done) 改变header时，没有重新编译需要编译的单元
# * debug和release切换时，没有重新编译
# * 指定输出目录

PWD = $(shell pwd)
CXX = g++
ARCR = ar -cr
# -Wconversion -Wold-style-cast
CXXFLAGS = -g -pthread -std=c++14 -pipe -fPIC -Wall -Wextra -Werror -Wunused-parameter -Woverloaded-virtual \
           -Wpointer-arith -Wshadow -Wwrite-strings -Wcast-align

CXXFLAGS_DEBUG = -O0
CXXFLAGS_RELEASE = -O2 -DNDEBUG

SRC_PATH = $(PWD)

DEBUG_OUTPUT = $(PWD)/debug/
RELEASE_OUTPUT = $(PWD)/release/

TARGET = libchefbase.a

INCLUDE_PATH = -I. \
               -I/usr/include/jsoncpp

LIBS =

SRCS = $(wildcard $(SRC_PATH)/*.cc)

#DEBUG_OBJS := $(notdir $(patsubst %.cc,%.do,$(SRCS)))
RELEASE_OBJS := $(addprefix $(RELEASE_OUTPUT), $(notdir $(patsubst %.cc,%.ro,$(SRCS))))

debug: CXXFLAGS += $(CXXFLAGS_DEBUG)
release: CXXFLAGS += $(CXXFLAGS_RELEASE)

all: release
.PHONY: all

#sinclude $(SRCS:.cc=.d)
sinclude $(addprefix $(RELEASE_OUTPUT), $(notdir $(patsubst %.cc,%.d,$(SRCS))))

#$(DEBUG_OBJS): %.do : %.cc
#	@mkdir -p $(DEBUG_OUTPUT)
#	@$(CXX) $(CXXFLAGS) -MM -MP -MT $@ -MF $(@:.do=.d) $<
#	$(CXX) $(CXXFLAGS) -c $< -o $(DEBUG_OUTPUT)/$@ $(INCLUDE_PATH)

$(RELEASE_OBJS): %.ro : %.cc
	@echo "hello"
	@mkdir -p $(RELEASE_OUTPUT)
	$(CXX) $(CXXFLAGS) -MM -MP -MT $(RELEASE_OUTPUT)$@ -MF $(@:.ro=.d) $<
	$(CXX) $(CXXFLAGS) -c $< -o $(RELEASE_OUTPUT)$@ $(INCLUDE_PATH)

#debug: $(DEBUG_OBJS)
#	$(ARCR) $(TARGET) $(DEBUG_OBJS)

release: $(RELEASE_OBJS)
	$(ARCR) $(RELEASE_OUTPUT)$(TARGET) $(addprefix $(RELEASE_OUTPUT), $^)

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f $(TARGET)
	rm -rf *.d

.PHONY: love
love:
	@echo 'make love done...'
	@echo $(OBJS)
