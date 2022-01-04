tic=$(date +%s%N);
./a.out;
toc=$(date +%s%N);
t=$((toc-tic));
echo "time in nanoseconds: $t"
