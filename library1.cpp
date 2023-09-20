//
// Created by adi.amuzig on 13/05/2020.
//
#include "library1.h"
#include "Diesel_Music.h"
#include <new>

void *Init(){
    try {
        return new Diesel_Music();
    } catch (...) {
        return nullptr;
    }
}

StatusType AddArtist(void *DS, int artistID, int numOfSongs){
    if (DS == nullptr){
        return INVALID_INPUT;
    }
    try {
        ((Diesel_Music *) DS)->AddArtist(artistID, numOfSongs);
    } catch (Diesel_Music::INVALID_INPUT& e) {
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
        ((Diesel_Music*)DS)->RemoveArtist(artistID);
    } catch (Diesel_Music::INVALID_INPUT& e) {
        return INVALID_INPUT;
    } catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    } catch (std::exception& e) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType AddToSongCount(void *DS, int artistID, int songID){
    if (DS == nullptr){
        return INVALID_INPUT;
    }
    try {
        ((Diesel_Music*)DS)->AddToSongCount(artistID, songID);
    } catch (Diesel_Music::INVALID_INPUT& e) {
        return INVALID_INPUT;
    } catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    } catch (std::exception& e) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType NumberOfStreams(void *DS, int artistID, int songID, int *streams){
    if (DS == nullptr || streams == nullptr){
        return INVALID_INPUT;
    }
    try {
        ((Diesel_Music*)DS)->NumberOfStreams(artistID,songID,streams);
    } catch (Diesel_Music::INVALID_INPUT& e) {
        return INVALID_INPUT;
    } catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    } catch (std::exception& e) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType GetRecommendedSongs(void *DS, int numOfSongs, int *artists, int *songs){
    if (numOfSongs<=0){
        return ALLOCATION_ERROR;
    }
    if (DS == nullptr){
        return INVALID_INPUT;
    }
    try {
        ((Diesel_Music*)DS)->GetRecommendedSongs(numOfSongs,artists,songs);
    } catch (Diesel_Music::INVALID_INPUT& e) {
        return INVALID_INPUT;
    } catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    } catch (std::exception& e) {
        return FAILURE;
    }
    return SUCCESS;
}

void Quit(void** DS){
    delete *((Diesel_Music**)DS);
    *((Diesel_Music**)DS) = nullptr;
}




