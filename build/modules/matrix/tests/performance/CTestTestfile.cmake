# CMake generated Testfile for 
# Source directory: /home/frukt71/Github/dumb_math_test/dumb_math/modules/matrix/tests/performance
# Build directory: /home/frukt71/Github/dumb_math_test/dumb_math/build/modules/matrix/tests/performance
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[MatrixEnvSetup]=] "sudo" "/home/frukt71/Github/dumb_math_test/dumb_math/modules/benchmarking/env_setup/set_performance.sh")
set_tests_properties([=[MatrixEnvSetup]=] PROPERTIES  FIXTURES_SETUP "MatrixPerfFixture" _BACKTRACE_TRIPLES "/home/frukt71/Github/dumb_math_test/dumb_math/modules/matrix/tests/performance/CMakeLists.txt;41;add_test;/home/frukt71/Github/dumb_math_test/dumb_math/modules/matrix/tests/performance/CMakeLists.txt;0;")
add_test([=[MatrixEnvTeardown]=] "sudo" "/home/frukt71/Github/dumb_math_test/dumb_math/modules/benchmarking/env_setup/restore_cpu_state.sh")
set_tests_properties([=[MatrixEnvTeardown]=] PROPERTIES  FIXTURES_CLEANUP "MatrixPerfFixture" _BACKTRACE_TRIPLES "/home/frukt71/Github/dumb_math_test/dumb_math/modules/matrix/tests/performance/CMakeLists.txt;45;add_test;/home/frukt71/Github/dumb_math_test/dumb_math/modules/matrix/tests/performance/CMakeLists.txt;0;")
add_test([=[RunMatrixPerfTest]=] "sudo" "/home/frukt71/Github/dumb_math_test/dumb_math/modules/benchmarking/env_setup/run_max_priority.sh" "/home/frukt71/Github/dumb_math_test/dumb_math/build/modules/matrix/tests/performance/matrix_performance_test" "1")
set_tests_properties([=[RunMatrixPerfTest]=] PROPERTIES  FIXTURES_REQUIRED "MatrixPerfFixture" _BACKTRACE_TRIPLES "/home/frukt71/Github/dumb_math_test/dumb_math/modules/matrix/tests/performance/CMakeLists.txt;49;add_test;/home/frukt71/Github/dumb_math_test/dumb_math/modules/matrix/tests/performance/CMakeLists.txt;0;")
add_test([=[PlotMatrixLatency]=] "bash" "-c" "/usr/bin/python3.10 /home/frukt71/Github/dumb_math_test/dumb_math/modules/benchmarking/stat_plot/spline.py /home/frukt71/Github/dumb_math_test/dumb_math/build/modules/matrix/tests/performance/latency/plots /home/frukt71/Github/dumb_math_test/dumb_math/build/modules/matrix/tests/performance/latency/data/*.csv")
set_tests_properties([=[PlotMatrixLatency]=] PROPERTIES  DEPENDS "RunMatrixPerfTest" FIXTURES_REQUIRED "MatrixPerfFixture" _BACKTRACE_TRIPLES "/home/frukt71/Github/dumb_math_test/dumb_math/modules/matrix/tests/performance/CMakeLists.txt;57;add_test;/home/frukt71/Github/dumb_math_test/dumb_math/modules/matrix/tests/performance/CMakeLists.txt;0;")
