#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <memory>
#include <cstdio>

/**
 * Log class
 */
class Log {
public:
        Log();
        ~Log();

        bool open(std::string strFileName);
        void close();

        bool common_log_init(std::string name="log"); //

        void enable();
        void disable();

        std::string get_time_str();

        template <typename T> void log_out(const T& value){
                if (m_b_enabled){
                        m_to_log_file << value;
                }
        }

        template <typename T> void log_out_ln(const T& value){
                if (m_b_enabled)
                {
                        m_to_log_file << value << std::endl;
                }
        }

        void log_out_ln() {
                if (m_b_enabled){
                        m_to_log_file << std::endl;
                }
        }

        template <typename T> Log& operator<<(const T& value){
                if (m_b_enabled){
                        m_to_log_file << value;
                }
                return (*this);
        }

        Log& operator<<(std::ostream& (*_pfn)(std::ostream&)){
                if (m_b_enabled){
                        (*_pfn)(m_to_log_file);
                }
                return (*this);
        }


        template<typename ... Args> void log_format(const std::string& format, Args ... args) {
                if (m_b_enabled){
                        std::string str = string_format(format, args ...);
                        m_to_log_file << str;
                }
        }

        template<typename ... Args> std::string string_format(const std::string& format, Args ... args) {
                size_t size = std::snprintf(nullptr, 0, format.c_str(), args ... ) + 1; // extra space for '\0'
                std::unique_ptr<char[]> buf(new char[size]);
                std::snprintf(buf.get(), size, format.c_str(), args ...);
                return std::string(buf.get(), buf.get()+size-1); // delete '\0'
        }

        template<typename T> std::string value_to_str(T value){
                std::stringstream ost;
                ost << value;
                return ost.str();
        }


private:
        std::ofstream m_to_log_file;
        bool m_b_enabled;
};



#endif /* LOG_H */
