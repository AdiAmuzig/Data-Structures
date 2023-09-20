//
// Created by adi.amuzig on 08/05/2020.
//

#include "Diesel_Music.h"


Diesel_Music::Diesel_Music() {
    total_num_of_songs = 0;
    playbacks_node zero_playbacks;
    zero_playbacks.num_of_playbacks = 0;
    playbacks.insert(playbacks.returnHead(), zero_playbacks);
}

void Diesel_Music::AddArtist(int artistID, int numOfSongs) {
//  check if input is valid
    if (artistID <= 0 || numOfSongs <= 0 ){
        throw INVALID_INPUT();
    }

// add new artist to music dict
    try {
        musicDict.find(artistID);
        throw FAILURE();
    } catch (AVLTree<artistsSongs,int>::noMatch& e) {
        i_playbacks* songs = new i_playbacks[numOfSongs];
        int playbacksSongsKeys[numOfSongs];
        int playbacksSongsData[numOfSongs];
        for (int i = 0; i < numOfSongs; ++i) {
            songs[i] = playbacks.returnHead();
            playbacksSongsData[i] = artistID;
            playbacksSongsKeys[i] = i;
        }
        artistsSongs new_artist;
        new_artist.num_of_songs = numOfSongs;
        new_artist.song = songs;
        musicDict.insert(new_artist, artistID);

        total_num_of_songs = total_num_of_songs + numOfSongs;

//    add new artist to playbacks
        playbacksSongs newPlaybacksSongs(playbacksSongsData, playbacksSongsKeys, numOfSongs);
        i_playbacks zero_playbacks = playbacks.returnHead();
        (*zero_playbacks).playbacksArtists.insert(newPlaybacksSongs,artistID);
    }

}

void Diesel_Music::RemoveArtist(int artistID) {
//    validate input
    if (artistID <= 0){
        throw INVALID_INPUT();
    }

    artistsSongs artist_to_remove = musicDict.find(artistID);
    int num_of_songs = artist_to_remove.num_of_songs;
    for (int i=0; i<num_of_songs; ++i){
        i_playbacks song = artist_to_remove.song[i];
        if (song.current != nullptr){

            playbacksSongs playbacksSongs = song.current->data.playbacksArtists.find(artistID);
            int songs_with_i_playbacks = playbacksSongs.getNumOfNodes();
            int song_keys [songs_with_i_playbacks];
            playbacksSongs.ExtractKeys(song_keys);
            song.current->data.playbacksArtists.erase(artistID);

            if (song.current->data.num_of_playbacks != 0 &&
                song.current->data.playbacksArtists.getNumOfNodes() == 0){
                    playbacks.erase(song);
            }

            for (int j=0; j<songs_with_i_playbacks ; ++j){
                int key_to_delete = song_keys[j];
                artist_to_remove.song[key_to_delete].current = nullptr;
            }
        }
    }
    total_num_of_songs = total_num_of_songs - num_of_songs;
    delete[] artist_to_remove.song;
    artist_to_remove.song = nullptr;
    musicDict.erase(artistID);
}

void Diesel_Music::AddToSongCount(int artistID, int songID) {
    if (songID < 0 || artistID <= 0){
        throw INVALID_INPUT();
    }
    artistsSongs& artist = musicDict.find(artistID);// TODO: cathch later if artist is not found
    if (songID >= artist.num_of_songs){
        throw INVALID_INPUT();
    }
//  erasing non essentials
    i_playbacks song_to_increase = artist.song[songID];
    int num_of_playbacks = (*song_to_increase).num_of_playbacks;
    playbacksArtists& playbacksArtist = (*song_to_increase).playbacksArtists;
    playbacksSongs& playbacksSongs_current = playbacksArtist.find(artistID);
    playbacksSongs_current.erase(songID);

    i_playbacks next_playback_node = song_to_increase;
    if (playbacksSongs_current.getNumOfNodes() == 0){
        playbacksArtist.erase(artistID);
    }

//   adding to higher song list
    if (song_to_increase == playbacks.returnEnd() ||
            song_to_increase.current->next->data.num_of_playbacks != num_of_playbacks+1){
        playbacks_node new_playbacks_node;
        new_playbacks_node.num_of_playbacks = num_of_playbacks +1;
        new_playbacks_node.playbacksArtists.clearTree();
        playbacks.insert(song_to_increase, new_playbacks_node);
    }

    ++ next_playback_node;
    if (num_of_playbacks != 0 && playbacksArtist.getNumOfNodes() == 0){
        playbacks.erase(song_to_increase);
    }

//    add finding the artist and adding to him the new song
    try {
        playbacksSongs& playbacksSongs_next = (*next_playback_node).playbacksArtists.find(artistID);

        playbacksSongs_next.insert(artistID, songID);
    } catch (const playbacksArtists::noMatch& e) {
        playbacksSongs playbacksSongs_next;
        playbacksSongs_next.insert(artistID, songID);
        (*next_playback_node).playbacksArtists.insert(playbacksSongs_next,artistID);
    }
    artist.song[songID] = next_playback_node;
}

void Diesel_Music::NumberOfStreams(int artistID, int songID, int *streams) {
    if (songID < 0 || artistID <= 0 ){
        throw INVALID_INPUT();
    }
    artistsSongs artist = musicDict.find(artistID);
    if (songID >= artist.num_of_songs){
        throw INVALID_INPUT();
    }
    i_playbacks song_to_increase = artist.song[songID];
    *streams = (*song_to_increase).num_of_playbacks;
}

void Diesel_Music::GetRecommendedSongs(int numOfSongs, int *artists, int *songs) {
    if (numOfSongs<=0){
        throw INVALID_INPUT();
    }
    if (numOfSongs > total_num_of_songs){
        throw FAILURE();
    }

    i_playbacks playbacks_node_current = playbacks.returnEnd();
    int counter = 0;
    while (numOfSongs != 0){
        if (numOfSongs == 0){
            return;
        }
        playbacksArtists main_tree = (*playbacks_node_current).playbacksArtists;
        AVLTree<playbacksSongs,int>::iterator i = main_tree.returnListHead();
        while (i.current != nullptr && numOfSongs != 0) {
            counter = (*i).listExtractKeysAndData(songs, artists, &numOfSongs, counter);
            ++i;
        }
        --playbacks_node_current;
    }
}

Diesel_Music::~Diesel_Music() {
    AVLTree<artistsSongs,int>::iterator i = musicDict.returnListHead();
    while (!(i.isNull())){
        delete [] (*i).song;
        ++i;
    }
}

