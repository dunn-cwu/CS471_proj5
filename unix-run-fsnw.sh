set -e
mkdir -p results/fsnw-times

./build/release/cs471-proj5.out params/fsnw.ini

echo "All tests finished."
