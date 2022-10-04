clean_up_and_exit ()
{
    ARG=$?

    if [[ $1 ]]; then
      echo -e "\n> $1"
    fi

    rm -f -- *.exe -f
    exit $ARG
}

mkdir -p playground/out

gcc playground/playground_runner.c ../src/*.c -o transpile_playground || clean_up_and_exit "Error compiling playground_runner; ec $?"
./transpile_playground.exe

gcc playground/out/playground.bl.c -o playground || clean_up_and_exit "Error compiling playground; ec $?"
./playground.exe

clean_up_and_exit
