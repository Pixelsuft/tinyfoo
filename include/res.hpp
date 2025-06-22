#pragma once

namespace res {
    void* get_asset_io(const char* fn);
    void* read_asset_data(const char* fn, int& size_buf);
    void free_asset_data(void* data);
}
