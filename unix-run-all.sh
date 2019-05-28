set -e

echo "----------------------"

./unix-run-fss.sh

echo "----------------------"

./unix-run-fsb.sh

echo "----------------------"

./unix-run-fsnw.sh

echo "----------------------"

echo "All tests finished."

