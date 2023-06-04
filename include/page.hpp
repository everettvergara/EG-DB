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


    struct page_data
    {
        uint64_t            heap_pos;
        uint16_t            active_pos;
        page_data_status    status;
    };

    // Recommended starting size: 256 records
    template <uint16_t S>
    class page
    {
        page_data           page_data_[S];
        uint16_t            active_[S];
        uint16_t            next_id_;

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

        auto get_next_id(const uint64_t p, const uint64_t h) const -> std::optional<uint64_t>
        {
            // If there are no more IDs left on this page
            if (inactive_size == 0) return {};
            
            // Get the next available ID
            uint16_t id = S - inactive_size;
            --inactive_size;

            // Init vars
            status[id]          = page_data_status::active;
            heap_pos[id]        = h;
            active[active_size] = id;
            active_pos[id]      = active_size;
            ++active_size;

            return p * S + id;
        }

        auto init()
        {
            // Set status
            for (uint32_t i = 0; i < S; ++i)
                page_data_[i].status = page_data_status::inactive;

            // Set actives
            active_size = 0;

            // Set inactives;
            next_id_ = 0;
        }

        // auto save(const std::string &filename, const uint_t i)
        // {
        //     // write_block_data<page>(filename, *this, i);
        // }

        // auto load(const std::string &filename, const uint_t i)
        // {
        //     // *this = read_block_data<page>(filename, i);
        // }

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