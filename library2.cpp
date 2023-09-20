//
// Created by adi.amuzig on 15/06/2020.
//

#include "library2.h"
#include "Sound_Cloud_Music.h"
#include <new>

void *Init(){
    try {
        return new Sound_Cloud_Music();
    } catch (...) {
        return nullptr;
    }
}

StatusType AddArtist(void *DS, int artistID){
    if (DS == nullptr){
        return INVALID_INPUT;
    }
    try {
        ((Sound_Cloud_Music *) DS) ->AddArtist(artistID);
    } catch (Sound_Cloud_Music::INVALID_INPUT& e) {
        return INVALID_INPUT;
    } catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    } catch (std::exception& e) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType RemoveArtist(void *DS, int artistID){
    if (DS == nullptr){
        return INVALID_INPUT;
    }
    try {
        ((Sound_Cloud_Music *) DS) ->RemoveArtist(artistID);
    } catch (Sound_Cloud_Music::INVALID_INPUT& e) {
        return INVALID_INPUT;
    } catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    } catch (std::exception& e) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType AddSong(void *DS, int artistID, int songID){
    if (DS == nullptr){
        return INVALID_INPUT;
    }
    try {
        ((Sound_Cloud_Music *) DS) ->AddSong(artistID,songID);
    } catch (Sound_Cloud_Music::INVALID_INPUT& e) {
        return INVALID_INPUT;
    } catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    } catch (std::exception& e) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType RemoveSong(void *DS, int artistID, int songID){
    if (DS == nullptr){
        return INVALID_INPUT;
    }
    try {
        ((Sound_Cloud_Music *) DS) ->RemoveSong(artistID,songID);
    } catch (Sound_Cloud_Music::INVALID_INPUT& e) {
        return INVALID_INPUT;
    } catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    } catch (std::exception& e) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType AddToSongCount(void *DS, int artistID, int songID, int count){
    if (DS == nullptr){
        return INVALID_INPUT;
    }
    try {
        ((Sound_Cloud_Music *) DS) ->AddToSongCount(artistID,songID,count);
    } catch (Sound_Cloud_Music::INVALID_INPUT& e) {
        return INVALID_INPUT;
    } catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    } catch (std::exception& e) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType GetArtistBestSong(void *DS, int artistID, int *songId){
    if (DS == nullptr || songId == nullptr){
        return INVALID_INPUT;
    }
    try {
        ((Sound_Cloud_Music *) DS) ->GetArtistBestSong(artistID,songId);
    } catch (Sound_Cloud_Music::INVALID_INPUT& e) {
        return INVALID_INPUT;
    } catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    } catch (std::exception& e) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType GetRecommendedSongInPlace(void *DS, int rank, int *artistId, int *songId){
    if (DS == nullptr || songId == nullptr || artistId == nullptr){
        return INVALID_INPUT;
    }
    try {
        ((Sound_Cloud_Music *) DS) ->GetRecommendedSongInPlace(rank,artistId,songId);
    } catch (Sound_Cloud_Music::INVALID_INPUT& e) {
        return INVALID_INPUT;
    } catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    } catch (std::exception& e) {
        return FAILURE;
    }
    return SUCCESS;
}

void Quit(void** DS){
    delete *((Sound_Cloud_Music**)DS);
    *((Sound_Cloud_Music**)DS) = nullptr;
}

