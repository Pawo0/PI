gcc $1
for file in $2/*.in; do
  start=`date +%s%N`
  test=${file%.in}.out
  diff -bsq $test <(./a.out < $file)
  end=`date +%s%N`
#  printf "%.3f%s\n" $(((end-start)/(10**6)))e-3 s
#  printf "your output: \n%s\n" "$(./a.out < $file)"
#  printf "excepted output: \n"
#  cat $test
#  printf "\n"

done
