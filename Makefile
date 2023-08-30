UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
BROWSER_OPENER = @open
LEAK_CMD = CK_FORK=no leaks -atExit -- ./$(TEST_PATH)
endif
ifeq ($(UNAME), Linux)
BROWSER_OPENER = @x-www-browser
LEAK_CMD = CK_FORK=no valgrind --leak-check=full ./$(TEST_PATH)
endif
GNU_COMPILER = -D CMAKE_CXX_COMPILER=g++ -D CMAKE_C_COMPILER=gcc
APP_LIB_PATH = matrix_oop.a
SRC_DIR = ./lib
SRC_EXT = cc
TEST_PATH = buildRelease/test

.PHONY: all clean test $(APP_LIB_PATH) gcov_report

all: clean buildRelease

$(APP_LIB_PATH): buildRelease
	@cmake --build buildRelease --target test
	cp ./buildRelease/lib_matrix_oop.a ./${APP_LIB_PATH}

clean:
	@rm -rf buildRelease 2>/dev/null
	@rm -rf buildDebug 2>/dev/null
	@rm *.tar.gz 2>/dev/null || true
	rm -rf ./${APP_LIB_PATH}

test: buildRelease
	@cmake --build buildRelease --target test
	@echo "\033[0;32m-----------TESTS------------:\033[0m"
	@./buildRelease/test
	@echo "\033[0;32m----------------------------:\033[0m"
	@echo "\033[0;32m------CHECK FORMATTING------:\033[0m"
	@cmake --build buildRelease --target format-check
	@echo "\033[0;32m----------------------------:\033[0m"

leaks:
	$(LEAK_CMD)

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem ${SRC_DIR}/*.${SRC_EXT} ${SRC_DIR}/*.h

gcov_report: buildDebug
	@cmake --build buildDebug --target test_coverage
	${BROWSER_OPENER} buildDebug/test_coverage/index.html
	
buildRelease:
	@cmake -S . -B buildRelease -D CMAKE_BUILD_TYPE=Release
buildDebug:
	@cmake -S . -B buildDebug $(GNU_COMPILER) -D CMAKE_BUILD_TYPE=Debug
