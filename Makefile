################################################################
#
# Makefile
#
# - search recursively the source file
#
# Time-stamp: "2013-12-18 15:53:50 Gmaj7sus4"
#
################################################################
#---------------------------------------------------------------
# User setting
#---------------------------------------------------------------

TARGET   = crabat
INCLUDES = -I./include
NOMAKEDIR= .git% data% doc% src/bin% bin%
OBJDIR = objs

#---------------------------------------------------------------
# Don't change the following
#---------------------------------------------------------------

GCC = g++
CFLAGS  = -g -MMD -MP -Wall -O2 $(shell root-config --cflags)
LDFLAGS = $(shell root-config --libs) $(shell gsl-config --libs) -lSpectrum

CPPS = $(shell find * -name *.cpp)
SRCS = $(filter-out $(NOMAKEDIR), $(CPPS))
DIRS = $(dir $(SRCS))
BINDIRS = $(addprefix $(OBJDIR)/, $(DIRS))

OBJS = $(addprefix $(OBJDIR)/, $(patsubst %.cpp, %.o, $(SRCS)))
DEPS = $(OBJS:.o=.d)
TILS = $(patsubst %.cpp, %.cpp~, $(SRCS))

ifeq "$(strip $(OBJDIR))" ""
  OBJDIR = .
endif

ifeq "$(strip $(DIRS))" ""
  OBJDIR = .
endif

default:
	@[ -d  $(OBJDIR)   ] || mkdir -p $(OBJDIR)
	@[ -d "$(BINDIRS)" ] || mkdir -p $(BINDIRS)
	@make all --no-print-directory
#	./$(TARGET)

all : $(OBJS) $(TARGET)

$(TARGET): $(OBJS) $(LIBS)
	$(GCC) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp
	$(GCC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	@rm -rf $(TARGET) $(TILS) $(OBJDIR)

-include $(DEPS)
