/*!
 * \author ddubois 
 * \date 21-Aug-18.
 */

#ifndef NOVA_RENDERER_LOGGER_HPP
#define NOVA_RENDERER_LOGGER_HPP

#include <string>
#include <functional>
#include <unordered_map>

namespace nova {
    enum class log_level {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
    };

    /*!
     * \brief A logger interface that can be implemented for whatever game Nova uses
     */
    class logger {
    public:
        static logger instance;

        void add_log_handler(log_level level, const std::function<void(std::string)>& log_handler);

        inline void log(log_level level, const std::string& msg);

    private:
        std::unordered_map<log_level, std::function<void(std::string)>> log_handlers;
    };
}


#endif //NOVA_RENDERER_LOGGER_HPP
