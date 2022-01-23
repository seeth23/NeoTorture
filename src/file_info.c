#include "file_info.h"
#include "string.h"

file_information
file_info_init(size_t initsize)
{
    file_information this;
    memset(&this, 0, sizeof(file_information));
    this.init_size = initsize;
    this.new_size = initsize;
    return this;
}
