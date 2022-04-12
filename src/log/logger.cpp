#include <fc/log/logger.hpp>
#include <fc/log/log_message.hpp>
#include <fc/exception/exception.hpp>
#include <fc/filesystem.hpp>
#include <unordered_map>
#include <string>
#include <fc/log/logger_config.hpp>

namespace fc {

    class logger::impl {
      public:
         impl( std::unique_ptr<spdlog::logger> agent_logger = nullptr)
         :_parent(nullptr),_enabled(true),_additivity(false),_level(log_level::warn)
         {
            if (!agent_logger)
               _agent_logger = std::make_unique<spdlog::logger>("", std::make_shared<spdlog::sinks::stderr_color_sink_st>());
            else
               _agent_logger = std::move(agent_logger);
            _agent_logger->set_level(spdlog::level::warn); // change agent logger's default level from `info` to `warn` to make it consistent with associated fc logger
         }
         fc::string       _name;
         logger           _parent;
         bool             _enabled;
         bool             _additivity;
         log_level        _level;
         std::unique_ptr<spdlog::logger> _agent_logger;
    };


    logger::logger()
    :my( new impl() ){}

    logger::logger(nullptr_t){}

    logger::logger( const string& name, const logger& parent )
    :my( new impl() )
    {
       my->_name = name;
       my->_parent = parent;
    }


    logger::logger( const logger& l )
    :my(l.my){}

    logger::logger( logger&& l )
    :my(fc::move(l.my)){}

    logger::~logger(){}

    logger& logger::operator=( const logger& l ){
       my = l.my;
       return *this;
    }
    logger& logger::operator=( logger&& l ){
       fc_swap(my,l.my);
       return *this;
    }
    bool operator==( const logger& l, std::nullptr_t ) { return !l.my; }
    bool operator!=( const logger& l, std::nullptr_t ) { return !!l.my;  }

    bool logger::is_enabled( log_level e )const {
       return e >= my->_level;
    }

    void logger::log( log_message m ) {
       std::unique_lock g( log_config::get().log_mutex );
       m.get_context().append_context( my->_name );

       if( my->_additivity && my->_parent != nullptr) {
          logger parent = my->_parent;
          g.unlock();
          parent.log( m );
       }
    }

    void logger::set_name( const fc::string& n ) { my->_name = n; }
    const fc::string& logger::name()const { return my->_name; }

    logger logger::get( const fc::string& s ) {
       return log_config::get_logger( s );
    }

    void logger::update( const fc::string& name, logger& log ) {
       log_config::update_logger( name, log );
    }

    logger  logger::get_parent()const { return my->_parent; }
    logger& logger::set_parent(const logger& p) { my->_parent = p; return *this; }

    log_level logger::get_log_level()const { return my->_level; }
    logger& logger::set_log_level(log_level ll) {
       my->_level = ll;
       switch (ll) {
          case fc::log_level::values::all:
             my->_agent_logger->set_level(spdlog::level::trace);
             break;
          case fc::log_level::values::debug:
             my->_agent_logger->set_level(spdlog::level::debug);
             break;
          case fc::log_level::values::info:
             my->_agent_logger->set_level(spdlog::level::info);
             break;
          case fc::log_level::values::warn:
             my->_agent_logger->set_level(spdlog::level::warn);
             break;
          case fc::log_level::values::error:
             my->_agent_logger->set_level(spdlog::level::err);
             break;
          case fc::log_level::values::off:
             my->_agent_logger->set_level(spdlog::level::off);
             break;
       }
       return *this;
    }

    std::unique_ptr<spdlog::logger>& logger::get_agent_logger()const { return my->_agent_logger;};
    void logger::set_agent_logger(std::unique_ptr<spdlog::logger> al) { my->_agent_logger = std::move(al); };

   bool configure_logging( const logging_config& cfg );
   bool do_default_config      = configure_logging( logging_config::default_config() );

} // namespace fc
