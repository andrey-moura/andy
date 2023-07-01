#pragma once

#include <string>

#include <console.hpp>
#include <core.hpp>

#define UVA_JOB_DECLARE(job_name) job_name();

#ifdef __UVA_JOB_COMPILATION__
    #define UVA_JOB_DEFINE(job_name) job_name::job_name () : basic_job(#job_name) {  }\
        int main(int argc, char** argv) {\
            uva::console::init_args(argc, argv); \
            std::shared_ptr<basic_job> job = std::make_shared<job_name>();\
            uva::job::basic_job::perform_async(job);\
        }
#else
    #define UVA_JOB_DEFINE(job_name) job_name::job_name () : basic_job(#job_name) {  }
#endif

namespace uva
{
    namespace job
    {
        class basic_job
        {
        public:
            basic_job(const std::string & __name);
        public:
            std::string name;
        protected:
            std::string m_command;
        public:
            virtual std::string command(var args);
            virtual int main(var args) { return 0; }
            
            virtual int perform_async(var args);
            virtual int perform_in_this_thread(var args);
        protected:
            template<class... Args>
            void log(const std::string& fmt, Args... args)
            {
                static std::string prefix = "[" + uva::string::toupper(name) + "] ";
                std::string msg = std::vformat(prefix + fmt, std::make_format_args(args...));
                uva::console::format_on_cout(msg);
            }
        public:
            static int perform_async(std::shared_ptr<basic_job> __job);
        };
        void log(const std::string& msg);
        template<class... Args>
        void log(const std::string& fmt, Args... args)
        {
            std::string msg = std::vformat(fmt, std::make_format_args(args...));
            log(msg);
        }
    }; // namespace job
    
}; // namespace uva
