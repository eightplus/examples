PACKAGENAME = boost_py_cpp_module

CXX = c++
STRIP = strip
CXXFLAGS ?= -O3 -Wall

INCLUDES = `pkg-config --cflags python`
LIBS = -lboost_python

SITE = $(shell python -c 'from distutils.sysconfig import get_python_lib;print get_python_lib()')

all: $(PACKAGENAME).so

.cc.o:
	$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@

$(PACKAGENAME).so: daemon.cc
	$(CXX) -L. -shared -fPIC $^ -o $@ $(INCLUDES) $(LIBS)
	$(STRIP) $@

install: $(PACKAGENAME).so
	mkdir -p $(DESTDIR)$(SITE)
	install -m 755 $(PACKAGENAME).so $(DESTDIR)$(SITE)
	
unstall:
	rm -f $(SITE)/$(PACKAGENAME).so
	
clean:
	rm -f $(OBJS) lib$(PACKAGENAME).a $(PACKAGENAME).so
