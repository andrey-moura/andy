#include <application_controller.hpp>

#include <iostream>

#include <console.hpp>
#include <file.hpp>
#include <core.hpp>

#include <application_controller.hpp>

void func_1() {
    std::cout << "Chamando func_1()" << std::endl;
}

void func_2() {
    std::cout << "Chamando func_1()" << std::endl;
}

bool func_3() {
    std::cout << "Chamando func_3()" << std::endl;
    return true;
}


void application_controller::new_job()
{
    std::filesystem::path project_root = std::filesystem::current_path();
    std::filesystem::path job_include = project_root / "include" / "jobs";
    std::filesystem::path job_source = project_root / "src" / "jobs";

    if(!std::filesystem::exists(job_include) || !std::filesystem::exists(job_source)) {
        uva::console::log_error("error: cannot find jobs folder");
        return;
    }

    std::string job_name = params[0];

    const char* const job_header_format =
R"~~~(#include <job.hpp>

using namespace uva;
using namespace job;

class {}_job : public basic_job
{{
public:
    UVA_JOB_DECLARE({}_job);
public:
    int main(var args) override;
}};
)~~~";

    const char* const job_source_format =
R"~~~(#include <{}_job.hpp>

#include <iostream>

#include <console.hpp>
#include <file.hpp>
#include <core.hpp>

UVA_JOB_DEFINE({}_job);

int {}_job::main(var args)
{{
    return 0;
}}

)~~~";

    uva::file::write_all_text(job_include / (job_name + "_job.hpp"), std::format(job_header_format, job_name, job_name));
    uva::file::write_all_text(job_source  / (job_name + "_job.cpp"), std::format(job_source_format, job_name, job_name, job_name));
}
