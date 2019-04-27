#pragma once

#include <string_view>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include "PointerReverseIterator.hpp"

class Mmap
{
    using descriptor = int;
    using filePtr = char*;

public:
    using iterator = char*;
    using const_iterator = const char*;

    using reverse_iterator = PointerReverseIterator<char>;
    using const_reverse_iterator = PointerReverseIterator<const char>;

    Mmap()
        : desc{-1}, size{0}, file{nullptr}
    {}
    
    ~Mmap()
    {
        if(file != nullptr)
            munmap(file, size);

        if(desc != -1)
            close(desc);
    }

    bool openFile(std::string_view path)
    {
        desc = open(path.data(), O_RDONLY); 
        if(desc == -1)
            return false;

        struct stat fileStats;
        if(fstat(desc, &fileStats) == -1)
            return false;
        size = fileStats.st_size;

        file = reinterpret_cast<char*>(mmap(NULL, size, PROT_READ, MAP_PRIVATE, desc, 0));
        if(file == MAP_FAILED)
            return false;

        return true;
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
    descriptor desc;
    unsigned size;
    filePtr file;
};
