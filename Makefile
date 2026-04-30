flags = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

common_all = COMMON/include/helpingFunctions.h COMMON/include/nameTableConsts.h COMMON/include/nameTableStack.h COMMON/include/operatorsArray.h COMMON/include/structAccessFunctions.h COMMON/include/structsAndConsts.h COMMON/include/treeFunctions.h COMMON/include/saveTreeInFile.h COMMON/include/readTreeFromFileFunc.h COMMON/include/numbersArray.h

common_cpp = COMMON/src/helpingFunctions.cpp COMMON/src/nameTableStack.cpp COMMON/src/structAccessFunctions.cpp COMMON/src/treeFunctions.cpp COMMON/src/saveTreeInFile.cpp COMMON/src/readTreeFromFileFunc.cpp

front_cpp = frontend/src/main.cpp frontend/src/lexicalAnalysis.cpp frontend/src/syntacticAnalysis.cpp

front_all = frontend/include/DSL.h frontend/include/lexicalAnalysis.h frontend/include/syntacticAnalysis.h

back_cpp = backend/src/backendMain.cpp backend/src/asmProgramWriter.cpp backend/src/sourceFileParser.cpp

back_all = backend/include/asmProgramWriter.h backend/include/backendConsts.h backend/include/sourceFileParser.h

middle_cpp = middleend/src/middleEndMain.cpp middleend/src/middleendOptimization.cpp

middle_all = middleend/include/middleendOptimization.h

reverse_cpp = reverse-frontend/src/reverseFrontendFunctions.cpp reverse-frontend/src/reverseFrontendMain.cpp

reverse_all = reverse-frontend/include/reverseFrontendFunctions.h reverse-frontend/include/reverseFrontendDSL.h

build-f: $(front_all) $(common_cpp) $(common_all)
	@echo -------------------------------------------------------------------------
	g++ $(front_cpp) $(common_cpp) $(flags) -o frontend/build/frontend.exe

build-b: $(back_all) $(common_cpp) $(common_all)
	@echo -------------------------------------------------------------------------
	g++ $(back_cpp) $(common_cpp) $(flags) -o backend/build/backend.exe

build-m: $(middle_all) $(middle_cpp) $(common_all)
	@echo -------------------------------------------------------------------------
	g++ $(middle_cpp) $(common_cpp) $(flags) -o middleend/build/middleend.exe

build-r: $(reverse_all) $(reverse_cpp) $(common_all)
	@echo -------------------------------------------------------------------------
	g++ $(reverse_cpp) $(common_cpp) $(flags) -o reverse-frontend/build/reverse.exe

run-f:
	./frontend/build/frontend.exe

run-b:
	./backend/build/backend.exe

run-m:
	./middleend/build/middleend.exe
run-r:
	./reverse-frontend/build/reverse.exe
