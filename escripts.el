;; Replace $PWD with your path
(shell-command "
   cd $PWD
   cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
   cmake --build build
   ./build/tutorial/exe
 ")
