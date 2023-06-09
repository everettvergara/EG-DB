#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <memory>
#include <optional>
#include <type_traits>
#include <initializer_list>
#include <mutex>

#include "common.hpp"
#include "file_helper.hpp"

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

    template <typename UINT, uint64_t N>
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

    template <typename UINT, uint64_t N>
    class page_ix
    {
    private:

        no_init<page_ix_data<UINT, N>>  data_;
        page_ix_data<UINT, N>           *data_ptr_;
        std::mutex                      action_mutex_;
        
    
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
        auto operator =(const page_ix &) -> page_ix & = delete;
        auto operator =(page_ix &&) -> page_ix & = delete;

    public:

        auto get_last_gen_id() -> std::optional<UINT>
        {
            if (data_ptr_->active_size == 0) return {};
            return data_ptr_->next_id - 1;
        }

    private:

        auto delete_id(const UINT id) -> UINT
        {

            if (data_ptr_->status[id] not_eq page_ix_status::ACTIVE) return 0;

            data_ptr_->status[id] = page_ix_status::DELETED;

            /*
                active_pos[id]
                0,  1   2   3   4   5   id
                --
                5,  4,  3,  2,  1,  0   ix in active[]
                            * ix: data_ptr_->active_pos_[id]    

                active[] Active Indices
                0,  1   2   3   4   5
                --
                5,  4,  3,  2,  1,  0
                        |           * lid: data_ptr_->active[data_ptr_->active_size];
                        

                ix: position of ID in active = data_ptr_->active_pos[id];
                    in the above sample: 

                lid: ID of the last active: data_ptr_->active[data_ptr_->active_size_];
                lix: position of lid in active = data_ptr_->active[data_ptr_->active_pos_[lid]]

            */

            auto ix = data_ptr_->active_pos[id];

            --data_ptr_->active_size;      
            auto lid = data_ptr_->active[data_ptr_->active_size];

            data_ptr_->active_pos[lid] = ix;
            data_ptr_->active[ix]      = lid;

            return 1;
        }
    
        auto update_id(const UINT id, const uint64_t heap_pos)
        {
            if (data_ptr_->status[id] not_eq page_ix_status::ACTIVE) return;
            data_ptr_->heap_pos[id] = heap_pos;
        }

    public:

        auto generate_next_id(std::fstream &file, const uint64_t page_no, const uint64_t heap_pos) -> UINT
        {
            std::lock_guard<std::mutex> lock(action_mutex_);

            auto gen_id                         = data_ptr_->next_id++;
            auto ix_active_size                 = data_ptr_->active_size++;

            data_ptr_->active[ix_active_size]   = gen_id;
            data_ptr_->heap_pos[gen_id]         = heap_pos;
            data_ptr_->active_pos[gen_id]       = ix_active_size;
            data_ptr_->status[gen_id]           = page_ix_status::ACTIVE;

            commit_page(file, page_no);
            
            return gen_id;
        }

        auto delete_all(std::fstream &file, const uint64_t page_no) -> UINT 
        {
            std::lock_guard<std::mutex> lock(action_mutex_);

            auto affected_rows = data_ptr_->active_size;

            for (UINT i = 0; i < data_ptr_->active_size; ++i)
                data_ptr_->status[data_ptr_->active[i]] = page_ix_status::DELETED;

            data_ptr_->active_size = 0;

            commit_page(file, page_no);

            return affected_rows;
        }

        auto delete_id(std::fstream &file, const uint64_t page_no, std::initializer_list<UINT> ids) -> UINT 
        {
            std::lock_guard<std::mutex> lock(action_mutex_);

            auto affected_rows = 0;

            for (auto id : ids) 
                affected_rows += delete_id(id);
            commit_page(file, page_no);

            return affected_rows;
        }

        auto update_id(std::fstream &file, const uint64_t page_no, std::initializer_list<std::tuple<UINT, uint64_t>> ids_heaps)
        {
            std::lock_guard<std::mutex> lock(action_mutex_);

            for (auto [id, heap_pos] : ids_heaps) update_id(id, heap_pos);
            commit_page(file, page_no);
        }

        auto retrieve_heap_pos_by_id(const UINT id) const -> std::vector<std::tuple<UINT, uint64_t>>
        {
            std::lock_guard<std::mutex> lock(action_mutex_);

             std::vector<std::tuple<UINT, uint64_t>> result;

            if (data_ptr_->status[id] not_eq page_ix_status::ACTIVE) return result;
            return {id, data_ptr_->heap_pos[id]};
        }
        
        auto retrieve_heap_pos_by_id(std::initializer_list<UINT> ids) const -> std::vector<std::tuple<UINT, uint64_t>>
        {
            std::lock_guard<std::mutex> lock(action_mutex_);

            std::vector<std::tuple<UINT, uint64_t>> result;
            
            for (auto id : ids)
            {
                if (data_ptr_->status[id] == page_ix_status::ACTIVE)
                    result.emplace_back(id, data_ptr_->heap_pos[id]);
            }

            return result;
        }

        auto retrieve_heap_pos_all_active() const -> std::vector<std::tuple<UINT, uint64_t>>
        {
            std::lock_guard<std::mutex> lock(action_mutex_);

            std::vector<std::tuple<UINT, uint64_t>> result;
            if (data_ptr_->active_size == 0) return result;

            result.reserve(data_ptr_->active_size);

            for (UINT i = 0; i < data_ptr_->active_size; ++i)
            {
                auto id = data_ptr_->active[i];
                result.emplace_back(id, data_ptr_->heap_pos[id]);
            }

            return result;
        }
    };
}
