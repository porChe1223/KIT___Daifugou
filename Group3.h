//
//  Group3.h
//  PlayingCard
//
//  Created by 下薗 真一 on 09/04/12.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//
//  Modified by Teigo Nakamura
//

#ifndef _Group3_H_
#define _Group3_H_

#include <random>

#include "player.h"
#include "gamestatus.h"

//
// すこしルールを覚えたプレイヤーのクラス
// Player クラスを継承して作成するクラスのサンプル
//
class Group3 : public Player {
    CardSet memory;  // 色々覚えておくための作業用
    CardSet trump;   // 同上
    int hantei = 0;

public:
    Group3(const char *name = "Group3") : Player(name) {
      // 必要ならば初期化を書く．
      // 基底クラス Player の初期化の継承部分（ : の後の Player(name)　の部分）
      // は変更しない．name = の後の部分は，デフォルト（引数を省略した場合の標準引数値）
      // として，他のグループと違う名前をつけるとよい．
    }
    ~Group3() { }

    // 思考処理の関数：このクラスで実装する
    void ready(void);
    bool follow(const GameStatus &, CardSet &);
    bool approve(const GameStatus &);
    int count(const CardSet cset, int rank);
    bool strongest(const CardSet cset, Card c, int num);
    bool judgeStrongestNum(const CardSet cset);
    bool follow2(const GameStatus &gstat, CardSet &cs) ;

    // 比較関数(ソート用)
    static bool myCardCmp(const Card &a, const Card &b) {
      return a.strength() <= b.strength();
    }

};

#endif