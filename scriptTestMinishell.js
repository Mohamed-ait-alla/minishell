const path = require("path");
const { spawnSync } = require("child_process");
const fs = require("fs");

const minishellPath = "./minishell";
const logDir = "./logs";
const testOutput = path.join(logDir, "test_output.txt");
const refOutput = path.join(logDir, "bash_output.txt");

const tests = ["$USER"];

const runCommand = (shell, command) => {
  try {
    const result = spawnSync(shell, [], {
      input: command + "\n",
      encoding: "utf-8",
      shell: false,
    });
    return result.stdout;
  } catch (error) {
    return `Error: ${error.message}`;
  }
};

const comparisonCommands = (command) => {
  const bashOutput = runCommand("bash", command);
  const minishellOutput = runCommand(minishellPath, command);
  fs.writeFileSync(refOutput, bashOutput);
  fs.writeFileSync(testOutput, minishellOutput);
  if (bashOutput !== minishellOutput) {
    console.log(`\x1b[31m[FAIL]\x1b[0m Command: ${command}`);
    printDiff(bashOutput, minishellOutput);
  } else {
    console.log(`\x1b[32m[OK]\x1b[0m   Command: ${command}`);
  }
};

const printDiff = (expected, actual) => {
  const expectedLines = expected.split("\n");
  const actualLines = actual.split("\n");

  for (let i = 0; i < expectedLines.length; i++) {
    if (expectedLines[i] !== actualLines[i]) {
      console.log(`\x1b[33m- Bash:      ${expectedLines[i] || "<empty>"}`);
      console.log(`+ Minishell: ${actualLines[i] || "<empty>"}\x1b[0m`);
    }
  }
};

const runTests = () => {
  if (!fs.existsSync(minishellPath)) {
    console.error("❌ ./minishell not found. Make sure it is compiled.");
    process.exit(1);
  }
  fs.mkdirSync(logDir, { recursive: true });
  console.log("Running Minishell tests...");
  for (const command of tests) {
    comparisonCommands(command);
  }
  console.log("Testing complete.");
};

runTests();
