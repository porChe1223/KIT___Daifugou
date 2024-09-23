//
//  myplayer.cpp
//  PlayingCard
//
//  Created by 下薗 真一 on 09/04/12.
//  Modified by Kazutaka Shimada on 09/04/21.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//
//  Modified by Teigo Nakamura
//

#include <iostream>
#include <string>

#include "card.h"
#include "cardset.h"
#include "player.h"
#include "gamestatus.h"
#include "myplayer.h"

void MyPlayer::ready() {
    // memory.clear();
    // trump.clear();
    hand.sort(myCardCmp);
}

// 「(弱い方から順に) 出せるなら温存せずに出す！！」という貪欲戦略
bool MyPlayer::follow(const GameStatus &gstat, CardSet &cs) {
    CardSet pile(gstat.pile);
    Card tmp1;
    Card tmp2;
    Card tmp3;
    Card tmp4;
    int leadSize;

    hand.sort(myCardCmp);  // 手札をソート(弱い方から順に)

    leadSize = pile.size();

    if (leadSize == 0) { // 自分がリーダなら

      // 同じ番号のカードが4枚ある場合、全部出す
      for (int i = 0; i < hand.size(); i++) {
        tmp1 = hand.at(i);
        tmp2 = hand.at(i + 1);
        tmp3 = hand.at(i + 2);
        tmp4 = hand.at(i + 3);
        if(tmp1.rank() == tmp4.rank()){
            cs.insert(tmp1);// 同じカードを3枚追加
            cs.insert(tmp2); 
            cs.insert(tmp3); 
            cs.insert(tmp4); 
            hand.remove(tmp1);
            hand.remove(tmp2);
            hand.remove(tmp3);
            hand.remove(tmp4);
            return true;
        }
      }
      // 同じ番号のカードが3枚ある場合、全部出す
      for (int i = 0; i < hand.size(); i++) {
        tmp1 = hand.at(i);
        tmp2 = hand.at(i + 1);
        tmp3 = hand.at(i + 2);
        if(tmp1.rank() == tmp3.rank()){
            cs.insert(tmp1);// 同じカードを3枚追加
            cs.insert(tmp2); 
            cs.insert(tmp3);
            hand.remove(tmp1);
            hand.remove(tmp2);
            hand.remove(tmp3);
            return true;
        }
      }
      // 同じ番号のカードが2枚ある場合、両方出す
      for (int i = 0; i < hand.size(); i++) {
        tmp1 = hand.at(i);
        tmp2 = hand.at(i + 1);
        if(tmp1.rank() == tmp2.rank()){
            cs.insert(tmp1); // 同じカードを2枚追加
            cs.insert(tmp2); 
            hand.remove(tmp1);
            hand.remove(tmp2);
            return true;
        }
      }
      // 同じ番号のカードがない場合、一枚だけ出す
      tmp1 = hand.at(0); // 一番弱いカードを1枚出す
      cs.insert(tmp1);
      hand.remove(tmp1);
      return true;
    }
    else if (leadSize == 1) { // リードが1枚の場合
      for (int i = 0; i < hand.size(); i++) {
        tmp1 = hand.at(i);
        if (tmp1.isGreaterThan(pile.at(0))) {
          cs.insert(tmp1);
          hand.remove(tmp1);
          return true;
        }
      }
    }
    else { // 2枚以上の場合
      for (int i = 0; i < hand.size(); i++) {
        tmp1 = hand.at(i);
        if (tmp1.isGreaterThan(pile.at(0)) || tmp1.isGreaterThan(pile.at(1))) { // by Teigo Nakamura 2020/07/04
          cs.clear();
          cs.insert(tmp1);
          for (int j = 1; j < leadSize && i + j < hand.size(); j++) {
            if (hand.at(i).rank() == hand.at(i + j).rank())
              cs.insert(hand.at(i + j));
          }
          if (cs.size() == leadSize) { // 出せるカードが揃った
            hand.remove(cs); // カード集合を remove 
            return true;
          }
        }
      }
    }

    // 出せるカードがないのでパス
    cs.clear();
    return true;
}

bool MyPlayer::approve(const GameStatus &gstat) {
    //
    // 未実装
    // 
    return true;
}
