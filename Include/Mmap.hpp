#pragma once

#include <string_view>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include "PointerReverseIterator.hpp"

#include <stdexcept>

struct FileOpenException : std::runtime_error
{
    FileOpenException() : runtime_error{"Could not open file"} {}
};

class Mmap
{
    using descriptor = int;
    using filePtr = char*;

public:
    using iterator = char*;
    using const_iterator = const char*;

    using reverse_iterator = PointerReverseIterator<char>;
    using const_reverse_iterator = PointerReverseIterator<const char>;

    Mmap(std::string_view path)
        : desc{-1}, size{0}, file{nullptr}
    {
        openFile(path);
    }
    Mmap(const Mmap&) = delete;
    Mmap& operator=(const Mmap&) = delete;

    Mmap(Mmap&&) = default;
    Mmap& operator=(Mmap&&) = default;
    
    ~Mmap()
    {
        if(file != nullptr)
            munmap(file, size);

        if(desc != -1)
            close(desc);
    }

    iterator begin()
    {
        return file;
    }

    iterator end()
    {
        return file + size + 1;
    }

    const_iterator cbegin() const
    {
        return file;
    }

    const_iterator cend() const
    {
        return file + size + 1;
    }


    reverse_iterator rbegin()
    {
        return file + size;
    }

    reverse_iterator rend()
    {
        return file - 1;
    }

    const_reverse_iterator crbegin() const
    {
        return file + size;
    }

    const_reverse_iterator crend() const
    {
        return file - 1;
    }

private:
    void openFile(std::string_view path)
    {
        desc = open(path.data(), O_RDONLY); 
        if(desc == -1)
            throw FileOpenException();

        struct stat fileStats;
        if(fstat(desc, &fileStats) == -1)
            throw FileOpenException();
        size = fileStats.st_size;

        file = reinterpret_cast<char*>(mmap(NULL, size, PROT_READ, MAP_PRIVATE, desc, 0));
        if(file == MAP_FAILED)
            throw FileOpenException();
    }
    descriptor desc;
    unsigned size;
    filePtr file;
};
