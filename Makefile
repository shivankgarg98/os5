### Common flags
#
CF_ALL = -ggdb -Wall -Wextra -std=gnu99
LF_ALL =
LL_ALL = 

builddir := build

### Build commands
CC = i586-pc-myos-gcc
COMP = $(CC) $(CF_ALL) $(CF_TGT) -o $@ -c $<
LINK = $(CC) $(LF_ALL) $(LF_TGT) -o $@ $^ $(LL_TGT) $(LL_ALL)
COMPLINK = $(CC) $(CF_ALL) $(CF_TGT) $(LF_ALL) $(LF_TGT) \
	-o $@ $< $(LL_TGT) $(LL_ALL)
DEP = $(CC) -M $(CF_ALL) $(CF_TGT) $< \
	-o $@ -MT "$(builddir)/$*.o $(builddir)/$*.d"

### Git status flags
GITHASH := $(shell git log -1 --pretty="tformat:%h")
GITDATE := $(shell git log -1 --pretty="tformat:%cd")
GITDIRTY := $(shell [[ -n `git status -s 2> /dev/null` ]] && echo 1 || echo 0)
GITMESSAGE := $(shell git log -1 --pretty="tformat:%s")
GITBRANCH := $(shell git log -1 --pretty="tformat:%d")
GITFLAGS := -DGITHASH='"$(GITHASH)"' \
	-DGITDATE='"$(GITDATE)"' \
	-DGITDIRTY='$(GITDIRTY)' \
	-DGITMESSAGE='"$(GITMESSAGE)"' \
	-DGITBRANCH='"$(GITBRANCH)"'

### Common functions
# $(call findsource, suffix, dir, depth, builddir)
findsource = $(addprefix $(4)/, \
	     $(patsubst %.$(2), %.o, \
	     $(shell find $(1) -maxdepth $(3) -name "*.$(2)")))


include Rules.mk
