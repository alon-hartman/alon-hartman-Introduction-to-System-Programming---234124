#include "Exceptions.h"

#include <string>

using namespace mtm;

Exception::Exception(const std::string error_message) : error_message(error_message.c_str()) {    }
Exception::~Exception() {   }

const char* Exception::what() const noexcept
{
    return error_message;
}

IllegalArgument::IllegalArgument() : Exception(illegal_argument) { }

IllegalCell::IllegalCell() : Exception(illegal_cell) { }

CellEmpty::CellEmpty() : Exception(cell_empty) {   }

MoveTooFar::MoveTooFar() : Exception(move_too_far) {    }

CellOccupied::CellOccupied() : Exception(cell_occupied) {   }

OutOfRange::OutOfRange() : Exception(out_of_range) {    }

OutOfAmmo::OutOfAmmo() : Exception(out_of_ammo) {   }

IllegalTarget::IllegalTarget() : Exception(illegal_target) {   }
