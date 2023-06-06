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
    private:
        page_data<UINT> page_data_[S];
        uint64_t        active_size_;
        UINT            active_[S];
        UINT            next_id_;

    public:

        auto get_status(const uint64_t i) const -> page_data
        {
            return page_data_[get_ix(i)];
        }

        auto get_active_size() -> uint64_t
        {
            return active_size_;
        }

        auto get_inactive_size() -> UINT
        {
            return S - next_id_;
        }

        // It is assumed that by calling this function
        // the programmer is aware that there are still
        // available ID slots.
        auto generate_next_id(const std::fstream &file, const uint64_t page, const uint64_t heap_pos) const -> uint64_t
        {
            auto gen_id                     = next_id_++;
            auto ix_active_size             = active_size_++;
            active[ix_active_size]          = gen_id;
            page_data_[gen_id].heap_pos     = heap_pos;
            page_data_[gen_id].active_pos   = ix_active_size;
            page_data_[gen_id].status       = page_data_status::active;

            commit_page_data(file, p, id);
            commit_active_size(file, p);
            commit_active(file, p, id_as);
            commit_next_id(file, p);
            
            return p * S + id;
        }

        // Init must be called if page is initialized first time 
        auto init(const std::string &filename, const uint64_t page)
        {
            for (uint64_t i = 0; i < S; ++i)
                page_data_[i].status = page_data_status::inactive;

            active_size_    = 0;
            next_id_        = 0;

            commit_full_page(filename, p);
        }

        auto load(const std::string &filename, const uint64_t page, const UINT i)
        {
            uint64_t fi = p * sizeof(T) + i;
            *this = read_block_data<page>(filename, fi);
        }

        auto debug()
        {
        }

        // it is assumed that i exists
        auto delete(const std::string &filename, const uint64_t page, const UINT i)
        {
            page_data_[i].status = page_data_status::deleted;

            auto file = get_file_handler_for_write_block_data(filename);
            commit_page_data(file, p, id);

            if (active_size_ > 0)
            {
                last_ix = active_size_ - 1;
                page_data[last_ix].active_pos = i;
                active_[i] = last_ix;
                --active_size;

                commit_active_size(file, p);
                commit_active(file, p, i);                
            }
        }

    private:

        auto commit_full_page(const std::string &filename, const uint64_t page)
        {
            uint64_t get_pos_page_start = sizeof(page<UINT>) * page;
            write_block_data<page>(filename, *this, get_pos_page_start);
        }            

        auto commit_page_data(std::fstream &file, const uint64_t page, const UINT i)
        {
            write_block_data<page_data>(file, &page_data_[id], get_pos_page_data(p, id));
        }

        auto get_pos_page_data(const uint64_t page, const UINT i) -> uint64_t
        {
            return sizeof(page<UINT>) * p + id * sizeof(page_data);
        }

        auto get_pos_active(const uint64_t p, const UINT i) -> uint64_t
        {
            return sizeof(page<UINT>) * p + sizeof(page_data_) + id * sizeof(UINT);
        }

        auto get_pos_active_size(const uint64_t p) -> uint64_t
        {
            return sizeof(page<UINT>) * p + sizeof(page_data_) + id * sizeof(UINT);
        }

        auto get_pos_next_size(const uint64_t p) -> uint64_t
        {
            return sizeof(page<UINT>) * p + sizeof(page_data_) + i * sizeof(UINT);
        }



        auto commit_active(std::fstream &file, const uint64_t page, const UINT i)
        {
            write_block_data<UINT>(file, &active_[id], get_pos_active(p, id));
        }

        auto commit_active_size(std::fstream &file, const uint64_t page)
        {
            write_block_data<uint64_t>(file, &active_size_, get_pos_active_size());
        }

        auto commit_next_id(std::fstream &file, const uint64_t page)
        {
            write_block_data<UINT>(file, &next_id_, get_pos_next_id());
        }

    };

}