#pragma once

namespace ctrl {
    void stop();
    void play();
    void pause();
    void next();
    void set_vol(float vol);
    void ch_vol(float delta);
    void set_pos(float vol);
    void ch_pos(float delta);
}
