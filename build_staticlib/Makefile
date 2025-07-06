DEPENDENCY_DIR=$(pwd)/ios_dependencies/ios_libs
BCPD_DIR := bcpd/register
BASE_DIR := bcpd/base

BCPDSRC := $(wildcard $(BCPD_DIR)/*.c $(BASE_DIR)/*.c)
LIBSRC := $(filter-out $(BCPD_DIR)/main.c, $(BCPDSRC))
LIBOBJS := $(patsubst %.c,%.o,$(LIBSRC))
OUTDIR := ./dist

IOS_CC := xcrun --sdk iphoneos clang
IOS_AR := xcrun --sdk iphoneos ar
IOS_CFLAGS := -arch arm64 -isysroot $(shell xcrun --sdk iphoneos --show-sdk-path) -fembed-bitcode
IOS_CFLAGS += -I$(DEPENDENCY_DIR)/openmp/include -I$(DEPENDENCY_DIR)/openblas/include
IOS_LDFLAGS := $(DEPENDENCY_DIR)/openmp/lib/libomp.a $(DEPENDENCY_DIR)/openblas/lib/libopenblas.a
IOS_LDFLAGS += -framework Accelerate

$(BCPD_DIR)/lib_bcpd_private.h: $(BCPD_DIR)/main.c
	sed 's/\bmain\b/bcpd_main/g' $< > $@

$(BCPD_DIR)/lib_bcpd.c:
	cp src/lib_bcpd.c $(BCPD_DIR)/lib_bcpd.c

$(BCPD_DIR)/lib_bcpd.h:
	cp src/lib_bcpd.h $(BCPD_DIR)/lib_bcpd.h

$(BCPD_DIR)/lib_bcpd.o: $(BCPD_DIR)/lib_bcpd.c $(BCPD_DIR)/lib_bcpd_private.h $(BCPD_DIR)/lib_bcpd.h
	$(IOS_CC) -O3 $(IOS_CFLAGS) -c $(BCPD_DIR)/lib_bcpd.c -o $@

$(OUTDIR)/libbcpd.a: $(LIBOBJS) $(BCPD_DIR)/lib_bcpd.o
	@mkdir -p $(OUTDIR)
	$(IOS_AR) rcs $(OUTDIR)/libbcpd.a $^

%.o: %.c
	$(IOS_CC) -O3 $(IOS_CFLAGS) -c $< -o $@

all : $(OUTDIR)/libbcpd.a

clean:
	rm -f $(OUTDIR)/libbcpd.a $(BCPD_DIR)/*.o $(BASE_DIR)/*.o $(BCPD_DIR)/lib_bcpd.c $(BCPD_DIR)/lib_bcpd.o
	rm -f $(BCPD_DIR)/lib_bcpd.h $(BCPD_DIR)/lib_bcpd_private.h 
