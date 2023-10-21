#include "job.hpp"

#include <filesystem>
#include <json.hpp>
#include <diagnostics.hpp>

uva::job::basic_job::basic_job(const std::string &__name)
    : name(__name)
{
    std::string executable = uva::string::replace(name, '_', '-');
    m_command = std::filesystem::current_path() / executable;
}

std::string uva::job::basic_job::command(var args)
{
    std::string cmd = m_command;
    if(args != null) {
#if __UVA_DEBUG_LEVEL__ > 0
        if(!args.is_a<var::map>()) {
            throw std::logic_error("error: required args passed to job to be a map");
        }
#endif
        //size_t to_reserve = cmd.size()+json.size()+20;
        size_t to_reserve = 1024;
        cmd.reserve(to_reserve);

        auto& map = args.as<var::map>();

        for(auto& pair : map)
        {
            cmd += " --\"";
            cmd += pair.first.to_s();
            cmd += "=";
            cmd += pair.second.to_s();
            cmd += "\"";
        }

        UVA_CHECK_RESERVED_BUFFER(cmd, to_reserve);
    }

    return cmd;
}

int uva::job::basic_job::perform_async(var args)
{
//TODO 
//use system in perform_sync
    std::string cmd = command(args);

#ifdef __linux__
    cmd += " &";
#else
    static_assert(false, "error: uva::job::basic_job::perform_async not implemented for current platform");
#endif

    return system(cmd.c_str());
}

int uva::job::basic_job::perform_in_this_thread(var args)
{
    return main(args);
}

int uva::job::basic_job::perform_async(std::shared_ptr<uva::job::basic_job> __job)
{
    var args = uva::console::get_named_args();

    uva::job::log("Received {} to perform asynchronous with parameters {}", __job->name, args.to_s());

    auto result = uva::diagnostics::measure_function([=](){
        return __job->main(args);
    });

    uva::job::log("Performed {} asynchronous in {}", __job->name, uva::string::pretty_format_duration(result.second));

    return result.first;
}

void uva::job::log(const std::string &msg)
{
    static std::string prefix = "[JOB EXEC] ";
    uva::console::put_on_cout(prefix + msg);
}
