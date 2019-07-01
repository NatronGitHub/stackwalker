BREAKPAD_SRCDIR := ../breakpad/src
BREAKPAD_OBJDIR := ../breakpad
BREAKPAD_LIBS := \
  $(BREAKPAD_SRCDIR)/libbreakpad.a \
  $(BREAKPAD_SRCDIR)/third_party/libdisasm//libdisasm.a \
  $(NULL)

BINS := \
  stackwalker \
  $(NULL)

all: $(BINS)

EXTRA_OBJS := \
  http_symbol_supplier.o \
  $(NULL)

CURL_CFLAGS := $(shell pkg-config libcurl --cflags)
CURL_LIBS := $(shell pkg-config libcurl --libs)
LIBZIP_CFLAGS := $(shell pkg-config libzip --cflags)
LIBZIP_LIBS := $(shell pkg-config libzip --libs)
JSON_CFLAGS := $(shell pkg-config jsoncpp --cflags)
JSON_LIBS := $(shell pkg-config jsoncpp --libs)
# Don't -Werror everywhere, some compilers are too picky.
#WERROR := -Werror

CXXFLAGS += \
  -I$(BREAKPAD_SRCDIR) \
  -D__STDC_FORMAT_MACROS=1 \
  -std=gnu++0x \
  -Wno-format \
  $(WERROR) \
  $(JSON_CFLAGS) \
  $(CURL_CFLAGS) \
  $(LIBZIP_CFLAGS) \
  $(NULL)
LIBS := $(JSON_LIBS) $(CURL_LIBS) $(LIBZIP_LIBS)

.SECONDEXPANSION:
$(BINS): %: %.cc $(BREAKPAD_LIBS) $(EXTRA_OBJS) $$($$*_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $< $(EXTRA_OBJS) $(BREAKPAD_LIBS) $($*_OBJS) $(LIBS)

clean:
	$(RM) $(BINS) *.o
