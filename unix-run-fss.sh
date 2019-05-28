set -e
mkdir -p results/fss-times

./build/release/cs471-proj5.out params/fss.ini

echo "All tests finished."
