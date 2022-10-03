clean_up_and_exit ()
{
    ARG=$?

    if [[ $1 ]]; then
        echo -e "\n> $1"
    fi

    rm -f -- *.exe
    rm ../src/*.gch -f
    exit $ARG
}

mkdir -p tests/out

gcc test_runner.c ../src/*.? -o transpile_tests || clean_up_and_exit "Error compiling test_runner; ec $?"
./transpile_tests.exe

for test_x in tests/test*.bl
do
  test_name=$(echo "$test_x" | sed -E 's/tests\/(.*)\.bl/\1/')

  gcc tests/out/"$test_name".bl.c -o "$test_name" || clean_up_and_exit "Error compiling $test_x.c; ec $?"

  test_exit_code=$?

  test_output="$(./"$test_name".exe)"
  # shellcheck disable=SC2206
  test_output_split=(${test_output//;/ })

  test_expected_result=${test_output_split[0]}
  test_actual_result=${test_output_split[1]}

  if [[ "$test_expected_result" == "$test_actual_result" ]]; then
    echo -e "\n$test_x SUCCESS; ec $test_exit_code"
  else
    echo -e "\n$test_x FAIL; expected: \"$test_expected_result\", actual: \"$test_actual_result\"; ec $test_exit_code"
  fi
done

clean_up_and_exit
