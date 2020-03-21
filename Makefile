
OUT_DIRS = build bin
NOMOBJS = t2dv2.o 
OBJS = $(NOMOBJS) main.o 
TOBJS = $(NOMOBJS) tests.o

NOMHEADERS = t2dv2.h 

NOMSOURCES = t2dv2.cpp 
SOURCES = $(NOMSOURCES) main.cpp
TSOURCES = $(NOMSOURCES) tests.cpp

LIBS = -lhdt -pthread -leasylogger -ltabularparser -ltadahdtentity
TLIBS = $(LIBS) -lgtest

CC = g++
CXXFLAGS = -std=c++11  

TESTAPP = bin/testapp
COVAPP = bin/covapp

OBJS_ABS = $(patsubst %,build/%,$(OBJS))
TOBJS_ABS = $(patsubst %,build/%,$(TOBJS))
SOURCES_ABS = $(patsubst %,src/%,$(SOURCES))
TSOURCES_ABS = $(patsubst %,src/%,$(TSOURCES))


COVCLEANFILES = gcov.css snow.png ruby.png *.gcov  *.gcda *.gcno index-sort-f.html index-sort-l.html index.html \
				amber.png glass.png updown.png coverage.info emerald.png Users usr v1\

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LIBCMAKETARGET := liblinux
	DETECTEDOS := Linux
endif

ifeq  ($(UNAME_S),Darwin)
	DETECTEDOS := MacOS
	LIBCMAKETARGET := libmac
endif

.PHONY : clean run test cov codecov install lib liblinux libmac


bin/tadaeexperiment: $(OBJS_ABS)
	$(CC) -o $@ $? $(LIBS)


$(OBJS_ABS): $(SOURCES_ABS) $(OUT_DIRS)
	$(CC) $(CXXFLAGS) -c $(SOURCES_ABS)
	mv *.o build/




cov:	
	$(CC) $(CXXFLAGS) -c -fprofile-arcs -ftest-coverage -fPIC  $(TSOURCES_ABS)
	mv *.o build/
	$(CC) $(CXXFLAGS) -o $(COVAPP) -fprofile-arcs -ftest-coverage $(TOBJS_ABS) $(TLIBS) 
	$(COVAPP)
	lcov --directory . --capture --output-file coverage.info
	lcov --remove coverage.info '/usr/*' --output-file coverage.info
	# This is to ignore the xcode directory (will have no effect on linux)
	lcov --remove coverage.info '/Applications/Xcode.app/*' --output-file coverage.info
	lcov --list coverage.info


codecov:
	$(MAKE) cov
	curl -s https://codecov.io/bash > codecovpush.sh
	chmod +x codecovpush.sh
	./codecovpush.sh
	rm codecovpush.sh
	$(MAKE) clean	

lib:
	echo  "Detected OS: " $(DETECTEDOS)
	$(MAKE) $(LIBCMAKETARGET)

libmac:
	$(CC) $(CXXFLAGS)  -dynamiclib -flat_namespace  $(NOMSOURCES)  -o $(LIBNAME)

liblinux:
	$(CC) $(CXXFLAGS) -fPIC -shared $(NOMSOURCES) -o  $(LIBNAME)

install:
	mkdir -p  $(HDIR)
	cp $(NOMHEADERS) $(HDIR)
	$(MAKE) lib
	mv $(LIBNAME)  $(LIBDIR)
	ln -fs $(LIBDIR)/$(LIBNAME)  $(LIBDIR)/$(LIBALIAS)
	echo -e "tada_hdt_entity_example lib is installed"
	$(MAKE) clean


run: 
	bin/tadaeexample
	

test:
	$(CC)  ${CXXFLAGS} -c $(TSOURCES_ABS)
	mv *.o build/
	$(CC) -o $(TESTAPP) $(TOBJS_ABS) $(TLIBS)
	$(TESTAPP)

bin:	 
	mkdir -p $@

build:
	mkdir -p $@

clean:
	-rm -Rf build
	-rm -Rf bin
	-rm -Rf $(COVCLEANFILES)
	-rm -Rf test.hdt test.hdt.index.v1-1 test.log
