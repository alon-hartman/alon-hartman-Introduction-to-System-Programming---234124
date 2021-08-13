#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <iostream>
#include <exception>
#include <string>

namespace mtm {
    const std::string message = "A game related error has occurred: ";
    const std::string illegal_argument = message + "IllegalArgument";
    const std::string illegal_cell = message + "IllegalCell";
    const std::string cell_empty = message + "CellEmpty";
    const std::string move_too_far = message + "MoveTooFar";
    const std::string cell_occupied = message + "CellOccupied";
    const std::string out_of_range = message + "OutOfRange";
    const std::string out_of_ammo = message + "OutOfAmmo";
    const std::string illegal_target = message + "IllegalTarget";


    class Exception : public std::exception 
    {
        public:
            // defaut constructor which initializes the error_message value to the given argument.
            // all of the derived classes use this to add their name to the error_message which will be returned in the
            // "what" method.
            explicit Exception(const std::string error_message = "");
            // returns error_message.
            virtual const char* what() const noexcept override;
            // destructor
            virtual ~Exception();
        protected:
            const char* error_message;
    };


    class IllegalArgument : public Exception
    {
        public:
            IllegalArgument();
            ~IllegalArgument() = default;
    };
    class IllegalCell : public Exception
    {
        public:
            IllegalCell();
            ~IllegalCell() = default;
    };
    class CellEmpty : public Exception
    {
        public:
            CellEmpty();
            ~CellEmpty() = default;
    };
    class MoveTooFar : public Exception
    {
        public:
            MoveTooFar();
            ~MoveTooFar() = default;
    };
    class CellOccupied : public Exception
    {
        public:
            CellOccupied();
            ~CellOccupied() = default;
    };
    class OutOfRange : public Exception
    {
        public:
            OutOfRange();
            ~OutOfRange() = default;
    };
    class OutOfAmmo : public Exception
    {
        public:
            OutOfAmmo();
            ~OutOfAmmo() = default;
    };
    class IllegalTarget : public Exception
    {
        public:
            IllegalTarget();
            ~IllegalTarget() = default;
    };
}
#endif