#include <iostream>

#include "log_analyzer.hpp"
#include "lib/utils/print_util.hpp" 


void LogAnalyzer::SetUp(int argc, char** argv) {
    err_code_ = args_parser_
        .AddFlag("help", 'h')
        .AddFlag("print", 'p')
        .AddOption("output", 'o')
        .AddOption("stats", 's')
        .AddOption("window", 'w')
        .AddOption("from", 'f')
        .AddOption("to", 'e')
        .Parse(argc, argv);

    if (err_code_ != 0) {
        return;
    }

    SetUpHelp();
    SetUpInput();
    SetUpOutput();
    SetUpPrint();
    SetUpStats();
    SetUpWindow();
    SetUpFrom();
    SetUpTo();
}

void LogAnalyzer::SetUpHelp() {
    if (args_parser_.GetFlag("help").value().declared) {
        PrintUtil::PrintlnInfo(USAGE_TEXT);
    }
}

void LogAnalyzer::SetUpInput() {
    auto free_args_size = args_parser_.GetFreeArgsSize();
    if (free_args_size == 0) {
        PrintUtil::PrintlnCrit("The input file is missing, nothing to do.");
        err_code_ = 1;

        return;
    }

    has_input_path_ = true;
    if (free_args_size > 1) {
        PrintUtil::PrintlnWarn("Multiple input files have been given, the last one will be selected.");
    }
    
    auto input_file_name = args_parser_.GetFreeArgs()[free_args_size - 1];
    input_stream_ = new std::ifstream(input_file_name);
    if (!input_stream_->is_open()) {
        PrintUtil::PrintlnCrit("Input file doesn't exist: ");
        PrintUtil::PrintlnCrit(input_file_name);
        err_code_ = 1;
    }
}

void LogAnalyzer::SetUpOutput() {
    auto opt = args_parser_.GetOption("output").value();
    has_output_path_ = opt.declared && opt.value_declared;
    if (has_output_path_) {
        output_stream_ = new std::ofstream(opt.value);

        if (!output_stream_->is_open()) {
            has_output_path_ = false;
            PrintUtil::PrintlnWarn("Can't open output file: ");
            PrintUtil::PrintlnWarn(opt.value);
            PrintUtil::PrintlnWarn("Output option was disabled.");
        }
    }
}

void LogAnalyzer::SetUpPrint() {
    print_error_requests_ = args_parser_.GetFlag("print").value().declared;
}

void LogAnalyzer::SetUpStats() {
    auto opt = args_parser_.GetOption("stats").value();
    print_most_freq_err_requests_ = opt.declared;
    if (opt.value_declared) {
        most_freq_err_requests_len_ = std::atoi(opt.value);
    }
}

void LogAnalyzer::SetUpWindow() {
    auto opt = args_parser_.GetOption("window").value();
    print_max_requests_window_ = opt.declared;
    if (opt.value_declared) {
        max_requests_window_len_seconds_ = std::atoi(opt.value);
    }
}

void LogAnalyzer::SetUpFrom() {
    auto opt = args_parser_.GetOption("from").value();
    has_lower_time_bound_ = opt.declared;
    if (opt.value_declared) {
        lower_time_bound_unix_ = std::atol(opt.value);
    }
}

void LogAnalyzer::SetUpTo() {
    auto opt = args_parser_.GetOption("to").value();
    has_upper_time_bound_ = opt.declared;
    if (opt.value_declared) {
        upper_time_bound_unix_ = std::atol(opt.value);
    }
}

int LogAnalyzer::Run(int argc, char** argv) {
    if (err_code_ != 0) {
        return err_code_;
    }

    char ch;
    while

    return err_code_;
}

bool LogAnalyzer::ReadRequest() {

}
