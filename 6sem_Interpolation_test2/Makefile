#############################################################################
# ��� makefile ��� ��������, ����������� � ���� �������

# ����������� �����:

    TargetName		:=App		# ��� ������������ �����
    Debug		:=n		# ������� - y, ����������� - n
    CompilingOptions	:=		# ����� �����������
    LinkingOptions	:=-lm -L/usr/X11R6/lib -lX11 # ����� ������� (��������, -lm)

# ����������� �����:

    ObjectFilesDirectory:=./obj 	# ��� ������� .o � .d �����
    ManiacCompiling	:=n		# ��� �������� ANSI C

#############################################################################
#
#   ���������� �����

TargetName:=$(strip $(TargetName))
Debug:=$(strip $(Debug))
CompilingOptions:=$(strip $(CompilingOptions))
LinkingOptions:=$(strip $(LinkingOptions))
ObjectFilesDirectory:=$(strip $(ObjectFilesDirectory))
ManiacCompiling:=$(strip $(ManiacCompiling))

DebugCompilingOptions:=-g
ReleaseCompilingOptions:=-O3 -fexpensive-optimizations -malign-double -DNDEBUG -ffast-math
DebugLinkingOptions:=# ������ ������ �� ������
ReleaseLinkingOptions:=-O3# ���������, � ��� ���-������ ������?
ManiacCompilingOptions:=-Wall -Werror -Wshadow -pedantic-errors \
    -Wpointer-arith -Waggregate-return \
    -Wstrict-prototypes -Wmissing-declarations \
    -Wlong-long -Winline -Wredundant-decls -Wcast-qual -Wcast-align \
    -D__STRICT_ANSI__

# ������ ��������

ifeq ($(Debug),y)
    CompilingOptions+=$(DebugCompilingOptions)
    LinkingOptions+=$(DebugLinkingOptions)
else
    CompilingOptions+=$(ReleaseCompilingOptions)
    LinkingOptions+=$(ReleaseLinkingOptions)
endif

ifeq ($(ManiacCompiling),y)
    CompilingOptions+=$(ManiacCompilingOptions)
endif

# ������ ���������
FilesSuffix1:=.c
FilesSuffix2:=.cpp
FilesSuffix1:=$(strip $(FilesSuffix1))
FilesSuffix2:=$(strip $(FilesSuffix2))

SourceFiles1:=$(wildcard *$(FilesSuffix1))
SourceFiles2:=$(wildcard *$(FilesSuffix2))
ObjectFiles:=$(SourceFiles1:%$(FilesSuffix1)=$(ObjectFilesDirectory)/%.o)
ObjectFiles+=$(SourceFiles2:%$(FilesSuffix2)=$(ObjectFilesDirectory)/%.o)

#############################################################################
#
# ���� �������

CCompiler:=gcc
CppCompiler:=g++
Linker:=g++

.PHONY: clean rebuild

$(ObjectFilesDirectory)/%.o: %$(FilesSuffix1) Makefile
	@mkdir -p $(ObjectFilesDirectory)
	@echo compiling $<
	@$(CCompiler) $(CompilingOptions) -c -o $@ $<

$(ObjectFilesDirectory)/%.o: %$(FilesSuffix2) Makefile
	@mkdir -p $(ObjectFilesDirectory)
	@echo compiling $<
	@$(CppCompiler) $(CompilingOptions) -c -o $@ $<

$(TargetName): $(ObjectFiles) Makefile
	@echo linking
	@$(Linker) $(ObjectFiles) $(LinkingOptions) -o $(TargetName)
	
	@echo "(Compiling options: $(CompilingOptions))"
	@echo "(Linking options: $(LinkingOptions))"
    ifeq ($(Debug),n)
	strip $(TargetName)
    endif

# � ��������� ������ ��������, � �������� �� ����� ��� ���.
	@if [ -t 1 ]; then echo "[1;32mSuccess[0;m"; else echo Success!; fi

clean:
	rm -rf $(ObjectFilesDirectory) $(TargetName)

rebuild: clean $(TargetName)
