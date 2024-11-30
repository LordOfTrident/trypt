<a name="readme-top"></a>
<div align="center">
	<a href="./LICENSE">
		<img alt="License" src="https://img.shields.io/badge/license-GPL v3-e8415e?style=for-the-badge">
	</a>
	<a href="https://github.com/LordOfTrident/trypt/graphs/contributors">
		<img alt="Contributors" src="https://img.shields.io/github/contributors/LordOfTrident/trypt?style=for-the-badge&color=f36a3b">
	</a>
	<a href="https://github.com/LordOfTrident/trypt/stargazers">
		<img alt="Stars" src="https://img.shields.io/github/stars/LordOfTrident/trypt?style=for-the-badge&color=efb300">
	</a>
	<a href="https://github.com/LordOfTrident/trypt/issues">
		<img alt="Issues" src="https://img.shields.io/github/issues/LordOfTrident/trypt?style=for-the-badge&color=0fae5e">
	</a>
	<a href="https://github.com/LordOfTrident/trypt/pulls">
		<img alt="Pull requests" src="https://img.shields.io/github/issues-pr/LordOfTrident/trypt?style=for-the-badge&color=4f79e4">
	</a>
	<br><br><br>
	<h1 align="center">trypt</h1>
	<p align="center">Linux file encrypting/decrypting tool</p>
	<p align="center">
		<a href="https://github.com/LordOfTrident/trypt/issues">Report Bug</a>
		·
		<a href="https://github.com/LordOfTrident/trypt/issues">Request Feature</a>
	</p>
	<br>
</div>

<details>
	<summary>Table of contents</summary>
	<ul>
		<li><a href="#introduction">Introduction</a></li>
		<li>
			<a href="#pre-requisites">Pre-requisites</a>
			<ul>
				<li><a href="#arch-linux">Arch Linux</a></li>
			</ul>
		</li>
		<li><a href="#quickstart">Quickstart</a></li>
		<li>
			<a href="#example">Example</a>
			<ul>
				<li><a href="#encrypting-folders">Encrypting folders</a></li>
			</ul>
		</li>
		<li><a href="#bugs">Bugs</a></li>
	</ul>
</details>

## Introduction
**trypt** is a file encrypting/decrypting tool for Linux systems. It is written in C99 and uses the
[libgcrypt](https://www.gnupg.org/software/libgcrypt/index.html) library for encryption and decryption.
Files are encrypted using the [AES](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard) 256 CFB algorithm.

> [!WARNING]
> This is the first time i did anything encryption-related, so you should probably use some professional
> software for encrypting personal data instead

## Pre-requisites
The following is required to compile the tool:
- A C99 compiler
- Makefile
- [libgcrypt](https://www.gnupg.org/software/libgcrypt/index.html)

### Arch Linux
```sh
$ pacman -S gcc make libgcrypt
```

## Quickstart
```sh
$ git clone https://github.com/LordOfTrident/trypt
$ make release
$ ./trypt
```

## Example
```sh
$ echo "Hello, world!" > a.txt
$ trypt en a.txt a.txt.en My16BytesLongKey
$ trypt de a.txt.en a.txt.de My16BytesLongKey
$ cat a.txt.de
Hello, world!
```
> [!NOTE]
> Keys have to be either 16, 24 or 32 bytes long

### Encrypting folders
Encrypting folders is probably a common thing, and for that you can use any archive file format like
[zip](https://en.wikipedia.org/wiki/ZIP_(file_format)) or [tar](https://en.wikipedia.org/wiki/Tar_(computing))
and then encrypt the generated archive file. I made a [script that combines trypt with tar to encrypt folders](trypt-tar),
which you can use or edit to use a different format. Example usage:
```sh
$ mkdir test
$ echo "Hello, world!" > test/a.txt
$ echo "Goodbye, world!" > test/b.txt
$ trypt-tar en test test.en My16BytesLongKey
$ trypt-tar de test.en test.de My16BytesLongKey
$ cat test.de/test/a.txt
Hello, world!
```

## Bugs
If you find any bugs, please, [create an issue and report them](https://github.com/LordOfTrident/trypt/issues).

<br>
<h1></h1>
<br>

<div align="center">
	<a href="https://en.wikipedia.org/wiki/C_(programming_language)">
		<img alt="C99" src="https://img.shields.io/badge/C99-0069a9?style=for-the-badge&logo=c&logoColor=white">
	</a>
	<a href="https://www.gnupg.org/software/libgcrypt/index.html">
		<img alt="libgcrypt" src="https://img.shields.io/badge/libgcrypt-0093dd?style=for-the-badge&logoColor=white">
	</a>
	<p align="center">Made with ❤️ love</p>
</div>

<p align="right">(<a href="#readme-top">Back to top</a>)</p>
