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

    enum struct page_ix_status : uint8_t
    {
        INACTIVE, ACTIVE, DELETED
    };

    // enum struct page_ix_construct_option : uint8_t
    // {
    //     INIT, LOAD
    // };

    template <typename UINT>
    struct page_ix_data
    {
        uint64_t        heap_pos;
        UINT            active_pos;
        page_ix_status  status;

        page_ix_data()
            : page_ix_status(INACTIVE)
        {
        }
    };

    template <typename UINT, UINT N>
    class page_ix
    {
    private:
        
        /* 
         * Contains Index refernce to {Heap position, Active position, Status}
         * -- 
         * Index    Page IX Data {Heap position, Active position, Status}
         * 0        ..., ..., ...
         * 1        ..., ..., ...
         * .
         * .
         * .
         * N
         * 
         */

        page_ix_data<UINT>  page_ix_data_[N];

        /*
         * The size of active record indices
         *
         */
        
        uint64_t            active_size_;
        
        /*
         * An array of active record indices.
         *
         */
        
        UINT                active_[N];

        /*
         * The next ID that will be used in the active index
         *
         */

        UINT                next_id_;

    public:

        // Auto create page option
        page_ix(std::fstream &file, const uint64_t page_no)
            : active_size_(0), next_id_(0)  
        {
            commit_full_page(file, p);
        }

        // Auto load page_ix option
        page_ix(std::fstream &file, const uint64_t page_no)
            : active_size_(0), next_id_(0)  
        {
            commit_full_page(file, p);
        }


        page(const page &) = delete;
        page(page &&) = delete;
        auto operator =(const page &) -> page & = delete;
        auto operator =(page &&) -> page & = delete;


    public:

        auto get_page_ix_data(const UINT i) const -> page_ix_data<UINT>
        {
            return page_ix_data_[i];
        }

        auto get_active_size() const -> decltype(active_size_)
        {
            return active_size_;
        }

        auto get_inactive_size() const -> UINT
        {
            return N - next_id_;
        }

        /* 
         * It is assumed that by calling this function.
         *
         * or simply call get_inactive_size() and 
         * if it equals = 0 then do not call generate_next_id()
         *
         * 
         */ 

        auto generate_next_id(std::fstream &file, const uint64_t page_no, const uint64_t heap_pos) -> uint64_t
        {
            auto gen_id                         = next_id_++;
            auto ix_active_size                 = active_size_++;
            active_[ix_active_size]             = gen_id;
            page_ix_data_[gen_id].heap_pos      = heap_pos;
            page_ix_data_[gen_id].active_pos    = ix_active_size;
            page_ix_data_[gen_id].status        = page_ix_status::ACTIVE;

            // commit_page_ix(file, p, id);
            // commit_active_size(file, p);
            // commit_active(file, p, id_as);
            // commit_next_id(file, p);
            
            return page_no * N + id;
        }

        

        auto load(std::fstream &file, const uint64_t page, const UINT i)
        {
            uint64_t fi = p * sizeof(T) + i;
            *this = read_block_data<page>(file, fi);
        }

        auto debug()
        {
        }

        // it is assumed that i exists
        auto delete(std::fstream &file, const uint64_t page, const UINT i)
        {
            page_ix_[i].status = page_ix_status::deleted;

            // auto file = get_file_handler_for_write_block_data(filename);
            commit_page_ix(file, p, id);

            if (active_size_ > 0)
            {
                last_ix = active_size_ - 1;
                page_ix[last_ix].active_pos = i;
                active_[i] = last_ix;
                --active_size;

                commit_active_size(file, p);
                commit_active(file, p, i);                
            }
        }

    private:

        auto commit_full_page(std::fstream &file, const uint64_t page)
        {
            uint64_t get_pos_page_start = sizeof(page<UINT>) * page;
            write_block_data<page>(file, *this, get_pos_page_start);
        }            

        auto commit_page_ix(std::fstream &file, const uint64_t page, const UINT i)
        {
            write_block_data<page_ix>(file, &page_ix_[id], get_pos_page_ix(p, id));
        }

        auto get_pos_page_ix(const uint64_t page, const UINT i) -> uint64_t
        {
            return sizeof(page<UINT>) * p + id * sizeof(page_ix);
        }

        auto get_pos_active(const uint64_t p, const UINT i) -> uint64_t
        {
            return sizeof(page<UINT>) * p + sizeof(page_ix_) + id * sizeof(UINT);
        }

        auto get_pos_active_size(const uint64_t p) -> uint64_t
        {
            return sizeof(page<UINT>) * p + sizeof(page_ix_) + id * sizeof(UINT);
        }

        auto get_pos_next_size(const uint64_t p) -> uint64_t
        {
            return sizeof(page<UINT>) * p + sizeof(page_ix_) + i * sizeof(UINT);
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