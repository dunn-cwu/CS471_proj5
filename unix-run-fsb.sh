set -e
mkdir -p results/fsb-times

./build/release/cs471-proj5.out params/fsb.ini

echo "All tests finished."
