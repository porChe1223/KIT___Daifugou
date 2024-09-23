//
//  MyGroupPlayer.cpp
//  PlayingCard
//

#include <iostream>
#include <string>

#include "card.h"
#include "cardset.h"
#include "player.h"
#include "gamestatus.h"
#include "MyGroupPlayer.h"

void MyGroupPlayer::ready() {
    int i;
    
    memory.clear();    // memoryは自分の手札と既出のカード以外（相手の持っている可能性のあるカードセット）
    // trump.clear();
    hand.sort(myCardCmp);
    memory.setupDeck();
    for(i=0; i<hand.size(); i++) {
      memory.remove(hand.at(i));
    }
}

// 「(弱い方から順に) 出せるなら温存せずに出す！！」という貪欲戦略
bool MyGroupPlayer::follow(const GameStatus &gstat, CardSet &cs) {
    CardSet pile(gstat.pile);
    Card tmp1;
    Card tmp2;
    Card tmp3;
    Card tmp4;
    int leadSize;
    int jkrumu;

    hand.sort(myCardCmp);  // 手札をソート(弱い方から順に)

    leadSize = pile.size();
    
    // jkrがハンドがあるにあるかどうかを確認
    if(hand.at(hand.size() - 1).rank() == 15) {
      jkrumu = 1;
    } else {
      jkrumu = 0;
    }

    if (leadSize == 0) { // 自分がリーダなら
      // 同じ番号のカードが4枚ある場合、全部出す
      for (int i = 0; i < hand.size() - 3; i++) {
        tmp1 = hand.at(i);
        tmp2 = hand.at(i + 1);
        tmp3 = hand.at(i + 2);
        tmp4 = hand.at(i + 3);
        if(tmp1.rank() == tmp4.rank()){
          if((tmp1.rank() >= 2 && tmp1.rank() <= 11) || hand.size() <= 5) {
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
      for (int i = 0; i < hand.size() - 2; i++) {
        tmp1 = hand.at(i);
        tmp2 = hand.at(i + 1);
        tmp3 = hand.at(i + 2);
        if(tmp1.rank() == tmp3.rank()){
          if((tmp1.rank() >= 2 && tmp1.rank() <= 11 && strongest(memory, tmp1, 3)) || hand.size() <= 4) {
            cs.insert(tmp1);// 同じカードを3枚追加
            cs.insert(tmp2);
            cs.insert(tmp3);
            hand.remove(tmp1);
            hand.remove(tmp2);
            hand.remove(tmp3);
            return true;
          } else if(jkrumu == 1 && strongest(memory, tmp1, 4)) {
            tmp4 = hand.at(hand.size() - 1);
            
            cs.insert(tmp1);
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
      // 同じ番号のカードが2枚ある場合、両方出す
      for (int i = 0; i < hand.size() - 1; i++) {
        tmp1 = hand.at(i);
        tmp2 = hand.at(i + 1);
        if(tmp1.rank() == tmp2.rank()){
          // 今選択している２枚が最強なら出す
          if(strongest(memory, tmp1, 2)) {
            if((tmp1.rank() >= 2 && tmp1.rank() <= 11) || hand.size() <= 3) {
              cs.insert(tmp1); // 同じカードを2枚追加
              cs.insert(tmp2);
              hand.remove(tmp1);
              hand.remove(tmp2);
              return true;
            }
          }
          // jkrを含めた３枚が最強なら出す
          else if(jkrumu == 1 && strongest(memory, tmp1, 3)) {
            tmp3 = hand.at(hand.size() - 1);
            
            cs.insert(tmp1);
            cs.insert(tmp2);
            cs.insert(tmp3);
            hand.remove(tmp1);
            hand.remove(tmp2);
            hand.remove(tmp3);
            
            return true;
          }
          // ２枚組は温存せずに以下の条件に沿って基本出す
          else if((tmp1.rank() >= 2 && tmp1.rank() <= 11) || hand.size() <= 3) {
            cs.insert(tmp1); // 同じカードを2枚追加
            cs.insert(tmp2);
            hand.remove(tmp1);
            hand.remove(tmp2);
            return true;
          }
        }
      }
      
      tmp1 = hand.at(0); // 一番弱いカードを1枚出す
      cs.insert(tmp1);
      hand.remove(tmp1);
      return true;
    }
    else if (leadSize == 1) { // リードが1枚の場合      
      if(hand.size() >= 7) { // 手札が７枚を切るまでは最強の一枚（jkrを持っていたらjkr）を温存する
        for (int i = 0; i < hand.size() - 1; i++) {
          tmp1 = hand.at(i);
          if (tmp1.isGreaterThan(pile.at(0))) {
            cs.insert(tmp1);           
            hand.remove(tmp1);
            return true;
          }
        }
      }
      else {            
        for (int i = 0; i < hand.size(); i++) {
          tmp1 = hand.at(i);
          if (tmp1.isGreaterThan(pile.at(0))) {
            cs.insert(tmp1);           
            hand.remove(tmp1);
            return true;
          }
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

bool MyGroupPlayer::approve(const GameStatus &gstat) {
  int i;
 
  if (gstat.playerName[gstat.turnIndex] != "TestPlayer") {
    for(i=0; i<=gstat.pile.size(); i++) {
      memory.remove(gstat.pile.at(i));
    }
  }
  return true;
}

// カードセットcsetの中に、数字がrankのカードは何枚あるか
int MyGroupPlayer::count(const CardSet cset, int rank) {
  int i, n=0;

  for(i=0; i<cset.size(); i++) {
    if(cset.at(i).rank() == rank) {
      n++;
    }
  }

  return n;
}

// カードセットcsetのどのカードよりも、カードcのnum枚の組み合わせが強ければtrue
bool MyGroupPlayer::strongest(const CardSet cset, Card c, int num) {
  int i;
  Card temp;
 
  for (i=1; i<=13; i++) {
    std::cout << temp;
    if (c.strength() < temp.strength()) {
      if(num <= count(cset,i)) {
    return false;
      } else if(num-1==count(cset,i) && count(cset,15)==1) {
    return false;
      }
    }
  }

  if(count(cset,15) && num==1)
    return false;

  return true;
}
