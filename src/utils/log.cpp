#include "log.h"

Log::Log() :m_b_enabled(true) {}

Log::~Log(){close();}

bool Log::open(std::string s_file_name){
    m_to_log_file.open(s_file_name.c_str(), std::ios_base::out); //  | std::ios_base::app

    if( !m_to_log_file ){
        return false;
    }
    return true;
}

void Log::close(){
    if(m_to_log_file.is_open()){
        m_to_log_file.close();
    }
}

bool Log::common_log_init(std::string name){
    time_t t_now_time;
    time(&t_now_time);

    tm* t_local_time = localtime(&t_now_time);
    //data std::string
    std::string s_date_str = value_to_str(t_local_time->tm_year+1900) + "-" +
        value_to_str(t_local_time->tm_mon+1) + "-" +
        value_to_str(t_local_time->tm_mday);

    return open("./"+ name+s_date_str + ".txt");
}

void Log::enable(){
    m_b_enabled = true;
}

void Log::disable()
{
    m_b_enabled = false;
}

//get std::string now
std::string Log::get_time_str(){
    time_t t_now_time;
    time(&t_now_time);

    tm* t_local_time = localtime(&t_now_time);

    //"2011-07-18 23:03:01 ";
    std::string str_format = "%Y-%m-%d %H:%M:%S ";

    char str_date_time[30] = {'\0'};
    strftime(str_date_time, 30, str_format.c_str(), t_local_time);

    std::string str_res = str_date_time;

    return str_res;
}
