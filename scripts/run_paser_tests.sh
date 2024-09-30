for input_file in inputs/*.ctds; 
do
    echo "Running test with input file: $input_file"
    ./compiler "$input_file"
done