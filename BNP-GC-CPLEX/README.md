
<a href="https://en.wikipedia.org/wiki/Branch_and_price"><img src="https://kuniga.files.wordpress.com/2011/10/envoltoria.png" title="BranchAndPrice" alt="BranchAndPrice"></a>


# Column Generation - Using Cplex

## Solving Bin Packing Using Column Generation

All instances are taken from <a href="http://people.brunel.ac.uk/~mastjjb/jeb/info.html" target="_blank">`OR-Library`</a>

>Bin packing - one-dimensional
>There are currently 8 data files.
>
>These files contain the instances of the bin packing problem considered in
>E.Falkenauer (1994) "A Hybrid Grouping Genetic Algorithm for Bin Packing"
>Working paper CRIF Industrial Management and Automation, CP 106 - P4, 50
>av. F.D.Roosevelt, B-1050 Brussels, Belgium, email: efalkena@ulb.ac.be.
>
>These data files are binpack1, binpack2, ..., binpack8 and were
>contributed by E. Falkenauer.
>
>The format of these data files is as follows:
>Number of test problems (P)
>For each test problem (p=1,...,P) in turn:
>   Problem identifier
>   Bin capacity, Number of items (n), Number of bins in the current
>                                      best known solution
>   For each item i (i=1,...,n): size of the item
>
>There are 2 classes of bin packing instances. The first class, files
>binpack1 to binpack4 (problem identifiers beginning with 'u') consists of
>items of sizes uniformly distributed in (20,100) to be packed into bins of
>size 150.
>
>The second class, files binpack5 to binpack8 (problem identifiers beginning
>with 't') consists of 'triplets' of items from (25,50) to be packed into
>bins of size 100.
>
>For the 'uniform' class, the value for "Number of bins in the current best
>known solution" is the one found by algorithm in the above-referenced
>paper. Except for problems u120_08, u120_19, u250_07, u250_12 and u250_13,
>this is also the smallest number of bins capable of accommodating all the
>items, so the value is the proven optimum.
>
>For the 'triplets' class, the instances were constructed with a known
>global optimum of n/3 bins, i.e. the guaranteed optimal solution has
>exactly three items per bin.
>
>Acknowledgement: Although the procedure of generating 'triplets' of a known
>optimum is due to Falkenauer, Andre van Vliet from the Econometric
>Institute, Erasmus University Rotterdam, has first suggested the idea of
>instances of that nature.
>
>The largest file is binpack4 of size 60Kb (approximately).
>The entire set of files is of size 200Kb (approximately).

[![Build Status](http://img.shields.io/travis/badges/badgerbadgerbadger.svg?style=flat-square)](https://travis-ci.org/badges/badgerbadgerbadger) 
[![Coverage Status](http://img.shields.io/coveralls/badges/badgerbadgerbadger.svg?style=flat-square)](https://coveralls.io/r/badges/badgerbadgerbadger) 
[![Gem Version](http://img.shields.io/gem/v/badgerbadgerbadger.svg?style=flat-square)](https://rubygems.org/gems/badgerbadgerbadger) 
[![License](http://img.shields.io/:license-mit-blue.svg?style=flat-square)](http://badges.mit-license.org) 

---

## Table of Contents

- [Installation](#installation)
- [Usage](#Usage)
- [Contributing](#contributing)
- [Support](#support)
- [License](#license)

---

## Installation

### Clone

- Clone this repo to your local machine using `https://github.com/fvcproductions/SOMEREPO`

### Setup


> build the application

```shell
$ make build
```

---

### Usage
Go to build/apps/ directory and run

```shell
$ ./program <path_to_instance>
```

If you are in linux you can use makefile (modify it according to your cplex version)

```shell
$ make <instance>
```
Were instance can be any instance name in data folder:
```shell
$ make IN02_120_150u
```
---

## Documentation

- <a href="http://www2.ic.uff.br/elavio/mini1.pdf" target="_blank">`Eduardo Uchoa`</a>
- <a href="http://repositorio.unicamp.br/bitstream/REPOSIP/321614/1/Borges_YulleGlebbyoFelipe_M.pdf" target="_blank">`Yulle Glebbyo Felipe Borges`</a>

---
## Contributing

### Step 1

- **Option 1**
    - 🍴 Fork this repo!

- **Option 2**
    - 👯 Clone this repo to your local machine using `git clone https://github.com/JhonnathaMonteiro/ppgi-mestrado-p2`

### Step 2

- **HACK AWAY!** 🔨🔨🔨

### Step 3

- 🔃 Create a new pull request using <a href="https://github.com/JhonnathaMonteiro/ppgi-mestrado-p2" target="_blank">`https://github.com/JhonnathaMonteiro/ppgi-mestrado-p2`</a>.

---

## Support

Reach out to me at one of the following places!

- LinkedIn at <a href="https://www.linkedin.com/in/jhonnatha-andrade-144493a3/" target="_blank">`Jhonnatha Andrade`</a>

---

## Donations

<noscript><a href="https://liberapay.com/Jhonnatha/"><img alt="Donate using Liberapay" src="https://liberapay.com/assets/widgets/donate.svg"></a></noscript> \
<img src="http://img.shields.io/liberapay/goal/Jhonnatha.svg?logo=liberapay">


---

## License

[![License](http://img.shields.io/:license-mit-blue.svg?style=flat-square)](http://badges.mit-license.org)

- **[MIT license](http://opensource.org/licenses/mit-license.php)**
- Copyright 2020 © <a href="https://github.com/JhonnathaMonteiro" target="_blank">Jhonnatha de Andrade Monteiro</a>.