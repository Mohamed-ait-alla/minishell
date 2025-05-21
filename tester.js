/* ************************************************************************** */
/*                                                                            */
/*         Created with love by:        Mohamed Dahani & Mohamed Ait Alla     */
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
  console.error('\x1b[1m\x1b[33m\nUsage: node file.js "minishell$> "\n\x1b[0m');
  process.exit(1);
}

// Add your test commands here
const tests = [
  // Empty & whitespace
  ``,
  ` `,
  `# comment`,
  `;`,
  `...`,
  `.............................................................`,
  `;;`,
  `;;;`,
  `;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;`,
  `                                                                                                                              `,
  ``,
  ``,
  ``,
  ``,
  ``,
  ``,
  ``,
  ``,
  ``,
  ``,
  ` `,
  ` `,
  ` `,
  ` `,
  ` `,
  ` `,
  ` `,
  ` `,
  ` `,
  ` `,

  // Quotes (single, double, unbalanced)
  `'`,
  `"`,
  `echo 'unterminated`,
  `echo "unterminated`,
  `echo "hello 'world'"`,
  `echo 'hello "world"'`,
  `echo 'one'"two"'three'"four"`,
  `echo "a"'b'"c"'d'"e'`,
  `echo """"'''''`,
  `echo "> >> < * ? [ ] | ; [ ] || && ( ) & # $  <<"`,
  `echo '> >> < * ? [ ] | ; [ ] || && ( ) & # $  <<'`,
  `echo Hello" "World"" "'from the'"harder 'test "'"=>"'"'"''"'if passed this 'your good' 'but "' the next one" will '"be"'"more 'harder'"""`,

  // Expansion
  `echo $USER`,
  `echo "$USER"`,
  `echo '$USER'`,
  `echo $HOME$USER$SHELL`,
  `echo "$HOME$USER$SHELL"`,
  `echo '$HOME$USER$SHELL'`,
  `echo $NOT_EXISTING`,
  `echo "$NOT_EXISTING"`,
  `echo "$USER$NOT_EXISTING"`,
  `echo $USER'$USER'$USER`,
  `echo $USER$`,
  `echo $USER_`,
  `echo $USER$USER$`,
  `echo "$USER"extra`,
  `echo $""`,
  `echo $"USER"`,
  `echo $'USER'`,
  `echo \$USER`,
  `echo "hello '$USER'"`,
  `echo '$USER'"$USER"`,
  `echo $USE""R`,
  `echo $USE''R`,
  `echo Hello" "World"" "'$USER'"$11337 'test "'"=>"'"'"''"'if passed this $USER"$USER"$USER'your Mr.$USER " $NOTHING"' 'but "' the next one" will '"be"'"more 'harder $?'"""" " but 'you have" "' "to "'handle "exit 'status'"'' '$?`,

  // export test
  `export var1="hello " var2=world`,
  `echo $var1$var2`,
  `echo "$var1$var2"`,
  `echo '$var1$var2'`,
  `echo "'$var1$var2'"`,
  `echo '"$var1$var2"'`,
  `echo '"'$var1'"$var2"'`,
  `echo '$var1'"$var2"`,
  `unset var1 var2`,
  `echo $var1$var2`,
  `export var3="ls                                                                -l"`,
  `$var`,
  `'$var'`,
  `"$var"`,
  `echo $?`,
  `echo $`,
  `echo $2025`,
  `echo $%`,
  `echo $_`,
  `export var4="hello"`,
  `var4+=" world"`,
  `echo var4`,
  `var4-=" world"`,
  `echo var4`,
  `var4*=" world"`,
  `echo var4`,
  `var4/=" world"`,
  `echo var4`,
  `echo $?`,
  `export xyz`,
  `echo $xyz`,
  `export abc=`,
  `echo $abc`,
  `echo $?`,
  `unset`,
  `unset HELLO `,
  `unset HELLO1 HELLO2 `,
  `unset HOME `,
  `unset PATH `,
  `unset SHELL `,
  `cd $PWD`,
  `cd 123123`,

  // cmds
  // test mjnoun hhhhh
  `mkdir a && cd a && mkdir b && cd b && mkdir c && cd c && rm -rf ../../../a`,
  "pwd",
  `ls`,
  `ls -l`,
  `ls                                   -l`,
  `ls -la`,
  `pwd`,
  `cd ..`,
  `cd`,

  // Pipes & Redirections
  `|`,
  `||`,
  `| |`,
  `    | |    `,
  `|| ls`,
  `| | ls`,
  `ls | | ls`,
  `ls |`,
  `ls | |`,
  `cat < Makefile`,
  `cat < non_existing_file`,
  `> file`,
  `< file`,
  `ls | | wc`,
  `cat file1.txt`,
  `cat file2.txt`,
  `echo hello >>> file3.txt`,
  `cat file3.txt`,
  `echo hello >>> `,
  `echo hello < file4.txt`,
  `cat file4.txt`,
  `cat file5.txt`,
  `cat file6.txt`,
  `echo hello <<< `,

  `cat test.txt`,
  `grep eBook < test.txt`,
  `grep 1337 >> test.txt`,
  'echo eBook | cat "test.txt"',
  `echo test > $NOTHINHG`,

  // pipes
  `echo hello | cat`,
  `ls -l | grep .c | wc -l`,
  `env | sort | grep -v SHLVL | grep -v ^_ `,
  `cat ./test_files/infile_big | grep oi `,
  `cat minishell.h | grep ");"$ `,
  `export GHOST=123 | env | grep GHOST `,
  `./minishell | ls`,

  // Exit tests
  `exit`,
  `echo "exit_code ->$? user ->$USER home -> $HOME"`,
  `exit 0`,
  `echo $?`,
  `exit +42`,
  `echo $?`,
  `exit -42`,
  `echo $?`,
  `exit +256`,
  `echo $?`,
  `exit -256`,
  `echo $?`,
  `exit -1`,
  `echo $?`,
  `exit -255`,
  `echo $?`,
  `exit +-75`,
  `echo $?`,
  `exit -+24`,
  `echo $?`,
  `exit ++25`,
  `echo $?`,
  `exit --55`,
  `echo $?`,
  `exit 123`,
  `echo $?`,
  `exit 298 `,
  `echo $?`,
  `exit +100 `,
  `echo $?`,
  `exit "+100"`,
  `echo $?`,
  `exit +"100"`,
  `echo $?`,
  `exit -100`,
  `echo $?`,
  `exit "-100" `,
  `echo $?`,
  ` exit -"100" `,
  `echo $?`,
  `exit hello `,
  `echo $?`,
  ` exit 42 world`,
  `echo $?`,

  // Combined
  `echo "$USER" | grep . | wc -c > out.txt`,

  // Syntax errors
  `| echo`,
  `echo |`,
  `echo >`,
  `echo <`,
  `echo >>`,
  `echo <<<`,

  // minishell in minishell
  `./minishell | ls`,

  // Minishell Tests
  // Execution Tests
  `ls`,
  `/usr/bin/ls`,
  `usr/bin/ls`,
  `./ls`,
  `hello`,
  `/usr/bin/hello`,
  `""`,
  `$`,
  `../existing_dir`,
  `../fake_dir`,
  `./Makefile`,
  `./does_not_exist`,
  `minishell`,
  `.minishell`,

  // CD
  `cd ../../../../../..`,
  `cd /home/user/`,
  `cd $HOME`,
  `cd $HOME/Documents`,
  `unset HOME, then cd`,
  `export HOME= then cd`,
  `cd /t /w`,
  `cd ./fakepath`,
  `unset OLDPWD; cd -`,
  `mkdir a; mkdir a/b; cd a/b; rm -r ../../a; cd ..`,

  // ECHO
  `echo -n -n -nnnn -nnnnm`,
  `echo a -nnnnma`,
  `echo -n -nnn hello -n`,
  `echo a hello -na`,

  // EXPORT
  `export hello`,
  `export var=foo`,
  `echo $var $foo`,
  `export t="abc def"`,

  // UNSET
  `unset PATH`,
  `ls (after unset PATH)`,
  `unset "" test`,
  `unset =`,
  `unset FAKEVAR`,
  `export var1=test`,
  `unset var (following var1)`,

  // EXIT
  `exit 10`,
  `exit -10`,
  `exit abc`,
  `exit --10`,
  `exit ++10`,
  `exit abc 5`,
  `exit 5 abc`,
  `exit 5 < Makefile`,
  `exit 8 > test`,
  `ls | exit`,
  `ls | exit 42`,
  `ls | exit 12 abc`,
  `ls | exit abc 12`,
  `exit | ls`,
  `exit 42 | ls`,
  `exit 12 abc | ls`,
  `exit abc 12 | ls`,
  `ls > file | exit`,
  `sleep 5 | exit`,
  `ls -l > x | exit | wc -l`,
  `exit | ls`,
  `exit 1 | exit 0`,
  `exit 0 | exit 1`,

  // Pipe tests
  `cat | cat | cat | ls`,
  `cat Makefile | grep a | wc -l | cd x`,
  `cat Makefile | grep a | wc -l | x`,
  `echo test |cat`,
  `echo test ||| cat`,
  `export A=1 B=2 C=3 D=4 E=5 F=6 G=7 H=8`,

  // Variable Expansion Tests
  `echo $USER`,
  `echo $`,
  `echo $""`,
  `echo $USER$USER`,
  `echo $USER""$USER`,
  `echo $USER" "$USER`,
  `echo test$1test`,
  `echo test$FAKE_VAR`,
  `echo "$USER>>"`,
  `echo "$USER11111ffjf"`,
  `echo $\"echo`,
  `echo "test$<test"`,
  `echo test$<test`,
  `echo $1`,
  `echo "$1"`,
  `echo $"USER"`,
  `echo $'USER'`,
  `echo $NULL test`,
  `echo hello$USER`,
  `echo hello$USERtest`,
  `echo $USER.test`,
  `echo "'$fake'"`,
  `echo """$fake"""`,
  `echo "'"$fake"'"`,
  `echo '"'"$fake"'"'`,
  `echo $"fake"`,

  // Quote Handling Tests
  `ec""ho test`,
  `""echo test`,
  `echo"" test`,
  `echo "" test`,
  `echo ""$USER`,
  `echo "$"USER`,
  `echo $""USER`,
  `ec''ho test`,
  `''echo test`,
  `echo'' test`,
  `echo '' test`,
  `echo ''$USER`,
  `echo '$'USER`,
  `echo $''USER`,
  `echo "" "" "" test`,
  `echo """""" test`,
  `echo $USER"" ''`,
  `ls ""`,
  `ls '"`,
  `ls "'`,
  `ls " "`,
  `ls " ' "`,
  `"ls"`,
  `l"s"`,
  `ls"" -la`,
  `ls "" -la`,
  `ls """"-la`,
  `ls ""-la`,
  `echo test""test`,
  `echo test ""test`,
  `echo test "'"test`,
  `echo "|" ls`,
  `echo "t | es <>t"`,
  `echo '"abc"'`,
  `echo ' "abc" '`,
  `echo "'abc'"`,

  // Redirection Tests
  `< hello`,
  `cat <t`,
  `> x`,
  `< Ma < Makefile cat`,
  `<a cat <b <c`,
  `<a cat <b <c`,
  `>a cat <b >>c`,
  `>a ls >b >>c >d`,
  `>a ls <e >>b >c`,
  `cat -e >t1 <t2`,
  `fakecmd hello > z`,
  `ls > a > b -l`,
  `ls > p | env > q`,
  `chmod 000 b`,
  `chmod 644 b; rm a`,
  `rm a b c`,
  `< Ma < Maa echo a`,
  `echo a < Ma < Maa`,
  `echo hello < txt sweetie (txt exist)`,
  `ls > e | cat<e`,
  `cat<f | ls >f`,
  `echo hello > a | echo fuck > a`,
  `echo hello > a > b | echo fuck >b`,
  `echo hello > b| echo ddd>> b`,

  // Signal Tests
  `ctrl+c`,
  `abc + ctrl + c`,
  `ctrl+d`,
  `abc + ctrl+d`,
  `ctrl+\\`,
  `abc + ctrl+\\`,
  `cat + enter + ctrl+c`,
  `cat + enter + ctrl+d`,
  `cat + enter + ctrl+\\`,

  // Other Syntax Error Tests
  `|`,
  `ls |`,
  `ls < |`,
  `ls << |`,
  `ls > |`,
  `ls >> |`,
  `ls | <`,
  `ls | <<`,
  `ls | >`,
  `ls | >>`,
  `ls >> >`,
  `ls > >>`,
  `ls << <`,
  `ls < <<`,
  `ls > >> |`,
  `ls >>`,
  `echo test > > out`,
  `echo test >> > out`,
  `echo hello > $fakevar`,
  `echo hello >> $fakevar`,
  `echo hello < $fakevar`,
  `cat < $fakevar`,
  `echo hello > $realvar`,
  `echo hello >> $realvar`,
  `< $realvar cat`,
  `export t="abc def"`,
  `echo test > $t"`,
  `echo test > "$t"`,
  `echo hello >>> test`,
  `echo hello | |`,
  `echo hello |;`,
  `echo a`,
  `<`,
  `<<`,
  `<<<`,
  `<<<<`,
  `>`,
  `>>`,
  `>>>`,
  `>>>`,

  // Other Exit Status Tests
  `fake_cmd`,
  `ls not_a_path`,
  `./not_executable_file`,
  `cat clac 3`,
  `cd $?`,
  `cat |||`,

  // remove all files
  `rm -rf file`,
  `rm -rf file1.txt`,
  `rm -rf file2.txt`,
  `rm -rf out.txt`,
  `rm -rf test.txt`,
  `rm -rf a b d e f p q t1 test x z`,
];

const runCommand = (shell, command) => {
  try {
    const result = spawnSync(shell, [], {
      input: command,
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
  let minishellOutput = runCommand(minishellPath, command)
    .replace(programName, "")
    .replace("\n", "")
    .replace(programName, "")
    .replace(command, "")
    .replace("exit\n", "");
  if (!command) {
    minishellOutput = "";
  }
  // console.log(`========>${minishellOutput}`);
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
    let totalCommands = tests.length;
    let totalCommandsFailed = commandsFailed.length;
    let grade = ((totalCommands - totalCommandsFailed) / totalCommands) * 100;

    let color;
    if (grade >= 70) color = "\x1b[32m";
    else if (grade >= 50) color = "\x1b[33m";
    else color = "\x1b[31m";

    console.log(`${color}\x1b[1m╭──────────────────────────────╮`);
    console.log(`│  Your Grade is: ${grade.toFixed(2)}%       │`);
    console.log(`╰──────────────────────────────╯\x1b[0m`);

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
    "\n\x1b[4m\x1b[34mCreated with love by: Mohamed Dahani & Mohamed Ait Alla ❤️\x1b[0m\n"
  );
  console.log("\x1b[4m\x1b[34mGitHub: https://github.com/dahani-dev\x1b[0m\n");
  console.log("\x1b[4m\x1b[34mGitHub: https://github.com/Mohamed-Ait-Alla\x1b[0m\n");
};

runTests();
