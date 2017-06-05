CXX = g++
ARCR = ar -cr

OS =
ifeq ($(shell uname), Linux)
	OS=linux
else
	OS=macos
endif

### mod .git path
CHEF_VERSION =
ifeq ($(shell test -e ../../.git && echo $$?), 0)
	BUILD_DATE = $(shell date +"%Y%m%d%H%M%S_")
	GIT_VERSION = $(shell git log --oneline | head -1 | cut -b 1-7)
	CHEF_VERSION = $(BUILD_DATE)$(GIT_VERSION)
else
	CHEF_VERSION = "unknown"
endif

# -Wconversion -Wold-style-cast
CXXFLAGS = -g -pthread -std=c++14 -pipe -fPIC -Wall -Wextra -Werror -Wunused-parameter -Woverloaded-virtual \
           -Wpointer-arith -Wshadow -Wwrite-strings -Wcast-align \
           -DCHEF_VERSION=$(CHEF_VERSION)

CXXFLAGS_DEBUG   = -O0
CXXFLAGS_RELEASE = -O2 -DNDEBUG


SRC_PATH = .

DEBUG_OUTPUT   = $(SRC_PATH)/debug
RELEASE_OUTPUT = $(SRC_PATH)/release

### mod
TARGET = libchefbase.a

### mod
INCLUDE_PATH = -I. \
               -I/usr/include/jsoncpp

SRCS := $(shell find $(SRC_PATH) -name '*.cc')
SRCS += $(wildcard $(SRC_PATH)/*.cc)

DEBUG_OBJS   = $(addprefix $(DEBUG_OUTPUT)/, $(patsubst %.cc,%.o,$(SRCS)))
RELEASE_OBJS = $(addprefix $(RELEASE_OUTPUT)/, $(patsubst %.cc,%.o,$(SRCS)))

all: release
.PHONY: all

-include $(patsubst %.cc,$(DEBUG_OUTPUT)/%.d,$(SRCS))
-include $(patsubst %.cc,$(RELEASE_OUTPUT)/%.d,$(SRCS))

$(DEBUG_OUTPUT)/%.o: $(SRC_PATH)/%.cc
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -MM -MP -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE_PATH)

$(RELEASE_OUTPUT)/%.o: $(SRC_PATH)/%.cc
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -MM -MP -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE_PATH)

debug: CXXFLAGS += $(CXXFLAGS_DEBUG)
debug: $(DEBUG_OBJS)
	$(ARCR) $(DEBUG_OUTPUT)/$(TARGET) $(DEBUG_OBJS)

release: CXXFLAGS += $(CXXFLAGS_RELEASE)
release: $(RELEASE_OBJS)
	$(ARCR) $(RELEASE_OUTPUT)/$(TARGET) $(RELEASE_OBJS)

.PHONY: clean
clean:
	rm -rf $(DEBUG_OUTPUT)
	rm -rf $(RELEASE_OUTPUT)

.PHONY: love
love:
	@echo 'make love done...'
	@echo $(CHEF_VERSION)
