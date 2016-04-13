a=($(wc $1))
lines=${a[0]}
words=${a[1]}

echo "$lines $words"
