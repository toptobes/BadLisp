clean_up_and_exit ()
{
    ARG=$?

    if [[ $1 ]]; then
        echo -e "\n> $1"
    fi

    rm -f -- *.exe
    rm ../src/*.gch -f
    rm test*.bl.c -f
    exit $ARG
}


gcc main_test_runner.c ../src/*.? -Wl,-etranspile_tests -o transpile_tests || clean_up_and_exit "Error compiling main_test_runner; ec $?"
./transpile_tests.exe


gcc test1.bl.c -o test1 || clean_up_and_exit "Error compiling test1; ec $?"

test_result="$(./test1.exe)"
test_exit_code=$?

if [[ $test_result == "72" ]]; then
  echo -e "\ntest1 SUCCESS; ec $test_exit_code"
else
  echo -e "\ntest1 FAIL; result: $test_result; ec $test_exit_code"
fi

clean_up_and_exit
