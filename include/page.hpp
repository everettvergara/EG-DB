#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <memory>
#include <optional>

#include "common.hpp"
#include "block_data.hpp"

namespace eg
{
    /*
    struct inactive_alloc : std::allocator<uint16_t>
    {
        auto allocate(size_t n) -> uint16_t *
        {
            auto *p = std::allocator<uint16_t>::allocate(n);

            for (size_t i = 0; i < 0xffff; ++i)
                p[i] = static_cast<uint16_t>(i);

            return p;
        }

        auto deallocate(uint16_t *p, size_t n)
        {
            std::allocator<uint16_t>::deallocate(p, n);
        }
    };
    */

    enum struct page_data_status : uint8_t
    {
        active, inactive, deleted
    };


    template <typename UINT>
    struct page_data
    {
        uint64_t            heap_pos;
        UINT                active_pos;
        page_data_status    status;
    };

    // Recommended starting size: 256 records

    // uint8_t, 2^8
    // S is recommended to be multiples of uint64_t

    template <typename UINT, UINT S>
    class page
    {
        page_data<UINT> page_data_[S];
        uint64_t        active_size_;
        UINT            active_[S];
        UINT            next_id_;


        auto get_head_pos(const uint16_t i) const -> std::optional<uint64_t>
        {
            if (status[i] not_eq page_data_status::active) return {};
            return pos[i];
        }


        auto get_status(const uint16_t i) const -> page_data_status
        {
            return status[i];
        }

        auto get_active_size() -> uint16_t
        {
            return S - inactive_size;
        }

        auto get_inactive_size() -> uint16_t
        {
            return inactive_size;
        }

        auto get_next_id(const std::string &filename, const uint64_t p, const uint64_t h) const -> std::optional<uint64_t>
        {
            // Get next ID
            auto id                     = next_id_++;
            auto id_as                  = active_size_++;
            active[id_as]               = id;
            page_data_[id].heap_pos     = h;
            page_data_[id].active_pos   = id_as;
            page_data_[id].status       = page_data_status::active;

            auto file = get_file_handler_for_write_block_data(filename);
            commit_next_id(file, p, id);
            commit_active(file, p, id_as);
            commit_active_size(file, p);
            commit_next_id(file, p);
            file.close();

            return p * S + id;
        }

        auto init(const std::string &filename, const uint64_t p)
        {
            // Set status
            for (uint64_t i = 0; i < S; ++i)
                page_data_[i].status = page_data_status::inactive;

            // Set next_id;
            active_size_ = 0;
            next_id_ = 0;

            commit_full_page(filename, p);
        }

        auto load(const std::string &filename, const uint64_t i)
        {
            *this = read_block_data<page>(filename, i);
        }

        auto debug()
        {

        }

        auto delete(const uint16_t i)
        {
            if (status[i] not_eq page_data_status::active) return;

            status[i] = page_data_status::deleted;

        }

    private:
    
        auto get_pos_page_data(const uint64_t p, const UINT id) -> uint64_t
        {
            return sizeof(page<UINT>) * p + id * sizeof(page_data);
        }

        auto get_pos_active(const uint64_t p, const UINT id) -> uint64_t
        {
            return sizeof(page<UINT>) * p + sizeof(page_data_) + id * sizeof(UINT);
        }

        auto get_pos_active_size(const uint64_t p) -> uint64_t
        {
            return sizeof(page<UINT>) * p + sizeof(page_data_) + id * sizeof(UINT);
        }

        auto get_pos_next_size(const uint64_t p) -> uint64_t
        {
            return sizeof(page<UINT>) * p + sizeof(page_data_) + id * sizeof(UINT);
        }

        auto commit_next_id(std::fstream &file, const uint64_t p, const UINT id)
        {
            write_block_data<page_data>(file, &page_data_[id], get_pos_page_data(p, id));
        }

        auto commit_active(std::fstream &file, const uint64_t p, const UINT id)
        {
            write_block_data<UINT>(file, &active_[id], get_pos_active(p, id));
        }

        auto commit_active_size(std::fstream &file, const uint64_t p)
        {
            write_block_data<uint64_t>(file, &active_size_, get_pos_active_size());
        }

        auto commit_next_id(std::fstream &file, const uint64_t p)
        {
            write_block_data<UINT>(file, &next_id_, get_pos_next_size());
        }

        auto commit_full_page(const std::string &filename, const uint64_t p)
        {
            write_block_data<page>(filename, *this, i);
        }    
    };

}