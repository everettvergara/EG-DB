#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <memory>
#include <optional>
#include <type_traits>
#include <initializer_list>

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

    private:
        auto delete_id(const UINT id)
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
    
        auto update_id(const UINT id, const uint64_t heap_pos)
        {
            if (data_ptr_->status[id] not_eq page_ix_status::ACTIVE) return;
            data_ptr_->heap_pos[id] = heap_pos;
        }

    public:
        auto delete_id(std::fstream &file, const uint64_t page_no, std::initializer_list<UINT> ids) -> UINT 
        {
            for (auto id : ids) delete_id(id);
            commit_page(file, page_no);
        }
    };

}