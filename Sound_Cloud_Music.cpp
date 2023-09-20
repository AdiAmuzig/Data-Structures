//
// Created by adi.amuzig on 14/06/2020.
//

#include "Sound_Cloud_Music.h"


void Sound_Cloud_Music::AddArtist(int artistID) {
    if (artistID <=0){
        throw INVALID_INPUT();
    }
    try {
        Artists.find(artistID);
    } catch (std::exception& e) {
        hashNode new_hash_node = hashNode();
        Artists.insert(new_hash_node,artistID);
        return;
    }
    throw FAILURE();
}

void Sound_Cloud_Music::RemoveArtist(int artistID) {
    if (artistID <=0){
        throw INVALID_INPUT();
    }

    hashNode currentHashNode;
    try{
        currentHashNode = Artists.find(artistID);
    }catch (std::exception& e) {
        throw FAILURE();
    }
    if (currentHashNode.songsByNum.getNumOfNodes() > 0){
        throw FAILURE();
    }

    Artists.erase(artistID);
}

void Sound_Cloud_Music::AddSong(int artistID, int songID) {
    if (artistID <= 0 || songID <= 0){
        throw INVALID_INPUT();
    }

    try{
        Artists.find(artistID);
    }catch (std::exception& e) {
        throw FAILURE();
    }

    try{
        Artists.find(artistID).songsByNum.find(songID);
    }catch (std::exception& e) {
        ((Artists.find(artistID)).songsByNum).insert(0,songID);
        TripleKey tripleKey = TripleKey(0,artistID,songID);
        ((Artists.find(artistID)).songsByRank).insert(songID,tripleKey);
        mainSongRanking.insert(tripleKey,tripleKey);
        return;
    }

    throw FAILURE();
}

void Sound_Cloud_Music::RemoveSong(int artistID, int songID) {
    if (artistID <= 0 || songID <= 0){
        throw INVALID_INPUT();
    }

    try{
        hashNode &currentHashNode = Artists.find(artistID);
        int numOfPlays = currentHashNode.songsByNum.find(songID);
        TripleKey tripleKey = TripleKey(numOfPlays,artistID,songID);
        currentHashNode.songsByNum.erase(songID);
        currentHashNode.songsByRank.erase(tripleKey);
        mainSongRanking.erase(tripleKey);
    }catch (std::exception& e) {
        throw FAILURE();
    }

}

void Sound_Cloud_Music::AddToSongCount(int artistID, int songID, int count) {
    if (artistID <= 0 || songID <= 0 || count <= 0){
        throw INVALID_INPUT();
    }

    try{
        hashNode &currentHashNode = Artists.find(artistID);
        int numOfPlays = currentHashNode.songsByNum.find(songID);
        int newNumOfPlays = numOfPlays + count;
        TripleKey oldTripleKey = TripleKey(numOfPlays,artistID,songID);
        TripleKey newTripleKey = TripleKey(newNumOfPlays,artistID,songID);

        currentHashNode.songsByNum.erase(songID);
        currentHashNode.songsByRank.erase(oldTripleKey);
        mainSongRanking.erase(oldTripleKey);

        currentHashNode.songsByNum.insert(newNumOfPlays,songID);
        currentHashNode.songsByRank.insert(songID,newTripleKey);
        mainSongRanking.insert(newTripleKey,newTripleKey);
    }catch (std::exception& e) {
        throw FAILURE();
    }

}

void Sound_Cloud_Music::GetArtistBestSong(int artistID, int *songId) {
    if (artistID <= 0 || songId == nullptr){
        throw INVALID_INPUT();
    }

    try{
        hashNode &currentHashNode = Artists.find(artistID);
        if (currentHashNode.songsByNum.getNumOfNodes() == 0){
            throw FAILURE();
        }
        *songId = currentHashNode.songsByRank.returnMaxNodeData();
    }catch (std::exception& e) {
        throw FAILURE();
    }
}

void Sound_Cloud_Music::GetRecommendedSongInPlace(int rank, int *artistId, int *songId) {
    if (artistId == nullptr || songId == nullptr || rank <= 0){
        throw INVALID_INPUT();
    }

    int totalNumOfSongs = mainSongRanking.getNumOfNodes();
    if (rank > totalNumOfSongs){
        throw FAILURE();
    }

    AVLTree<TripleKey,TripleKey>::iterator i = mainSongRanking.valueByRank(rank);
    TripleKey tripleKey = *i;
    *artistId = tripleKey.getSecondKey();
    *songId = tripleKey.getThirdKey();
}























