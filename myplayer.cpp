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
          if(hand.size() == 4){ //手札残りがこれらだけの場合複数枚出す(Jkr使用可)
            cs.insert(tmp1);// 同じカードを3枚追加
            cs.insert(tmp2); 
            cs.insert(tmp3);
            cs.insert(tmp4);
            hand.remove(tmp1);
            hand.remove(tmp2);
            hand.remove(tmp3);
            hand.remove(tmp4);
            return true;
          } else if(tmp1.rank() >= 2 && tmp1.rank() <= 11){ //3～K以外ならば複数枚出す
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
      }
      // 同じ番号のカードが3枚ある場合、全部出す
      for (int i = 0; i < hand.size(); i++) {
        tmp1 = hand.at(i);
        tmp2 = hand.at(i + 1);
        tmp3 = hand.at(i + 2);
        if(tmp1.rank() == tmp3.rank()){
          if(hand.size() == 3){ //手札残りがこれらだけの場合複数枚出す(Jkr使用可)
            cs.insert(tmp1);// 同じカードを3枚追加
            cs.insert(tmp2); 
            cs.insert(tmp3);
            hand.remove(tmp1);
            hand.remove(tmp2);
            hand.remove(tmp3);
            return true;
          } else if(tmp1.rank() >= 2 && tmp1.rank() <= 11){ //3～K以外ならば複数枚出す
            cs.insert(tmp1);// 同じカードを3枚追加
            cs.insert(tmp2); 
            cs.insert(tmp3);
            hand.remove(tmp1);
            hand.remove(tmp2);
            hand.remove(tmp3);
            return true;
          }
        }
      }
      // 同じ番号のカードが2枚ある場合、両方出す
      for (int i = 0; i < hand.size(); i++) {
        tmp1 = hand.at(i);
        tmp2 = hand.at(i + 1);
        if(tmp1.rank() == tmp2.rank()){
          if(hand.size() == 2){ //手札残りがこれらだけの場合複数枚出す(Jkr使用可)
            cs.insert(tmp1); // 同じカードを2枚追加
            cs.insert(tmp2); 
            hand.remove(tmp1);
            hand.remove(tmp2);
            return true;
          } else if(tmp1.rank() >= 2 && tmp1.rank() <= 11 && tmp1.rank() != 15){ //3～K、Jkr以外ならば複数枚出す
            cs.insert(tmp1); // 同じカードを2枚追加
            cs.insert(tmp2); 
            hand.remove(tmp1);
            hand.remove(tmp2);
            return true;
          }
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

// カードセットcsetの中に、数字がrankのカードは何枚あるか
int MyPlayer::count(const CardSet cset, int rank) {
  int i, n=0;

  for(i=0; i<cset.size(); i++) {
    if(cset.at(i).rank() == rank) {
      n++;
    }
  }

  return n;
}

bool MyPlayer::approve(const GameStatus &gstat) {
  // hand.sort(myCardCmp); // 現在の手札をソートする（弱い方から順に）
  // CardSet pile(gstat.pile); // 現在の札場のカードを取得する

  // // ここから、相手の手札を予測するロジックを追加する
  // // まず、全カードリストを作成し、すでに出されたカードと自身の手札を除く
  // CardSet allCards; // 全カードセットを作成
  // allCards.remove(memory); // すでに出されたカードを除く
  // allCards.remove(hand); // 自分の手札を除く

  // // これが相手の手札の可能性があるカードセット
  // CardSet possibleOpponentHand = allCards;

  // // 仮のコンテナ型として std::vector を使用
  // std::vector<Card> possibleOpponentHand;

  // // 可能な限り、相手が出せるカードを予測する
  // for(const Card &card : possibleOpponentHand) {
  //   if(card.isGreaterThan(pile.at(0))) {
  //     return true; // 相手が出せるカードがあると予測される場合は承認する
  //   }
  // }

  // // 出せるカードがない場合はパスする
  // return false;
  
  return true;
}