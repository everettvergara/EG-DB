#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <memory>
#include <optional>
#include <type_traits>

#include "common.hpp"
#include "block_data.hpp"

namespace eg
{

    enum struct page_ix_status : uint8_t
    {
        INACTIVE, ACTIVE, DELETED
    };

    enum struct page_ix_construct_option : uint8_t
    {
        INIT, LOAD
    };

    // TODO: CHANGE ACTIVE_SIZE TO UINT, since N = 0 to 254
    struct test
    {
        int x;
        test() 
            : x(10)
        {
            std::cout << "test constructor here..." << std::endl;
        }

    };

    template <typename UINT, UINT N>
    struct page_ix_data
    {
        uint64_t        heap_pos[N];
        UINT            active_pos[N];
        page_ix_status  status[N];
        uint64_t        active_size;
        UINT            active[N];
        UINT            next_id;
    };

    template <typename T>
    struct no_init : std::allocator<T>
    {
        static_assert(std::is_trivially_copyable_v<T>, "no_init is allowable only on trivially copyable structs.");
    };

    template <typename UINT, UINT N>
    class page_ix
    {
    private:

        no_init<page_ix_data<UINT, N>>  data_;
        page_ix_data<UINT, N>           *data_ptr_;
        
    
    private:

        auto commit_page(std::fstream &file, const uint64_t page_no) -> void
        {
            write_block_data<page_ix_data<UINT, N>>(file, page_no, data_ptr_);
        }

        auto load_page(std::fstream &file, const uint64_t page_no) -> void
        {
            read_block_data<page_ix_data<UINT, N>>(file, page_no, data_ptr_);
        }

    public:
        page_ix(std::fstream &file, const uint64_t page_no, const page_ix_construct_option option)
            : data_(), data_ptr_(data_.allocate(1))
        {
            if (option == page_ix_construct_option::INIT) 
                commit_page(file, page_no);
            else 
                load_page(file, page_no);   
        }

        page_ix(const page_ix &) = delete;
        page_ix(page_ix &&) = delete;
        auto operator =(const ppage_ixage &) -> page_ix & = delete;
        auto operator =(page_ix &&) -> page_ix & = delete;


    public:

        auto generate_next_id(std::fstream &file, const uint64_t page_no, const uint64_t heap_pos) -> UINT
        {
            auto gen_id                         = data_ptr_->next_id_++;
            auto ix_active_size                 = data_ptr_->active_size_++;

            data_ptr_->active[ix_active_size]   = gen_id;
            data_ptr_->heap_pos[gen_id]         = heap_pos;
            data_ptr_->active_pos[gen_id]       = ix_active_size;
            data_ptr_->status[gen_id]           = page_ix_status::ACTIVE;

            commit_page(file, page_no);
            
            return gen_id;
        }

        auto delete_id_wo_commit(std::fstream &file, const uint64_t page_no, const UINT id)
        {
            if (data_ptr_->status[id] not_eq page_ix_status::ACTIVE) return;
            
            data_ptr_->status[id] = page_ix_status::deleted;

            if (data_ptr_->active_size > 0)
            {
                auto lix = data_ptr_->active_size - 1;
                data_ptr_->active[i] = data_ptr_->active[lix]; 
                data_ptr_->active_pos[lix] = i;
                --data_ptr_->active_size;             
            }       
        }
    };



/*
    template <typename UINT, UINT N>
    class page_ix
    {
    private:

        std::vector<page_ix_data<UINT, N>, no_init<page_ix_data<UINT, N>>> data_;

    public:

        // Auto create page option
        page_ix(std::fstream &file, const uint64_t page_no, const page_ix_construct_option option)
            :   data_(N)
        {
            if (option == page_ix_construct_option::INIT)
            {
                for (UINT i = 0; i < N; ++i)
                    data_[i].status = page_ix_status::INACTIVE;

                commit_full_page(file, page_no);

            } 
            else 
            {
                load_full_page(file, page_no);
            }
        }


        page(const page &) = delete;
        page(page &&) = delete;
        auto operator =(const page &) -> page & = delete;
        auto operator =(page &&) -> page & = delete;

    public:

        auto get_file_page_pos(const uint64_t page_no) const -> uint64_t
        {
            size_t s =  N * sizeof(uint64_t) +          // Heap pos 
                        N * sizeof(UINT) +              // Active pos 
                        N * sizeof(page_ix_status) +    // Page IX status
                        sizeof(uint64_t)                // active_size_
                        N * sizeof(UINT) +              // active
                        sizeof(UINT);                   // Next ID 

            uint64_t fp = page_no * s + i;

            return fp;
        }

        auto load_full_page(std::fstream &file, const uint64_t page_no)
        {
            auto start = get_file_page_pos(page_no);
            read_block_data<page>(file, fi, data.data());
        }

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


    */

}