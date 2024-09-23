//
//  myplayer.h
//  PlayingCard
//
//  Created by 下薗 真一 on 09/04/12.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//
//  Modified by Teigo Nakamura
//

#ifndef _MYPLAYER_H_
#define _MYPLAYER_H_

#include <random>

#include "player.h"
#include "gamestatus.h"

//
// すこしルールを覚えたプレイヤーのクラス
// Player クラスを継承して作成するクラスのサンプル
//
class MyPlayer : public Player {
    CardSet memory;  // 色々覚えておくための作業用
    CardSet trump;   // 同上

public:
    MyPlayer(const char *name = "Me") : Player(name) {
      // 必要ならば初期化を書く．
      // 基底クラス Player の初期化の継承部分（ : の後の Player(name)　の部分）
      // は変更しない．name = の後の部分は，デフォルト（引数を省略した場合の標準引数値）
      // として，他のグループと違う名前をつけるとよい．
    }
    ~MyPlayer() { }

    // 思考処理の関数：このクラスで実装する
    void ready(void);
    bool follow(const GameStatus &, CardSet &);
    bool approve(const GameStatus &);
    int count(const CardSet cset, int rank);

    // 比較関数(ソート用)
    static bool myCardCmp(const Card &a, const Card &b) {
      return a.strength() <= b.strength();
    }

};

#endif

