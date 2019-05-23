# Simple-Neural-Network
Sample of 2-layer categorization-neural-network(CNN).  
This CNN consists of 2-layer(input-layer, output-layer). not exist hidden-layer.  
2層カテゴリ分類ニューラルネットワークのサンプル．  
入力層と出力層のみ．隠れ層はなし．  

---
このニューラルネットワークは，mnistと呼ばれる手書き数字データを学習・分類します．  
実行する場合は，[mnist](http://yann.lecun.com/exdb/mnist/)のサイトから
- train-images-idx3-ubyte.gz
- train-labels-idx1-ubyte.gz
- t10k-images-idx3-ubyte.gz
- t10k-labels-idx1-ubyte.gz

の4つのファイルをダウンロードおよび解凍し，src/mnistの中へ入れてください．  

このプログラムは，『Windows10 64bit』『GCC version 6.3.0』の下で動作確認を行っています．  
コンパイルを行うには次のコマンドを実行します．  
```gcc -lm snnet.c main.c -o snnet```

教師データを基に学習を行うには次のコマンドを実行します．  
```./snnet.exe learn```

テストデータを基に識別を行うには次のコマンドを実行します．  
```./snnet.exe test```
