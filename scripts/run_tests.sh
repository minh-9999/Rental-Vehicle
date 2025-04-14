
#!/bin/bash
set -e

# Check if pipefail is supported (e.g. not on PowerShell)
(set -o pipefail) 2>/dev/null && set -o pipefail

echo "📁 Preparing logs directory..."
mkdir -p build/logs

echo "📦 Moving to build directory..."
cd build

echo "🧪 Running Unit Tests..."
ctest -R "Unit" --output-on-failure | tee logs/unit_test.log

echo "🔌 Running Integration Tests..."
ctest -R "Integration" --output-on-failure | tee logs/integration_test.log

echo "📦 Combining test logs..."
cat logs/unit_test.log logs/integration_test.log > logs/test.log || true

echo "✅ All tests executed and logs saved."
