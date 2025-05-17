# 🐚 Mini Linux Shell (`mysh`)

A basic Unix/Linux shell written in C. This project mimics core functionalities of a typical shell, allowing users to run commands, use built-in utilities, handle input/output redirection, and single piping.

---

## 🚀 Features

- Execute standard Linux commands (e.g., `ls`, `pwd`, `echo`)
- Built-in commands:  
  - `cd` — change directory  
  - `exit` — terminate the shell
- Input redirection (`<`)
- Output redirection (`>`)
- Single pipe (`|`) between two commands

---

## 🗂️ Project Structure

```
mini-shell/
├── main.c         # Shell source code
├── mysh           # Compiled binary (ignored in .gitignore)
├── README.md      # Project documentation
└── .gitignore     # Git ignore rules
```

---

## 🛠️ How to Compile

Make sure you have GCC installed. Then compile with:

```bash
gcc main.c -o mysh
```

---

## ▶️ How to Run

Run the shell:

```bash
./mysh
```

You will see the shell prompt:

```bash
mysh>
```

---

## 💡 Example Commands

```bash
mysh> ls
mysh> pwd
mysh> cd ..
mysh> echo Hello > file.txt
mysh> cat < file.txt
mysh> ls | grep txt
mysh> exit
```

---

## 📦 Requirements

- GCC compiler
- Unix/Linux environment (tested on Kali Linux in VMware)

---

## 👨‍💻 Author

**Vigneshwaran Murugan**  
[🔗 GitHub Profile](https://github.com/VigneshwaranMurugan16/)

---

## 📝 License

This project is open-source and licensed under the MIT License.
