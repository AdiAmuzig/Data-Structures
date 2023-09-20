//
// Created by adi.amuzig on 08/05/2020.
//

#ifndef HW1_WET_SOL_DIESEL_MUSIC_H
#define HW1_WET_SOL_DIESEL_MUSIC_H

#include "Doubly_Linked_List.h"
#include "AVL_Tree.h"


class Diesel_Music {
private:

    typedef AVLTree<int,int> playbacksSongs;
    typedef AVLTree<playbacksSongs,int> playbacksArtists;
    struct playbacks_node{
        int num_of_playbacks;
        AVLTree<playbacksSongs,int> playbacksArtists;
    };

    typedef DoublyLinkedList<playbacks_node>::Iterator i_playbacks;
    struct artistsSongs {//TODO: remember to delete "song"
        int num_of_songs;
        i_playbacks* song;
    };

    AVLTree<artistsSongs,int> musicDict;
    DoublyLinkedList<playbacks_node> playbacks;
    int total_num_of_songs;

public:
//  Main functions
    Diesel_Music();
    ~Diesel_Music();
    void AddArtist(int artistID, int numOfSongs);
    void RemoveArtist(int artistID);
    void AddToSongCount(int artistID, int songID);
    void NumberOfStreams(int artistID, int songID, int *streams);
    void GetRecommendedSongs(int numOfSongs, int *artists, int *songs);

//    Exceptions
    class SUCCESS: public std::exception {};
    class FAILURE: public std::exception {};
    class ALLOCATION_ERROR: public std::exception {};
    class INVALID_INPUT: public std::exception {};

};


#endif //HW1_WET_SOL_DIESEL_MUSIC_H
