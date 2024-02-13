#ifndef IA_ERRORS_HPP
#define IA_ERRORS_HPP

namespace errors
{
    class NotPlayableMoveError {

    };

    class OutOfBoundError {

    };

    class UnAllocatedVariableError {

    };

    class SDLError {
        public : 
            std::string message;
            SDLError(std::string message) {
                this->message = message;
            }
    };
} // namespace errors


#endif