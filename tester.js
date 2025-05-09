/* ************************************************************************** */
/*                                                                            */
/*                Created with love by:               Mohamed Dahani          */
/*                                                                            */
/* ************************************************************************** */

const path = require("path");
const { spawnSync } = require("child_process");
const fs = require("fs");

const minishellPath = "./minishell";
const logDir = "./logs";
const testOutput = path.join(logDir, "test_output.txt");
const refOutput = path.join(logDir, "bash_output.txt");
let errorStatus = false;
let commandsFailed = [];
let programName = process.argv[2];
let syntaxErrorOfSquote = "";
let syntaxErrorOfDquote = "";

if (!programName) {
  console.error(
    "\x1b[1m\x1b[31m\n⚠️  Please enter your minishell prompt exactly, including any trailing space (e.g., minishell$> ).\x1b[0m"
  );
  console.error(
    '\x1b[1m\x1b[33m\nUsage: node tester.js "minishell$> "\n\x1b[0m'
  );
  process.exit(1);
}

// Add your test commands here
const tests = [
  // tests of quotes
  "",
  '"',
  "'",
  "echo ''",
  'echo ""',
  "echo ' '",
  'echo " "',
  "echo hello",
  "echo 'hello world'",
  'echo "hello world"',
  "echo 'hello\"world'",
  'echo "hello\'world"',
  "echo 'hello'\"world\"",
  "echo \"hello\"'world'",
  "echo \"'hello'\"",
  "echo \"'hello'\"\"\"",
  "echo \"'hello'\'\'\"",
  "echo '\"hello\"'",
  "echo '\"hello\"\"\"'",
  "echo '\"hello\"\'\''",
  // test of expanding
  'echo "hello $USER',
  "echo 'hello $USER'",
  'echo "hello $USER"',
  "echo '$USER $USER'",
  'echo "$USER $USER"',
  "echo 'hello $USER' world",
  "echo \"hello 'world' $USER\"",
  "echo 'hello \"world\" $USER'",
  "echo \"$USER 'hello' world\"",
  "echo \"hello $USER\"' world'",
  "echo 'hello' \"world $USER\"",
  'echo "hello world $USER"',
  'echo ""$USER""',
  "echo '$USER'$USER\"$USER\"",
  "echo 'hello' \"world\" '\"$USER\"'",
  'echo "hello" "world"',
  "echo $USER'$USER'$USER",
  "echo $'USER'",
  "echo $\"USER\"",
  // test cmds
  "ls",
  "pwd",
  "cd ..",
  "cd .",
  "cd /",
  "cd /tmp",
  "export VAR=value",
  "unset VAR",
  // test exit status
  "echo $?",
  // test exit
  "exit",
];

const runCommand = (shell, command) => {
  try {
    const result = spawnSync(shell, [], {
      input: command,
      encoding: "utf-8",
      shell: false,
    });
    if (command === "exit" && shell === "bash") {
      return "exit\n";
    }
    return result.stdout;
  } catch (error) {
    return `Error: ${error.message}`;
  }
};

const comparisonCommands = (command) => {
  const bashOutput = runCommand("bash", command);
  let minishellOutput = runCommand(minishellPath, command)
    .replace(programName, "")
    .replace("\n", "")
    .replace(programName, "")
    .replace(command, "");
  if (command === '"' || command === "'") {
    syntaxErrorOfDquote = minishellOutput;
    syntaxErrorOfSquote = minishellOutput;
  }
  if (
    minishellOutput === syntaxErrorOfDquote ||
    minishellOutput === syntaxErrorOfSquote
  ) {
    minishellOutput = "";
  }
  if (bashOutput !== minishellOutput) {
    fs.mkdirSync(logDir, { recursive: true });
    fs.writeFileSync(refOutput, bashOutput);
    fs.writeFileSync(testOutput, minishellOutput);
    console.log(`\x1b[31m[FAIL]\x1b[0m Command: ${command}`);
    commandsFailed.push(command);
    printDiff(bashOutput, minishellOutput);
    errorStatus = true;
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
    console.log(
      "\n\x1b[1m\x1b[31m❌ ./minishell not found. Make sure it is compiled.\x1b[0m\n"
    );
    process.exit(1);
  }
  console.log("\n\x1b[1m\x1b[37m🐚 Running Minishell tests...\x1b[0m\n");
  for (const command of tests) {
    comparisonCommands(command);
  }

  console.log("\n\x1b[1m\x1b[37m🧪 Testing complete.\x1b[0m\n");

  if (errorStatus) {
    console.log(
      "\x1b[31m\x1b[1m╭─────────────────────────────────────────────────╮"
    );
    console.log(
      "\x1b[31m\x1b[1m│  ❌ tests failed. Please check the logs!        │"
    );
    console.log(
      "\x1b[31m\x1b[1m╰─────────────────────────────────────────────────╯\x1b[0m"
    );
    console.log("\x1b[1m\x1b[37m\n🚨 Commands that failed:\x1b[0m\n");
    for (let i = 0; i < commandsFailed.length; i++) {
      console.log(
        `\x1b[31m${i + 1}.\x1b[0m \x1b[1m\x1b[37m${commandsFailed[i]}\x1b[0m`
      );
    }
  } else {
    console.log("\x1b[32m\x1b[1m╭──────────────────────────────╮");
    console.log("\x1b[1m│  ✅ All tests passed!        │");
    console.log("\x1b[32m╰──────────────────────────────╯\x1b[0m");
  }
  console.log(
    "\n\x1b[4m\x1b[34mCreated with love by: Mohamed Dahani ❤️\x1b[0m\n"
  );
  console.log("\x1b[4m\x1b[34mGitHub: https://github.com/dahani-dev\x1b[0m\n");
};

runTests();
