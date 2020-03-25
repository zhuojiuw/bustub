//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// clock_replacer.cpp
//
// Identification: src/buffer/clock_replacer.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/clock_replacer.h"

namespace bustub {

ClockReplacer::ClockReplacer(size_t num_pages) {
    ClockItem clock_item = {1, false};
    for (size_t i = 0; i != num_pages; i++) {
        clock_replacer.emplace_back(clock_item);
    }
    clock_hand = 0;
    clock_size = 0;
}

ClockReplacer::~ClockReplacer() = default;

bool ClockReplacer::Victim(frame_id_t *frame_id) {
    std::lock_guard<std::mutex> guard(clock_mutex);
    while (clock_size>0){
        if (clock_replacer[clock_hand].isPin) {
            clock_hand = (clock_hand + 1) % clock_replacer.clock_size;
        } else if (clock_replacer[clock_hand].ref) {
            clock_replacer[clock_hand].ref = false;
            clock_hand = (clock_hand + 1) % clock_replacer.clock_size;
        } else {
            clock_replacer[clock_hand].ref = true;
            clock_size--;
            *frame_id = clock_hand;
            clock_hand = (clock_hand + 1) % clock_replacer.clock_size;
            return true;
        }
    }
    return false;
}

void ClockReplacer::Pin(frame_id_t frame_id) {
    std::lock_guard<std::mutex> guard(clock_mutex);
    if(clock_replacer[frame_id].isPin == false) {
        clock_replacer[frame_id].isPin = true;
        clock_size--;
    }
}

void ClockReplacer::Unpin(frame_id_t frame_id) {
    std::lock_guard<std::mutex> guard(clock_mutex);
    if(clock_replacer[frame_id].isPin == true) {
        clock_replacer[frame_id].isPin = false;
        clock_size++;
    }
}

size_t ClockReplacer::Size() {
    std::lock_guard<std::mutex> guard(clock_mutex);
    return clock_size;
}

}  // namespace bustub
