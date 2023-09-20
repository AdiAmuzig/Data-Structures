//
// Created by adi.amuzig on 14/06/2020.
//

#ifndef CODE_SOUND_CLOUD_MUSIC_H
#define CODE_SOUND_CLOUD_MUSIC_H

#include <exception>
#include "AVL_Tree.h"
#include "TripleKey.h"
#include "DynamicChainHashing.h"

class Sound_Cloud_Music{
private:
    struct hashNode{
        AVLTree<int,TripleKey> songsByRank;
        AVLTree<int,int> songsByNum;
    };

    DynamicChainHashing<hashNode,int> Artists;
    AVLTree<TripleKey, TripleKey> mainSongRanking;


public:
    Sound_Cloud_Music() = default;
    ~Sound_Cloud_Music() = default;
    void AddArtist(int artistID);
    void RemoveArtist(int artistID);
    void AddSong(int artistID, int songID);
    void RemoveSong(int artistID, int songID);
    void AddToSongCount(int artistID, int songID, int count);
    void GetArtistBestSong(int artistID, int *songId);
    void GetRecommendedSongInPlace(int rank, int *artistId, int *songId);

    class SUCCESS: public std::exception {};
    class FAILURE: public std::exception {};
    class ALLOCATION_ERROR: public std::exception {};
    class INVALID_INPUT: public std::exception {};
};

#endif //CODE_SOUND_CLOUD_MUSIC_H
