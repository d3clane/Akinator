# Akinator


## Installation 

```
git clone https://github.com/d3clane/Akinator.git
cd Akinator
make 
```

## Description

Wise Tree of knowledge that learns something new with each loose is provided in this repository.

Actually, there is a tree which contains different character traits and depending on the answer it chooses next question. If there is no such character, Wise Tree asks user to give one character trait and his name. After that, Wise Tree will never forget it.

Acquired skills:
- Graphviz
- Tree storing in prefix form

## Graphviz

Graphviz is used to created imgs of tree knowledge. So the user could know what does Wise Tree know and maybe add something on purpose using this information. There is an example of img:
![Graphviz Img](https://github.com/d3clane/Akinator/blob/main/ReadmeAssets/imgs/img_4_time_23%3A57%3A44.png)

## Tree storing

Tree is stored in text file in prefix form. To read from file there are two functions type:
- functions using [library](https://github.com/d3clane/Onegin) are used (they speed up input / output from files). They are faster than standard reading input however they use extra memory.
- functions without this lib. They are slower but doesn't use extra memory.
