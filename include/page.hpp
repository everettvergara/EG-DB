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
        active, inactive, deleted, locked
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
    template <typename UINT, UINT S>
    class page
    {
        page_data<UINT> page_data_[S];
        UINT            active_[S];
        uint64_t        active_size_;
        UINT            next_id_;

        page()
        {
        }

        auto delete(const uint16_t i)
        {
            if (status[i] not_eq page_data_status::active) return;

            status[i] = page_data_status::deleted;

        }

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

        auto commit_next_id(const std::string &filename, const uint64_t p, const UINT id)
        {
            auto offset = sizeof(page<UINT>) * p;
            write_block_data<page_data>(filename, &page_data_[id], offset + id * sizeof(page_data));
        }

        auto commit_active(const std::string &filename, const uint64_t p, const UINT id)
        {
            auto offset = sizeof(page<UINT>) * p + sizeof(page_data_);
            write_block_data<UINT>(filename, &active_[id], offset + id * sizeof(UINT));
        }

        auto commit_active_size(const std::string &filename, const uint64_t p)
        {
            auto offset = sizeof(page<UINT>) * p + sizeof(page_data_) + sizeof(active_);
            write_block_data<uint64_t>(filename, &active_size_, offset + sizeof(uint64_t));
        }

        auto commit_next_id(const std::string &filename, const uint64_t p)
        {
            auto offset = sizeof(page<UINT>) * p + sizeof(page_data_) + sizeof(active_) + sizeof(active_size_);
            write_block_data<UINT>(filename, &next_id_, offset + sizeof(UINT));
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

            commit_next_id(filename, p, id);
            commit_active(fiename, p, id_as);
            commit_active_size(filename, p);
            commit_next_id(filename, p);

            return p * S + id;
        }

        auto init(const std::string &filename, const uint64_t i)
        {
            // Set status
            for (uint64_t i = 0; i < S; ++i)
                page_data_[i].status = page_data_status::inactive;

            // Set next_id;
            active_size_ = 0;
            next_id_ = 0;

            write_block_data<page>(filename, *this, i);
        }

        auto load(const std::string &filename, const uint64_t i)
        {
            *this = read_block_data<page>(filename, i);
        }

        auto debug()
        {
            // std::cout << "Status:";
            // for (uint32_t i = 0; i < S; ++i)
            // {
            //     std::cout << " " << i << ": " << static_cast<int>(status[i]);
            // } 
            // std::cout << std::endl;

            // std::cout << "Active:";
            // for (uint32_t i = 0; i < active_size; ++i)
            // {
            //     std::cout << " " << i << ": " << active[i];
            // } 
            // std::cout << std::endl;

            // std::cout << "Inactive:";
            // for (uint32_t i = 0; i < inactive_size; ++i)
            // {
            //     std::cout << " " << i << ": " << inactive[i];
            // } 
            // std::cout << std::endl;
        }
    };

}